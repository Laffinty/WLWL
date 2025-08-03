#include "wl.h"

// Built-in function for addition.
static Object* builtin_add(DynArray* args) {
    double sum = 0;
    for (int i = 0; i < da_count(args); i++) {
        // da_get returns a pointer to the element in the array.
        // Since the array stores Object*, the element is an Object*.
        // So da_get gives us a pointer to an Object*, i.e., Object**.
        Object** arg_ptr = (Object**)da_get(args, i);
        if (!arg_ptr) continue;
        
        Object* arg = *arg_ptr; // Dereference to get the actual Object*
        if (arg->type != OBJ_NUMBER) {
            return create_error("argument to '+' must be NUMBER, got %s", object_type_to_str(arg->type));
        }
        // Cast to NumberObject to access the value
        sum += ((NumberObject*)arg)->value;
    }
    return create_number(sum);
}

// Register all built-in functions in the environment.
void register_builtins(Environment* env) {
    env_set(env, "+", create_builtin(builtin_add));
    // Register other built-in functions here, e.g., "-", "*", "/"
}
