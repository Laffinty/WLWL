#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "object.h"
#include "../core/dyn_array.h" // Explicitly include for DynArray

// An entry in the environment, mapping a name to an object.
typedef struct {
    char* name;
    Object* value;
} EnvEntry;

// The environment holds variable bindings. It can be nested to create scopes.
typedef struct Environment {
    struct Environment* outer; // Pointer to the enclosing environment
    DynArray* entries;         // DynArray of EnvEntry
} Environment;

// --- Public Functions ---

// Creates a new, empty environment.
Environment* create_environment();

// Creates a new environment that extends an existing one (for function calls).
Environment* create_enclosed_environment(Environment* outer);

// Frees an environment and all its entries.
void free_environment(Environment* env);

// Gets a value from the environment, searching outer scopes if necessary.
Object* env_get(Environment* env, const char* name);

// Sets a value in the environment.
Object* env_set(Environment* env, const char* name, Object* value);

#endif // ENVIRONMENT_H
