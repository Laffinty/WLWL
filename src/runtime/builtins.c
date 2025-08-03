#include "wl.h"

// Built-in function for addition.
static Object* builtin_add(DynArray* args) {
    double sum = 0;
    for (int i = 0; i < da_count(args); i++) {
        Object** arg_ptr = (Object**)da_get(args, i);
        if (!arg_ptr) continue;
        
        Object* arg = *arg_ptr;
        if (arg->type != OBJ_NUMBER) {
            return create_error("argument to '+' must be NUMBER, got %s", object_type_to_str(arg->type));
        }
        sum += ((NumberObject*)arg)->value;
    }
    return create_number(sum);
}

// Built-in function for subtraction.
static Object* builtin_sub(DynArray* args) {
    if (da_count(args) != 2) {
        return create_error("wrong number of arguments. got=%d, want=2", da_count(args));
    }
    
    Object** left_ptr = (Object**)da_get(args, 0);
    Object** right_ptr = (Object**)da_get(args, 1);
    
    if (!left_ptr || !right_ptr) {
        return create_error("null argument");
    }
    
    Object* left = *left_ptr;
    Object* right = *right_ptr;
    
    if (left->type != OBJ_NUMBER || right->type != OBJ_NUMBER) {
        return create_error("arguments to '-' must be NUMBER");
    }
    
    double result = ((NumberObject*)left)->value - ((NumberObject*)right)->value;
    return create_number(result);
}

// Built-in function for multiplication.
static Object* builtin_mul(DynArray* args) {
    double product = 1;
    for (int i = 0; i < da_count(args); i++) {
        Object** arg_ptr = (Object**)da_get(args, i);
        if (!arg_ptr) continue;
        
        Object* arg = *arg_ptr;
        if (arg->type != OBJ_NUMBER) {
            return create_error("argument to '*' must be NUMBER, got %s", object_type_to_str(arg->type));
        }
        product *= ((NumberObject*)arg)->value;
    }
    return create_number(product);
}

// Built-in function for division.
static Object* builtin_div(DynArray* args) {
    if (da_count(args) != 2) {
        return create_error("wrong number of arguments. got=%d, want=2", da_count(args));
    }
    
    Object** left_ptr = (Object**)da_get(args, 0);
    Object** right_ptr = (Object**)da_get(args, 1);
    
    if (!left_ptr || !right_ptr) {
        return create_error("null argument");
    }
    
    Object* left = *left_ptr;
    Object* right = *right_ptr;
    
    if (left->type != OBJ_NUMBER || right->type != OBJ_NUMBER) {
        return create_error("arguments to '/' must be NUMBER");
    }
    
    double divisor = ((NumberObject*)right)->value;
    if (divisor == 0) {
        return create_error("division by zero");
    }
    
    double result = ((NumberObject*)left)->value / divisor;
    return create_number(result);
}

// Built-in function for equality comparison.
static Object* builtin_eq(DynArray* args) {
    if (da_count(args) != 2) {
        return create_error("wrong number of arguments. got=%d, want=2", da_count(args));
    }
    
    Object** left_ptr = (Object**)da_get(args, 0);
    Object** right_ptr = (Object**)da_get(args, 1);
    
    if (!left_ptr || !right_ptr) {
        return create_error("null argument");
    }
    
    Object* left = *left_ptr;
    Object* right = *right_ptr;
    
    if (left->type != right->type) {
        return FALSE_OBJ;
    }
    
    switch (left->type) {
        case OBJ_NUMBER:
            return ((NumberObject*)left)->value == ((NumberObject*)right)->value ? TRUE_OBJ : FALSE_OBJ;
        case OBJ_STRING:
            return strcmp(((StringObject*)left)->value, ((StringObject*)right)->value) == 0 ? TRUE_OBJ : FALSE_OBJ;
        case OBJ_BOOLEAN:
            return ((BooleanObject*)left)->value == ((BooleanObject*)right)->value ? TRUE_OBJ : FALSE_OBJ;
        case OBJ_NULL:
            return TRUE_OBJ;
        default:
            return FALSE_OBJ;
    }
}

// Built-in function for greater than comparison.
static Object* builtin_gt(DynArray* args) {
    if (da_count(args) != 2) {
        return create_error("wrong number of arguments. got=%d, want=2", da_count(args));
    }
    
    Object** left_ptr = (Object**)da_get(args, 0);
    Object** right_ptr = (Object**)da_get(args, 1);
    
    if (!left_ptr || !right_ptr) {
        return create_error("null argument");
    }
    
    Object* left = *left_ptr;
    Object* right = *right_ptr;
    
    if (left->type != OBJ_NUMBER || right->type != OBJ_NUMBER) {
        return create_error("arguments to '>' must be NUMBER");
    }
    
    return ((NumberObject*)left)->value > ((NumberObject*)right)->value ? TRUE_OBJ : FALSE_OBJ;
}

// Built-in function for string concatenation.
static Object* builtin_concat(DynArray* args) {
    if (da_count(args) == 0) {
        return create_string("");
    }
    
    // Calculate total length needed
    int total_len = 0;
    for (int i = 0; i < da_count(args); i++) {
        Object** arg_ptr = (Object**)da_get(args, i);
        if (!arg_ptr) continue;
        
        Object* arg = *arg_ptr;
        if (arg->type != OBJ_STRING) {
            return create_error("argument to 'CONCAT' must be STRING, got %s", object_type_to_str(arg->type));
        }
        total_len += strlen(((StringObject*)arg)->value);
    }
    
    // Allocate result string
    char* result = malloc(total_len + 1);
    result[0] = '\0';
    
    // Concatenate all strings
    for (int i = 0; i < da_count(args); i++) {
        Object** arg_ptr = (Object**)da_get(args, i);
        if (arg_ptr) {
            Object* arg = *arg_ptr;
            strcat(result, ((StringObject*)arg)->value);
        }
    }
    
    Object* str_obj = create_string(result);
    free(result);
    return str_obj;
}

// Built-in function for printing (returns NULL)
static Object* builtin_print(DynArray* args) {
    for (int i = 0; i < da_count(args); i++) {
        Object** arg_ptr = (Object**)da_get(args, i);
        if (arg_ptr) {
            print_object(*arg_ptr);
            if (i < da_count(args) - 1) {
                printf(" ");
            }
        }
    }
    printf("\n");
    return NULL_OBJ;
}

// Register all built-in functions in the environment.
void register_builtins(Environment* env) {
    // Arithmetic functions
    env_set(env, "+", create_builtin(builtin_add));
    env_set(env, "-", create_builtin(builtin_sub));
    env_set(env, "*", create_builtin(builtin_mul));
    env_set(env, "/", create_builtin(builtin_div));
    
    // Comparison functions
    env_set(env, "=", create_builtin(builtin_eq));
    env_set(env, ">", create_builtin(builtin_gt));
    
    // String functions
    env_set(env, "CONCAT", create_builtin(builtin_concat));
    
    // I/O functions
    env_set(env, "PRINT", create_builtin(builtin_print));
}