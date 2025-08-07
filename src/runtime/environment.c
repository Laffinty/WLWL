#include "wl.h"
#include <string.h>
#include <stdio.h>

static Object* apply_function(Object* fn, DynArray* args);

static bool is_truthy(Object* obj) {
    if (!obj || obj == NULL_OBJ) {
        return false;
    }
    if (obj == FALSE_OBJ) {
        return false;
    }
    if (obj->type == OBJ_NUMBER && ((NumberObject*)obj)->value == 0) {
        return false;
    }
    if (obj->type == OBJ_STRING && strlen(((StringObject*)obj)->value) == 0) {
        return false;
    }
    return true;
}

Environment* create_environment() {
    Environment* env = (Environment*)malloc(sizeof(Environment));
    env->entries = da_create(sizeof(EnvEntry));
    env->outer = NULL;
    return env;
}

Environment* create_enclosed_environment(Environment* outer) {
    Environment* env = create_environment();
    env->outer = outer;
    return env;
}

void free_environment(Environment* env) {
    if (!env) return;
    for (int i = 0; i < da_count(env->entries); i++) {
        EnvEntry* entry = (EnvEntry*)da_get(env->entries, i);
        free(entry->name);
    }
    da_free(env->entries);
    free(env);
}

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

Object* env_set(Environment* env, const char* name, Object* value) {
    return env_define(env, name, value, true);
}

Object* env_define(Environment* env, const char* name, Object* value, bool is_mutable) {
    for (int i = 0; i < da_count(env->entries); i++) {
        EnvEntry* entry = (EnvEntry*)da_get(env->entries, i);
        if (strcmp(entry->name, name) == 0) {
            return create_error("identifier already declared: %s", name);
        }
    }
    
    EnvEntry new_entry;
    new_entry.name = strdup(name);
    new_entry.value = value;
    new_entry.is_mutable = is_mutable;
    da_push(env->entries, &new_entry);
    
    return value;
}

Object* env_assign(Environment* env, const char* name, Object* value) {
    for (int i = 0; i < da_count(env->entries); i++) {
        EnvEntry* entry = (EnvEntry*)da_get(env->entries, i);
        if (strcmp(entry->name, name) == 0) {
            if (!entry->is_mutable) {
                return create_error("cannot assign to immutable variable: %s", name);
            }
            entry->value = value;
            return value;
        }
    }
    
    if (env->outer != NULL) {
        return env_assign(env->outer, name, value);
    }
    
    return create_error("identifier not found: %s", name);
}

static Object* apply_function(Object* fn, DynArray* args) {
    if (fn->type == OBJ_BUILTIN) {
        BuiltinObject* builtin = (BuiltinObject*)fn;
        return builtin->function(args);
    }
    return create_error("not a function: %s", object_type_to_str(fn->type));
}

