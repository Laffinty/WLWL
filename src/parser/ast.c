#include "wl.h"

void free_ast_node(ASTNode *node) {
    if (node == NULL) return;

    switch (node->type) {
        case PROGRAM:
            for (int i = 0; i < da_count(node->program.statements); i++) {
                free_ast_node(da_get(node->program.statements, i));
            }
            da_free(node->program.statements);
            break;
        case LET_STATEMENT:
            free_ast_node(node->let_stmt.name);
            free_ast_node(node->let_stmt.value);
            break;
        case EXPRESSION_STATEMENT:
            free_ast_node(node->expr_stmt.expression);
            break;
        case IDENTIFIER:
            free(node->identifier.value);
            break;
        case STRING_LITERAL:
            free(node->string_literal.value);
            break;
        case CALL_EXPRESSION:
            free_ast_node(node->call_expr.function);
            for (int i = 0; i < da_count(node->call_expr.arguments); i++) {
                free_ast_node(da_get(node->call_expr.arguments, i));
            }
            da_free(node->call_expr.arguments);
            break;
        case NUMBER_LITERAL:
        case BOOLEAN_LITERAL:
            // 无动态内存
            break;
    }
    free(node);
}
