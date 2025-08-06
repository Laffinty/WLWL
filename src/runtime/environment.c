#include "wl.h"
#include <string.h>
#include <stdio.h>

static Object* apply_function(Object* fn, DynArray* args);

static bool is_truthy(Object* obj) {
    if (!obj || obj == NULL_OBJ) {
        return false;
    }
    if (obj == FALSE_OBJ) {
        return false;
    }
    if (obj->type == OBJ_NUMBER && ((NumberObject*)obj)->value == 0) {
        return false;
    }
    if (obj->type == OBJ_STRING && strlen(((StringObject*)obj)->value) == 0) {
        return false;
    }
    return true;
}

Environment* create_environment() {
    Environment* env = (Environment*)malloc(sizeof(Environment));
    env->entries = da_create(sizeof(EnvEntry));
    env->outer = NULL;
    return env;
}

Environment* create_enclosed_environment(Environment* outer) {
    Environment* env = create_environment();
    env->outer = outer;
    return env;
}

void free_environment(Environment* env) {
    if (!env) return;
    for (int i = 0; i < da_count(env->entries); i++) {
        EnvEntry* entry = (EnvEntry*)da_get(env->entries, i);
        free(entry->name);
    }
    da_free(env->entries);
    free(env);
}

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

Object* env_set(Environment* env, const char* name, Object* value) {
    return env_define(env, name, value, true);
}

Object* env_define(Environment* env, const char* name, Object* value, bool is_mutable) {
    for (int i = 0; i < da_count(env->entries); i++) {
        EnvEntry* entry = (EnvEntry*)da_get(env->entries, i);
        if (strcmp(entry->name, name) == 0) {
            return create_error("identifier already declared: %s", name);
        }
    }
    
    EnvEntry new_entry;
    new_entry.name = strdup(name);
    new_entry.value = value;
    new_entry.is_mutable = is_mutable;
    da_push(env->entries, &new_entry);
    
    return value;
}

Object* env_assign(Environment* env, const char* name, Object* value) {
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
    
    if (env->outer != NULL) {
        return env_assign(env->outer, name, value);
    }
    
    return create_error("identifier not found: %s", name);
}

static Object* apply_function(Object* fn, DynArray* args) {
    if (fn->type == OBJ_BUILTIN) {
        BuiltinObject* builtin = (BuiltinObject*)fn;
        return builtin->function(args);
    }
    return create_error("not a function: %s", object_type_to_str(fn->type));
}

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
                        return result;
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
            return env_define(env, node->let_stmt.name->identifier.value, val, false);
        }
        case NODE_VAR_STATEMENT: {
            Object* val = eval(node->var_stmt.value, env);
            if (IS_ERROR(val)) {
                return val;
            }
            return env_define(env, node->var_stmt.name->identifier.value, val, true);
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
        case NODE_IF_EXPRESSION: {
            Object* condition = eval(node->if_expr.condition, env);
            if (IS_ERROR(condition)) {
                return condition;
            }
            
            if (is_truthy(condition)) {
                return eval(node->if_expr.then_branch, env);
            } else if (node->if_expr.else_branch) {
                return eval(node->if_expr.else_branch, env);
            } else {
                return NULL_OBJ;
            }
        }
        case NODE_COND_EXPRESSION: {
            for (int i = 0; i < da_count(node->cond_expr.branches); i++) {
                ASTNode** branch_ptr = (ASTNode**)da_get(node->cond_expr.branches, i);
                if (branch_ptr && *branch_ptr) {
                    ASTNode* branch = *branch_ptr;
                    Object* condition = eval(branch->cond_branch.condition, env);
                    if (IS_ERROR(condition)) {
                        return condition;
                    }
                    
                    if (is_truthy(condition)) {
                        return eval(branch->cond_branch.expression, env);
                    }
                }
            }
            return NULL_OBJ;
        }
        case NODE_ARRAY_LITERAL: {
            if (da_count(node->array_literal.elements) > 0) {
                ASTNode** first_elem_ptr = (ASTNode**)da_get(node->array_literal.elements, 0);
                if (first_elem_ptr) {
                    return eval(*first_elem_ptr, env);
                }
            }
            return NULL_OBJ;
        }
        default:
            return create_error("unknown node type: %d", node->type);
    }
}