Object* eval(ASTNode* node, Environment* env) {
    if (!node) {
        return NULL;
    }

    switch (node->type) {
        case NODE_PROGRAM: {
            Object* result = NULL;
            for (int i = 0; i < da_count(node->program.statements); i++) {
                ASTNode** stmt_ptr = (ASTNode**)da_get(node->program.statements, i);
                if (stmt_ptr) {
                    result = eval(*stmt_ptr, env);
                    if (result && IS_ERROR(result)) {
                        return result;
                    }
                    // 检查循环控制语句，在程序级别也需要传播
                    if (result && (IS_BREAK(result) || IS_CONTINUE(result))) {
                        return result;
                    }
                }
            }
            return result ? result : NULL_OBJ;
        }
        // 新增语句块执行逻辑
        case NODE_BLOCK_EXPRESSION: {
            Object* result = NULL;
            for (int i = 0; i < da_count(node->block_expr.statements); i++) {
                ASTNode** stmt_ptr = (ASTNode**)da_get(node->block_expr.statements, i);
                if (stmt_ptr) {
                    result = eval(*stmt_ptr, env);
                    if (result && IS_ERROR(result)) {
                        return result;
                    }
                    // 检查循环控制语句
                    if (result && (IS_BREAK(result) || IS_CONTINUE(result))) {
                        return result;
                    }
                }
            }
            return result ? result : NULL_OBJ;
        }
        case NODE_LET_STATEMENT: {
            Object* val = eval(node->let_stmt.value, env);
            if (IS_ERROR(val)) {
                return val;
            }
            Object* result = env_define(env, node->let_stmt.name->identifier.value, val, false);
            // 如果是错误，返回错误；否则返回定义的值
            return IS_ERROR(result) ? result : val;
        }
        case NODE_VAR_STATEMENT: {
            Object* val = eval(node->var_stmt.value, env);
            if (IS_ERROR(val)) {
                return val;
            }
            Object* result = env_define(env, node->var_stmt.name->identifier.value, val, true);
            // 如果是错误，返回错误；否则返回定义的值
            return IS_ERROR(result) ? result : val;
        }
        case NODE_SET_STATEMENT: {
            Object* val = eval(node->set_stmt.value, env);
            if (IS_ERROR(val)) {
                return val;
            }
            Object* result = env_assign(env, node->set_stmt.name->identifier.value, val);
            // 如果是错误，返回错误；否则返回赋值的值
            return IS_ERROR(result) ? result : val;
        }
        case NODE_EXPRESSION_STATEMENT:
            return eval(node->expr_stmt.expression, env);
        case NODE_NUMBER_LITERAL:
            return create_number(node->number_literal.value);
        case NODE_STRING_LITERAL:
            return create_string(node->string_literal.value);
        case NODE_BOOLEAN_LITERAL:
            return node->boolean_literal.value ? TRUE_OBJ : FALSE_OBJ;
        case NODE_IDENTIFIER: {
            Object* val = env_get(env, node->identifier.value);
            if (!val) {
                return create_error("identifier not found: %s", node->identifier.value);
            }
            return val;
        }
        case NODE_CALL_EXPRESSION: {
            Object* function = eval(node->call_expr.function, env);
            if (IS_ERROR(function)) {
                return function;
            }
            
            DynArray* args = da_create(sizeof(Object*));
            for (int i = 0; i < da_count(node->call_expr.arguments); i++) {
                ASTNode** arg_ptr = (ASTNode**)da_get(node->call_expr.arguments, i);
                if (arg_ptr) {
                    Object* evaluated = eval(*arg_ptr, env);
                    if (IS_ERROR(evaluated)) {
                        da_free(args);
                        return evaluated;
                    }
                    da_push(args, &evaluated);
                }
            }
            
            Object* result = apply_function(function, args);
            da_free(args);
            return result;
        }
        case NODE_IF_EXPRESSION: {
            Object* condition = eval(node->if_expr.condition, env);
            if (IS_ERROR(condition)) {
                return condition;
            }
            
            if (is_truthy(condition)) {
                return eval(node->if_expr.then_branch, env);
            } else if (node->if_expr.else_branch) {
                return eval(node->if_expr.else_branch, env);
            } else {
                return NULL_OBJ;
            }
        }
        case NODE_COND_EXPRESSION: {
            for (int i = 0; i < da_count(node->cond_expr.branches); i++) {
                ASTNode** branch_ptr = (ASTNode**)da_get(node->cond_expr.branches, i);
                if (branch_ptr && *branch_ptr) {
                    ASTNode* branch = *branch_ptr;
                    Object* condition = eval(branch->cond_branch.condition, env);
                    if (IS_ERROR(condition)) {
                        return condition;
                    }
                    
                    if (is_truthy(condition)) {
                        return eval(branch->cond_branch.expression, env);
                    }
                }
            }
            return NULL_OBJ;
        }
        case NODE_ARRAY_LITERAL: {
            if (da_count(node->array_literal.elements) > 0) {
                ASTNode** first_elem_ptr = (ASTNode**)da_get(node->array_literal.elements, 0);
                if (first_elem_ptr) {
                    return eval(*first_elem_ptr, env);
                }
            }
            return NULL_OBJ;
        }
        
        // === 循环语句执行逻辑 ===
        
        case NODE_WHILE_EXPRESSION: {
            Object* result = NULL_OBJ;
            
            while (true) {
                Object* condition = eval(node->while_expr.condition, env);
                if (IS_ERROR(condition)) {
                    return condition;
                }
                
                if (!is_truthy(condition)) {
                    break;
                }
                
                result = eval(node->while_expr.body, env);
                if (IS_ERROR(result)) {
                    return result;
                }
                
                // 处理循环控制语句
                if (IS_BREAK(result)) {
                    result = NULL_OBJ;  // break退出循环，返回null
                    break;
                }
                if (IS_CONTINUE(result)) {
                    result = NULL_OBJ;  // continue继续下一次迭代
                    continue;
                }
            }
            
            return result;
        }
        
        case NODE_FOR_EXPRESSION: {
            // 创建新的作用域用于for循环
            Environment* loop_env = create_enclosed_environment(env);
            Object* result = NULL_OBJ;
            
            // 执行初始化表达式
            Object* init_result = eval(node->for_expr.init, loop_env);
            if (IS_ERROR(init_result)) {
                free_environment(loop_env);
                return init_result;
            }
            
            while (true) {
                // 检查条件
                Object* condition = eval(node->for_expr.condition, loop_env);
                if (IS_ERROR(condition)) {
                    free_environment(loop_env);
                    return condition;
                }
                
                if (!is_truthy(condition)) {
                    break;
                }
                
                // 执行循环体
                result = eval(node->for_expr.body, loop_env);
                if (IS_ERROR(result)) {
                    free_environment(loop_env);
                    return result;
                }
                
                // 处理循环控制语句
                if (IS_BREAK(result)) {
                    result = NULL_OBJ;
                    break;
                }
                if (IS_CONTINUE(result)) {
                    result = NULL_OBJ;
                    // 继续执行步进语句
                }
                
                // 执行步进表达式
                Object* step_result = eval(node->for_expr.step, loop_env);
                if (IS_ERROR(step_result)) {
                    free_environment(loop_env);
                    return step_result;
                }
            }
            
            free_environment(loop_env);
            return result;
        }
        
        case NODE_FOREACH_EXPRESSION: {
            // FOREACH暂时简化实现 - 仅处理数组字面量
            Object* iterable = eval(node->foreach_expr.iterable, env);
            if (IS_ERROR(iterable)) {
                return iterable;
            }
            
            // 创建新的作用域
            Environment* loop_env = create_enclosed_environment(env);
            Object* result = NULL_OBJ;
            
            // 简化版本：如果是数组字面量，我们遍历其元素
            // 实际实现中需要更复杂的迭代器支持
            if (node->foreach_expr.iterable->type == NODE_ARRAY_LITERAL) {
                DynArray* elements = node->foreach_expr.iterable->array_literal.elements;
                
                for (int i = 0; i < da_count(elements); i++) {
                    ASTNode** elem_ptr = (ASTNode**)da_get(elements, i);
                    if (elem_ptr) {
                        Object* elem_value = eval(*elem_ptr, env);
                        if (IS_ERROR(elem_value)) {
                            free_environment(loop_env);
                            return elem_value;
                        }
                        
                        // 设置循环变量
                        env_define(loop_env, node->foreach_expr.var_name->identifier.value, elem_value, false);
                        
                        // 执行循环体
                        result = eval(node->foreach_expr.body, loop_env);
                        if (IS_ERROR(result)) {
                            free_environment(loop_env);
                            return result;
                        }
                        
                        // 处理循环控制语句
                        if (IS_BREAK(result)) {
                            result = NULL_OBJ;
                            break;
                        }
                        if (IS_CONTINUE(result)) {
                            result = NULL_OBJ;
                            continue;
                        }
                    }
                }
            } else {
                free_environment(loop_env);
                return create_error("FOREACH currently only supports array literals");
            }
            
            free_environment(loop_env);
            return result;
        }
        
        case NODE_BREAK_STATEMENT:
            return create_break();
            
        case NODE_CONTINUE_STATEMENT:
            return create_continue();
        
        default:
            return create_error("unknown node type: %d", node->type);
    }
}