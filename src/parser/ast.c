#include "wl.h"

void free_ast_node(ASTNode *node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_PROGRAM:
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
        case NODE_VAR_STATEMENT:
            free_ast_node(node->var_stmt.name);
            free_ast_node(node->var_stmt.value);
            break;
        case NODE_SET_STATEMENT:
            free_ast_node(node->set_stmt.name);
            free_ast_node(node->set_stmt.value);
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
        case NODE_IF_EXPRESSION:
            free_ast_node(node->if_expr.condition);
            free_ast_node(node->if_expr.then_branch);
            if (node->if_expr.else_branch) {
                free_ast_node(node->if_expr.else_branch);
            }
            break;
        case NODE_COND_EXPRESSION:
            for (int i = 0; i < da_count(node->cond_expr.branches); i++) {
                ASTNode** branch_ptr = (ASTNode**)da_get(node->cond_expr.branches, i);
                if (branch_ptr) {
                    free_ast_node(*branch_ptr);
                }
            }
            da_free(node->cond_expr.branches);
            break;
        case NODE_COND_BRANCH:
            free_ast_node(node->cond_branch.condition);
            free_ast_node(node->cond_branch.expression);
            break;
        case NODE_ARRAY_LITERAL:
            for (int i = 0; i < da_count(node->array_literal.elements); i++) {
                ASTNode** elem_ptr = (ASTNode**)da_get(node->array_literal.elements, i);
                if (elem_ptr) {
                    free_ast_node(*elem_ptr);
                }
            }
            da_free(node->array_literal.elements);
            break;
        case NODE_NUMBER_LITERAL:
        case NODE_BOOLEAN_LITERAL:
            break;
    }
    free(node);
}