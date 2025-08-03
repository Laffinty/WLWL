#include "wl.h"
#include <string.h>
#include <stdio.h>

// Forward declaration
static Object* apply_function(Object* fn, DynArray* args);

// Creates a new, empty environment.
Environment* create_environment() {
    Environment* env = (Environment*)malloc(sizeof(Environment));
    env->entries = da_create(sizeof(EnvEntry));
    env->outer = NULL;
    return env;
}

// Creates a new environment enclosed by an outer one.
Environment* create_enclosed_environment(Environment* outer) {
    Environment* env = create_environment();
    env->outer = outer;
    return env;
}

// Frees an environment and all its entries.
void free_environment(Environment* env) {
    if (!env) return;
    for (int i = 0; i < da_count(env->entries); i++) {
        EnvEntry* entry = (EnvEntry*)da_get(env->entries, i);
        free(entry->name);
        // Note: We don't free entry->value here as its ownership is managed elsewhere
    }
    da_free(env->entries);
    free(env);
}

// Gets a value from the environment, checking outer environments if necessary.
Object* env_get(Environment* env, const char* name) {
    for (int i = 0; i < da_count(env->entries); i++) {
        EnvEntry* entry = (EnvEntry*)da_get(env->entries, i);
        if (strcmp(entry->name, name) == 0) {
            return entry->value;
        }
    }
    if (env->outer != NULL) {
        return env_get(env->outer, name);
    }
    return NULL;
}

// Sets a value in the environment (legacy function for builtin registration).
Object* env_set(Environment* env, const char* name, Object* value) {
    return env_define(env, name, value, true); // Default to mutable for compatibility
}

// Define a new variable with mutability flag
Object* env_define(Environment* env, const char* name, Object* value, bool is_mutable) {
    // Check if variable already exists in current scope
    for (int i = 0; i < da_count(env->entries); i++) {
        EnvEntry* entry = (EnvEntry*)da_get(env->entries, i);
        if (strcmp(entry->name, name) == 0) {
            // Variable already exists - this is an error for redefinition
            return create_error("identifier already declared: %s", name);
        }
    }
    
    // Create new entry
    EnvEntry new_entry;
    new_entry.name = strdup(name);
    new_entry.value = value;
    new_entry.is_mutable = is_mutable;
    da_push(env->entries, &new_entry);
    
    return value;
}

// Assign to an existing variable (for SET statements)
Object* env_assign(Environment* env, const char* name, Object* value) {
    // Search in current environment first
    for (int i = 0; i < da_count(env->entries); i++) {
        EnvEntry* entry = (EnvEntry*)da_get(env->entries, i);
        if (strcmp(entry->name, name) == 0) {
            if (!entry->is_mutable) {
                return create_error("cannot assign to immutable variable: %s", name);
            }
            entry->value = value;
            return value;
        }
    }
    
    // Search in outer environments
    if (env->outer != NULL) {
        return env_assign(env->outer, name, value);
    }
    
    return create_error("identifier not found: %s", name);
}

// Apply a function to arguments
static Object* apply_function(Object* fn, DynArray* args) {
    if (fn->type == OBJ_BUILTIN) {
        BuiltinObject* builtin = (BuiltinObject*)fn;
        return builtin->function(args);
    }
    // Future: handle user-defined functions
    return create_error("not a function: %s", object_type_to_str(fn->type));
}

// Main evaluation function
Object* eval(ASTNode* node, Environment* env) {
    if (!node) {
        return NULL;
    }

    switch (node->type) {
        case NODE_PROGRAM: {
            Object* result = NULL;
            for (int i = 0; i < da_count(node->program.statements); i++) {
                ASTNode** stmt_ptr = (ASTNode**)da_get(node->program.statements, i);
                if (stmt_ptr) {
                    result = eval(*stmt_ptr, env);
                    if (result && IS_ERROR(result)) {
                        return result; // Stop on error
                    }
                }
            }
            return result;
        }
        case NODE_LET_STATEMENT: {
            Object* val = eval(node->let_stmt.value, env);
            if (IS_ERROR(val)) {
                return val;
            }
            return env_define(env, node->let_stmt.name->identifier.value, val, false); // immutable
        }
        case NODE_VAR_STATEMENT: {
            Object* val = eval(node->var_stmt.value, env);
            if (IS_ERROR(val)) {
                return val;
            }
            return env_define(env, node->var_stmt.name->identifier.value, val, true); // mutable
        }
        case NODE_SET_STATEMENT: {
            Object* val = eval(node->set_stmt.value, env);
            if (IS_ERROR(val)) {
                return val;
            }
            return env_assign(env, node->set_stmt.name->identifier.value, val);
        }
        case NODE_EXPRESSION_STATEMENT:
            return eval(node->expr_stmt.expression, env);
        case NODE_NUMBER_LITERAL:
            return create_number(node->number_literal.value);
        case NODE_STRING_LITERAL:
            return create_string(node->string_literal.value);
        case NODE_BOOLEAN_LITERAL:
            return node->boolean_literal.value ? TRUE_OBJ : FALSE_OBJ;
        case NODE_IDENTIFIER: {
            Object* val = env_get(env, node->identifier.value);
            if (!val) {
                return create_error("identifier not found: %s", node->identifier.value);
            }
            return val;
        }
        case NODE_CALL_EXPRESSION: {
            Object* function = eval(node->call_expr.function, env);
            if (IS_ERROR(function)) {
                return function;
            }
            
            // Evaluate arguments
            DynArray* args = da_create(sizeof(Object*));
            for (int i = 0; i < da_count(node->call_expr.arguments); i++) {
                ASTNode** arg_ptr = (ASTNode**)da_get(node->call_expr.arguments, i);
                if (arg_ptr) {
                    Object* evaluated = eval(*arg_ptr, env);
                    if (IS_ERROR(evaluated)) {
                        da_free(args);
                        return evaluated;
                    }
                    da_push(args, &evaluated);
                }
            }
            
            Object* result = apply_function(function, args);
            da_free(args);
            return result;
        }
        default:
            return create_error("unknown node type: %d", node->type);
    }
}