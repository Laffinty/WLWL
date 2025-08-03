#ifndef BUILTINS_H
#define BUILTINS_H

#include "environment.h" // 修正：包含完整定义

// 注册所有内置函数到环境中
void register_builtins(Environment* env);

#endif // BUILTINS_H
