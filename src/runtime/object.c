#include "wl.h"

// --- 全局单例对象定义 ---
Object* TRUE_OBJ = NULL;
Object* FALSE_OBJ = NULL;
Object* NULL_OBJ = NULL;

// --- 辅助函数 ---
static Object* allocate_object(ObjectType type, size_t size) {
    Object* obj = malloc(size);
    if (obj == NULL) {
        log_error("Memory allocation failed in allocate_object.");
        exit(1);
    }
    obj->type = type;
    return obj;
}

// --- 公共函数 ---
void init_global_objects() {
    BooleanObject* true_obj = malloc(sizeof(BooleanObject));
    true_obj->base.type = OBJ_BOOLEAN;
    true_obj->value = true;
    TRUE_OBJ = (Object*)true_obj;

    BooleanObject* false_obj = malloc(sizeof(BooleanObject));
    false_obj->base.type = OBJ_BOOLEAN;
    false_obj->value = false;
    FALSE_OBJ = (Object*)false_obj;

    Object* null_obj = malloc(sizeof(Object));
    null_obj->type = OBJ_NULL;
    NULL_OBJ = null_obj;
}

Object* create_number(double value) {
    NumberObject* obj = (NumberObject*)allocate_object(OBJ_NUMBER, sizeof(NumberObject));
    obj->value = value;
    return (Object*)obj;
}

Object* create_string(char* value) {
    StringObject* obj = (StringObject*)allocate_object(OBJ_STRING, sizeof(StringObject));
    obj->value = strdup(value);
    return (Object*)obj;
}

Object* create_return_value(Object* value) {
    ReturnValueObject* obj = (ReturnValueObject*)allocate_object(OBJ_RETURN_VALUE, sizeof(ReturnValueObject));
    obj->value = value;
    return (Object*)obj;
}

Object* create_error(const char* format, ...) {
    ErrorObject* obj = (ErrorObject*)allocate_object(OBJ_ERROR, sizeof(ErrorObject));
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    obj->message = strdup(buffer);
    return (Object*)obj;
}

Object* create_function(DynArray* parameters, struct ASTNode* body, struct Environment* env) {
    FunctionObject* obj = (FunctionObject*)allocate_object(OBJ_FUNCTION, sizeof(FunctionObject));
    obj->parameters = parameters;
    obj->body = body;
    obj->env = env;
    return (Object*)obj;
}

Object* create_builtin(BuiltinFunction function) {
    BuiltinObject* obj = (BuiltinObject*)allocate_object(OBJ_BUILTIN, sizeof(BuiltinObject));
    obj->function = function;
    return (Object*)obj;
}

void free_object(Object* object) {
    if (object == NULL || object == TRUE_OBJ || object == FALSE_OBJ || object == NULL_OBJ) {
        return;
    }

    switch (object->type) {
        case OBJ_STRING:
            free(((StringObject*)object)->value);
            break;
        case OBJ_ERROR:
            free(((ErrorObject*)object)->message);
            break;
        case OBJ_RETURN_VALUE:
            free_object(((ReturnValueObject*)object)->value);
            break;
        case OBJ_FUNCTION:
            break;
        default:
            break;
    }
    free(object);
}

void print_object(Object* object) {
    if (object == NULL) { printf("NULL object pointer\n"); return; }
    switch (object->type) {
        case OBJ_NUMBER: printf("%g", ((NumberObject*)object)->value); break;
        case OBJ_STRING: printf("\"%s\"", ((StringObject*)object)->value); break;
        case OBJ_BOOLEAN: printf(((BooleanObject*)object)->value ? "TRUE" : "FALSE"); break;
        case OBJ_NULL: printf("NULL"); break;
        case OBJ_RETURN_VALUE: print_object(((ReturnValueObject*)object)->value); break;
        case OBJ_ERROR: printf("Error: %s", ((ErrorObject*)object)->message); break;
        case OBJ_BUILTIN: printf("[builtin function]"); break;
        case OBJ_FUNCTION: {
            FunctionObject* fn = (FunctionObject*)object;
            printf("fun(");
            for (int i = 0; i < da_count(fn->parameters); i++) {
                ASTNode* param_node = da_get(fn->parameters, i);
                printf("%s", param_node->identifier.value); // 修正: .value
                if (i < da_count(fn->parameters) - 1) printf(", ");
            }
            printf(") { ... }");
            break;
        }
        default: printf("Unknown object type: %d\n", object->type); break;
    }
}

const char* object_type_to_str(ObjectType type) {
    switch(type) {
        case OBJ_NUMBER: return "NUMBER";
        case OBJ_STRING: return "STRING";
        case OBJ_BOOLEAN: return "BOOLEAN";
        case OBJ_NULL: return "NULL";
        case OBJ_RETURN_VALUE: return "RETURN_VALUE";
        case OBJ_ERROR: return "ERROR";
        case OBJ_FUNCTION: return "FUNCTION";
        case OBJ_BUILTIN: return "BUILTIN";
        default: return "UNKNOWN";
    }
}
