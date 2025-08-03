#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../core/dyn_array.h"
#include "object.h"

// 环境中的条目
typedef struct {
    char* name;
    Object* value;
    bool is_mutable; // 添加可变性标记
} EnvEntry;

// 环境结构体
typedef struct Environment {
    struct Environment* outer;
    DynArray* entries;
} Environment;

// --- 公共函数 ---
Environment* create_environment();
Environment* create_enclosed_environment(Environment* outer);
void free_environment(Environment* env);
Object* env_get(Environment* env, const char* name);
Object* env_set(Environment* env, const char* name, Object* value);

// 新增函数：支持可变性设置
Object* env_define(Environment* env, const char* name, Object* value, bool is_mutable);
Object* env_assign(Environment* env, const char* name, Object* value);

#endif // ENVIRONMENT_H