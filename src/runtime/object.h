#ifndef OBJECT_H
#define OBJECT_H

#include "../core/dyn_array.h"
#include <stdbool.h>

// --- 前向声明 ---
struct ASTNode;
struct Environment;

// --- 对象类型枚举 ---
typedef enum {
    OBJ_NUMBER,
    OBJ_BOOLEAN,
    OBJ_NULL,
    OBJ_RETURN_VALUE,
    OBJ_ERROR,
    OBJ_FUNCTION,
    OBJ_STRING,
    OBJ_BUILTIN,
    
    // 新增循环控制对象类型
    OBJ_BREAK,
    OBJ_CONTINUE,
} ObjectType;

// --- 对象核心结构 ---
typedef struct Object {
    ObjectType type;
} Object;

// --- 具体对象结构 ---
typedef struct {
    Object base;
    double value;
} NumberObject;

typedef struct {
    Object base;
    char* value;
} StringObject;

typedef struct {
    Object base;
    bool value;
} BooleanObject;

typedef struct {
    Object base;
    Object* value;
} ReturnValueObject;

typedef struct {
    Object base;
    char* message;
} ErrorObject;

typedef struct {
    Object base;
    DynArray* parameters;
    struct ASTNode* body;
    struct Environment* env;
} FunctionObject;

typedef Object* (*BuiltinFunction)(DynArray* args);

typedef struct {
    Object base;
    BuiltinFunction function;
} BuiltinObject;

// 新增循环控制对象结构
typedef struct {
    Object base;
} BreakObject;

typedef struct {
    Object base;
} ContinueObject;

// --- 单例对象 ---
extern Object* TRUE_OBJ;
extern Object* FALSE_OBJ;
extern Object* NULL_OBJ;
extern Object* BREAK_OBJ;
extern Object* CONTINUE_OBJ;

// --- 辅助宏和函数 ---
#define IS_ERROR(obj) ((obj) != NULL && (obj)->type == OBJ_ERROR)
#define IS_BREAK(obj) ((obj) != NULL && (obj)->type == OBJ_BREAK)
#define IS_CONTINUE(obj) ((obj) != NULL && (obj)->type == OBJ_CONTINUE)

void init_global_objects();
Object* create_number(double value);
Object* create_string(char* value);
Object* create_return_value(Object* value);
Object* create_error(const char* format, ...);
Object* create_function(DynArray* parameters, struct ASTNode* body, struct Environment* env);
Object* create_builtin(BuiltinFunction function);

// 新增循环控制对象创建函数
Object* create_break();
Object* create_continue();

void free_object(Object* object);
void print_object(Object* object);
const char* object_type_to_str(ObjectType type);

#endif // OBJECT_H