#include "wl.h"

// --- 函数前向声明 ---
static Object* eval_node(ASTNode* node, Environment* env);
static Object* apply_function(Object* fn, DynArray* args);

// --- 主求值函数 ---
Object* eval(ASTNode* node, Environment* env) {
    return eval_node(node, env);
}

// --- 静态辅助函数 ---

static Object* eval_program(ASTNode* node, Environment* env) {
    Object* result = NULL_OBJ;
    for (int i = 0; i < da_count(node->program.statements); i++) {
        ASTNode* stmt = da_get(node->program.statements, i);
        result = eval_node(stmt, env);
        // 可以在这里处理 return 和 error
    }
    return result;
}

static Object* eval_node(ASTNode* node, Environment* env) {
    if (node == NULL) return NULL_OBJ;

    switch (node->type) {
        case PROGRAM:
            return eval_program(node, env);
        case EXPRESSION_STATEMENT:
            return eval_node(node->expr_stmt.expression, env);
        case NUMBER_LITERAL:
            return create_number(node->number_literal.value);
        case IDENTIFIER: {
            Object* val = env_get(env, node->identifier.value);
            if (!val) {
                return create_error("identifier not found: %s", node->identifier.value);
            }
            return val;
        }
        case CALL_EXPRESSION: {
            Object* fn_obj = eval_node(node->call_expr.function, env);
            if (IS_ERROR(fn_obj)) return fn_obj;

            DynArray* args = da_create(sizeof(Object*));
            for (int i = 0; i < da_count(node->call_expr.arguments); i++) {
                ASTNode* arg_node = da_get(node->call_expr.arguments, i);
                Object* evaluated_arg = eval_node(arg_node, env);
                if (IS_ERROR(evaluated_arg)) {
                    da_free(args);
                    return evaluated_arg;
                }
                da_push(args, &evaluated_arg);
            }
            
            Object* result = apply_function(fn_obj, args);
            da_free(args); // 释放参数数组本身
            return result;
        }
        default:
            return create_error("unknown AST node type: %d", node->type);
    }
}

static Object* apply_function(Object* fn, DynArray* args) {
    if (fn->type == OBJ_BUILTIN) {
        return ((BuiltinObject*)fn)->function(args);
    }
    // 未来在这里处理用户自定义函数 (OBJ_FUNCTION)
    
    return create_error("not a function: %s", object_type_to_str(fn->type));
}
