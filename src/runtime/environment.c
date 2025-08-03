#include "wl.h"
#include <string.h>
#include <stdio.h>

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

// Sets a value in the environment.
Object* env_set(Environment* env, const char* name, Object* value) {
    // First, check if the variable already exists and update it.
    for (int i = 0; i < da_count(env->entries); i++) {
        EnvEntry* entry = (EnvEntry*)da_get(env->entries, i);
        if (strcmp(entry->name, name) == 0) {
            entry->value = value; // Update existing entry
            return value;
        }
    }
    
    // If not, create a new entry.
    EnvEntry new_entry;
    new_entry.name = strdup(name);
    new_entry.value = value;
    da_push(env->entries, &new_entry);
    
    return value;
}


// Main evaluation function (moved from evaluator.c)
Object* eval(ASTNode* node, Environment* env) {
    if (!node) {
        return NULL; // Or an error object
    }

    switch (node->type) {
        case NODE_PROGRAM: {
            Object* result = NULL;
            for (int i = 0; i < da_count(node->program.statements); i++) {
                ASTNode** stmt_ptr = (ASTNode**)da_get(node->program.statements, i);
                if (stmt_ptr) {
                    result = eval(*stmt_ptr, env);
                    // Could add logic for return statements here
                }
            }
            return result;
        }
        case NODE_NUMBER_LITERAL:
            return create_number(node->number_literal.value);
        case NODE_IDENTIFIER:
            return env_get(env, node->identifier.name);
        // Add other cases for different node types (IF, LET, CALL, etc.)
        default:
            return create_error("Unknown node type: %d", node->type);
    }
}
