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

// Built-in function for not equal comparison.
static Object* builtin_ne(DynArray* args) {
    Object* eq_result = builtin_eq(args);
    if (IS_ERROR(eq_result)) {
        return eq_result;
    }
    return eq_result == TRUE_OBJ ? FALSE_OBJ : TRUE_OBJ;
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

// Built-in function for less than comparison.
static Object* builtin_lt(DynArray* args) {
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
        return create_error("arguments to '<' must be NUMBER");
    }
    
    return ((NumberObject*)left)->value < ((NumberObject*)right)->value ? TRUE_OBJ : FALSE_OBJ;
}

// Built-in function for greater than or equal comparison.
static Object* builtin_gte(DynArray* args) {
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
        return create_error("arguments to '>=' must be NUMBER");
    }
    
    return ((NumberObject*)left)->value >= ((NumberObject*)right)->value ? TRUE_OBJ : FALSE_OBJ;
}

// Built-in function for less than or equal comparison.
static Object* builtin_lte(DynArray* args) {
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
        return create_error("arguments to '<=' must be NUMBER");
    }
    
    return ((NumberObject*)left)->value <= ((NumberObject*)right)->value ? TRUE_OBJ : FALSE_OBJ;
}

// 辅助函数：将对象转换为字符串表示
static char* object_to_string(Object* obj) {
    if (!obj) return strdup("NULL");
    
    switch (obj->type) {
        case OBJ_STRING:
            return strdup(((StringObject*)obj)->value);
        case OBJ_NUMBER: {
            char buffer[64];
            double val = ((NumberObject*)obj)->value;
            // 如果是整数，不显示小数点
            if (val == (long)val) {
                snprintf(buffer, sizeof(buffer), "%.0f", val);
            } else {
                snprintf(buffer, sizeof(buffer), "%g", val);
            }
            return strdup(buffer);
        }
        case OBJ_BOOLEAN:
            return strdup(((BooleanObject*)obj)->value ? "TRUE" : "FALSE");
        case OBJ_NULL:
            return strdup("NULL");
        default:
            return strdup("[object]");
    }
}

// Built-in function for string concatenation - 修复版本，支持自动类型转换
static Object* builtin_concat(DynArray* args) {
    if (da_count(args) == 0) {
        return create_string("");
    }
    
    // 计算总长度
    int total_len = 0;
    char** string_args = malloc(da_count(args) * sizeof(char*));
    
    for (int i = 0; i < da_count(args); i++) {
        Object** arg_ptr = (Object**)da_get(args, i);
        if (!arg_ptr) {
            string_args[i] = strdup("");
            continue;
        }
        
        Object* arg = *arg_ptr;
        string_args[i] = object_to_string(arg);
        total_len += strlen(string_args[i]);
    }
    
    // 分配结果字符串
    char* result = malloc(total_len + 1);
    result[0] = '\0';
    
    // 连接所有字符串
    for (int i = 0; i < da_count(args); i++) {
        strcat(result, string_args[i]);
        free(string_args[i]);
    }
    free(string_args);
    
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
    env_set(env, "!=", create_builtin(builtin_ne));
    env_set(env, ">", create_builtin(builtin_gt));
    env_set(env, "<", create_builtin(builtin_lt));
    env_set(env, ">=", create_builtin(builtin_gte));
    env_set(env, "<=", create_builtin(builtin_lte));
    
    // String functions
    env_set(env, "CONCAT", create_builtin(builtin_concat));
    
    // I/O functions
    env_set(env, "PRINT", create_builtin(builtin_print));
    
    // 重要修复：注册常量值
    env_set(env, "NULL", NULL_OBJ);
    env_set(env, "TRUE", TRUE_OBJ);
    env_set(env, "FALSE", FALSE_OBJ);
}