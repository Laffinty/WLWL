#include "wl.h"

void free_ast_node(ASTNode *node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_PROGRAM:  // 修正：使用 NODE_ 前缀
            for (int i = 0; i < da_count(node->program.statements); i++) {
                ASTNode** stmt_ptr = (ASTNode**)da_get(node->program.statements, i);
                if (stmt_ptr) {
                    free_ast_node(*stmt_ptr);
                }
            }
            da_free(node->program.statements);
            break;
        case NODE_LET_STATEMENT:
            free_ast_node(node->let_stmt.name);
            free_ast_node(node->let_stmt.value);
            break;
        case NODE_EXPRESSION_STATEMENT:
            free_ast_node(node->expr_stmt.expression);
            break;
        case NODE_IDENTIFIER:
            free(node->identifier.value);
            break;
        case NODE_STRING_LITERAL:
            free(node->string_literal.value);
            break;
        case NODE_CALL_EXPRESSION:
            free_ast_node(node->call_expr.function);
            for (int i = 0; i < da_count(node->call_expr.arguments); i++) {
                ASTNode** arg_ptr = (ASTNode**)da_get(node->call_expr.arguments, i);
                if (arg_ptr) {
                    free_ast_node(*arg_ptr);
                }
            }
            da_free(node->call_expr.arguments);
            break;
        case NODE_NUMBER_LITERAL:
        case NODE_BOOLEAN_LITERAL:
            // 无动态内存
            break;
    }
    free(node);
}