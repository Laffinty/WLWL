#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../core/dyn_array.h"
#include "object.h"

typedef struct {
    char* name;
    Object* value;
    bool is_mutable;
} EnvEntry;

typedef struct Environment {
    struct Environment* outer;
    DynArray* entries;
} Environment;

Environment* create_environment();
Environment* create_enclosed_environment(Environment* outer);
void free_environment(Environment* env);
Object* env_get(Environment* env, const char* name);
Object* env_set(Environment* env, const char* name, Object* value);

Object* env_define(Environment* env, const char* name, Object* value, bool is_mutable);
Object* env_assign(Environment* env, const char* name, Object* value);

#endif