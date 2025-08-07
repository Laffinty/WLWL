#include "wl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ASTNode* parse_statement(Parser* p);
static ASTNode* parse_let_statement(Parser* p);
static ASTNode* parse_var_statement(Parser* p);
static ASTNode* parse_set_statement(Parser* p);
static ASTNode* parse_expression_statement(Parser* p);
static ASTNode* parse_expression(Parser* p);
static ASTNode* parse_identifier(Parser* p);
static ASTNode* parse_number_literal(Parser* p);
static ASTNode* parse_string_literal(Parser* p);
static ASTNode* parse_boolean_literal(Parser* p);
static ASTNode* parse_call_expression(Parser* p, ASTNode* function);
static DynArray* parse_expression_list(Parser* p, TokenType end_token);

static ASTNode* parse_if_expression(Parser* p);
static ASTNode* parse_cond_expression(Parser* p);
static ASTNode* parse_array_literal(Parser* p);

// 新增循环解析函数声明
static ASTNode* parse_while_expression(Parser* p);
static ASTNode* parse_for_expression(Parser* p);
static ASTNode* parse_foreach_expression(Parser* p);
static ASTNode* parse_break_statement(Parser* p);
static ASTNode* parse_continue_statement(Parser* p);

static void parser_error(Parser* p, const char* msg) {
    char* error_msg = strdup(msg);
    da_push(&p->errors, &error_msg);
}

static bool current_token_is(Parser* p, TokenType expected) {
    return p->current_token.type == expected;
}

static bool peek_token_is(Parser* p, TokenType expected) {
    return p->peek_token.type == expected;
}

static bool expect_peek(Parser* p, TokenType expected) {
    if (peek_token_is(p, expected)) {
        next_token(p);
        return true;
    } else {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), 
                "expected next token to be %s, got %s instead",
                token_type_to_str(expected), 
                token_type_to_str(p->peek_token.type));
        parser_error(p, error_msg);
        return false;
    }
}

void next_token(Parser* p) {
    free_token(&p->current_token);
    
    p->current_token = p->peek_token;
    p->peek_token = lexer_next_token(p->l);
}

Parser create_parser(Lexer* l) {
    Parser p;
    p.l = l;
    da_init(&p.errors, sizeof(char*));

    p.current_token.literal = NULL;
    p.peek_token.literal = NULL;

    next_token(&p);
    next_token(&p);
    
    return p;
}

void free_parser(Parser* p) {
    free_token(&p->current_token);
    free_token(&p->peek_token);
    
    for (int i = 0; i < da_count(&p->errors); i++) {
        char** error_ptr = (char**)da_get(&p->errors, i);
        if (error_ptr) {
            free(*error_ptr);
        }
    }
    da_clear(&p->errors);
}

ASTNode* parse_program(Parser* p) {
    ASTNode* program = (ASTNode*)malloc(sizeof(ASTNode));
    program->type = NODE_PROGRAM;
    program->program.statements = da_create(sizeof(ASTNode*));

    while (!current_token_is(p, TOKEN_EOF)) {
        ASTNode* stmt = parse_statement(p);
        if (stmt != NULL) {
            da_push(program->program.statements, &stmt);
        }
        next_token(p);
    }
    return program;
}

static ASTNode* parse_statement(Parser* p) {
    switch (p->current_token.type) {
        case TOKEN_LET:
            return parse_let_statement(p);
        case TOKEN_VAR:
            return parse_var_statement(p);
        case TOKEN_SET:
            return parse_set_statement(p);
        case TOKEN_BREAK:
            return parse_break_statement(p);
        case TOKEN_CONTINUE:
            return parse_continue_statement(p);
        default:
            return parse_expression_statement(p);
    }
}

static ASTNode* parse_let_statement(Parser* p) {
    ASTNode* stmt = (ASTNode*)malloc(sizeof(ASTNode));
    stmt->type = NODE_LET_STATEMENT;
    
    if (!expect_peek(p, TOKEN_LPAREN)) {
        free(stmt);
        return NULL;
    }
    
    if (!expect_peek(p, TOKEN_IDENTIFIER)) {
        free(stmt);
        return NULL;
    }
    
    stmt->let_stmt.name = parse_identifier(p);
    
    if (!expect_peek(p, TOKEN_COMMA)) {
        free_ast_node(stmt->let_stmt.name);
        free(stmt);
        return NULL;
    }
    
    next_token(p);
    stmt->let_stmt.value = parse_expression(p);
    
    if (!expect_peek(p, TOKEN_RPAREN)) {
        free_ast_node(stmt);
        return NULL;
    }
    
    if (peek_token_is(p, TOKEN_SEMICOLON)) {
        next_token(p);
    }
    
    return stmt;
}

static ASTNode* parse_var_statement(Parser* p) {
    ASTNode* stmt = (ASTNode*)malloc(sizeof(ASTNode));
    stmt->type = NODE_VAR_STATEMENT;
    
    if (!expect_peek(p, TOKEN_LPAREN)) {
        free(stmt);
        return NULL;
    }
    
    if (!expect_peek(p, TOKEN_IDENTIFIER)) {
        free(stmt);
        return NULL;
    }
    
    stmt->var_stmt.name = parse_identifier(p);
    
    if (!expect_peek(p, TOKEN_COMMA)) {
        free_ast_node(stmt->var_stmt.name);
        free(stmt);
        return NULL;
    }
    
    next_token(p);
    stmt->var_stmt.value = parse_expression(p);
    
    if (!expect_peek(p, TOKEN_RPAREN)) {
        free_ast_node(stmt);
        return NULL;
    }
    
    if (peek_token_is(p, TOKEN_SEMICOLON)) {
        next_token(p);
    }
    
    return stmt;
}

static ASTNode* parse_set_statement(Parser* p) {
    ASTNode* stmt = (ASTNode*)malloc(sizeof(ASTNode));
    stmt->type = NODE_SET_STATEMENT;
    
    if (!expect_peek(p, TOKEN_LPAREN)) {
        free(stmt);
        return NULL;
    }
    
    if (!expect_peek(p, TOKEN_IDENTIFIER)) {
        free(stmt);
        return NULL;
    }
    
    stmt->set_stmt.name = parse_identifier(p);
    
    if (!expect_peek(p, TOKEN_COMMA)) {
        free_ast_node(stmt->set_stmt.name);
        free(stmt);
        return NULL;
    }
    
    next_token(p);
    stmt->set_stmt.value = parse_expression(p);
    
    if (!expect_peek(p, TOKEN_RPAREN)) {
        free_ast_node(stmt);
        return NULL;
    }
    
    if (peek_token_is(p, TOKEN_SEMICOLON)) {
        next_token(p);
    }
    
    return stmt;
}

static ASTNode* parse_expression_statement(Parser* p) {
    ASTNode* stmt = (ASTNode*)malloc(sizeof(ASTNode));
    stmt->type = NODE_EXPRESSION_STATEMENT;
    stmt->expr_stmt.expression = parse_expression(p);
    
    if (peek_token_is(p, TOKEN_SEMICOLON)) {
        next_token(p);
    }
    
    return stmt;
}

// 新增解析语句块的函数声明
static ASTNode* parse_block_expression(Parser* p);

static ASTNode* parse_expression(Parser* p) {
    ASTNode* left = NULL;
    
    switch (p->current_token.type) {
        case TOKEN_IDENTIFIER:
            left = parse_identifier(p);
            break;
        case TOKEN_NUMBER:
            left = parse_number_literal(p);
            break;
        case TOKEN_STRING:
            left = parse_string_literal(p);
            break;
        case TOKEN_TRUE:
        case TOKEN_FALSE:
            left = parse_boolean_literal(p);
            break;
        case TOKEN_NULL:
            left = (ASTNode*)malloc(sizeof(ASTNode));
            left->type = NODE_IDENTIFIER;
            left->identifier.value = strdup("NULL");
            break;
        case TOKEN_IF:
            left = parse_if_expression(p);
            break;
        case TOKEN_COND:
            left = parse_cond_expression(p);
            break;
        case TOKEN_WHILE:
            left = parse_while_expression(p);
            break;
        case TOKEN_FOR:
            left = parse_for_expression(p);
            break;
        case TOKEN_FOREACH:
            left = parse_foreach_expression(p);
            break;
        case TOKEN_LBRACKET:
            left = parse_array_literal(p);
            break;
        case TOKEN_LPAREN:
            left = parse_block_expression(p);
            break;
        // 允许在表达式上下文中使用语句
        case TOKEN_VAR:
            left = parse_var_statement(p);
            break;
        case TOKEN_LET:
            left = parse_let_statement(p);
            break;
        case TOKEN_SET:
            left = parse_set_statement(p);
            break;
        case TOKEN_BREAK:
            left = parse_break_statement(p);
            break;
        case TOKEN_CONTINUE:
            left = parse_continue_statement(p);
            break;
        default:
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), 
                    "no prefix parse function for %s found", 
                    token_type_to_str(p->current_token.type));
            parser_error(p, error_msg);
            return NULL;
    }
    
    if (left && left->type == NODE_IDENTIFIER && peek_token_is(p, TOKEN_LPAREN)) {
        left = parse_call_expression(p, left);
    }
    
    return left;
}

static ASTNode* parse_identifier(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_IDENTIFIER;
    node->identifier.value = strdup(p->current_token.literal);
    return node;
}

static ASTNode* parse_number_literal(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_NUMBER_LITERAL;
    node->number_literal.value = atof(p->current_token.literal);
    return node;
}

static ASTNode* parse_string_literal(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_STRING_LITERAL;
    node->string_literal.value = strdup(p->current_token.literal);
    return node;
}

static ASTNode* parse_boolean_literal(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_BOOLEAN_LITERAL;
    node->boolean_literal.value = (strcmp(p->current_token.literal, "TRUE") == 0);
    return node;
}

static ASTNode* parse_call_expression(Parser* p, ASTNode* function) {
    ASTNode* call = (ASTNode*)malloc(sizeof(ASTNode));
    call->type = NODE_CALL_EXPRESSION;
    call->call_expr.function = function;
    call->call_expr.arguments = parse_expression_list(p, TOKEN_RPAREN);
    return call;
}

static DynArray* parse_expression_list(Parser* p, TokenType end_token) {
    DynArray* args = da_create(sizeof(ASTNode*));
    
    if (!expect_peek(p, TOKEN_LPAREN)) {
        da_free(args);
        return NULL;
    }
    
    if (peek_token_is(p, end_token)) {
        next_token(p);
        return args;
    }
    
    next_token(p);
    ASTNode* arg = parse_expression(p);
    if (arg) {
        da_push(args, &arg);
    }
    
    while (peek_token_is(p, TOKEN_COMMA)) {
        next_token(p);
        next_token(p);
        arg = parse_expression(p);
        if (arg) {
            da_push(args, &arg);
        }
    }
    
    if (!expect_peek(p, end_token)) {
        da_free(args);
        return NULL;
    }
    
    return args;
}

static ASTNode* parse_if_expression(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_IF_EXPRESSION;
    
    if (!expect_peek(p, TOKEN_LPAREN)) {
        free(node);
        return NULL;
    }
    
    next_token(p);
    node->if_expr.condition = parse_expression(p);
    if (!node->if_expr.condition) {
        free(node);
        return NULL;
    }
    
    if (!expect_peek(p, TOKEN_COMMA)) {
        free_ast_node(node->if_expr.condition);
        free(node);
        return NULL;
    }
    
    next_token(p);
    node->if_expr.then_branch = parse_expression(p);
    if (!node->if_expr.then_branch) {
        free_ast_node(node->if_expr.condition);
        free(node);
        return NULL;
    }
    
    node->if_expr.else_branch = NULL;
    if (peek_token_is(p, TOKEN_COMMA)) {
        next_token(p);
        next_token(p);
        node->if_expr.else_branch = parse_expression(p);
    }
    
    if (!expect_peek(p, TOKEN_RPAREN)) {
        free_ast_node(node);
        return NULL;
    }
    
    return node;
}

static ASTNode* parse_cond_expression(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_COND_EXPRESSION;
    node->cond_expr.branches = da_create(sizeof(ASTNode*));
    
    if (!expect_peek(p, TOKEN_LPAREN)) {
        da_free(node->cond_expr.branches);
        free(node);
        return NULL;
    }
    
    // 解析 COND 分支
    while (!current_token_is(p, TOKEN_RPAREN) && !current_token_is(p, TOKEN_EOF)) {
        // 期望左括号 (
        if (!expect_peek(p, TOKEN_LPAREN)) {
            break;
        }
        
        ASTNode* branch = (ASTNode*)malloc(sizeof(ASTNode));
        branch->type = NODE_COND_BRANCH;
        
        // 解析条件
        next_token(p);
        branch->cond_branch.condition = parse_expression(p);
        if (!branch->cond_branch.condition) {
            free(branch);
            break;
        }
        
        // 期望逗号
        if (!expect_peek(p, TOKEN_COMMA)) {
            free_ast_node(branch->cond_branch.condition);
            free(branch);
            break;
        }
        
        // 解析值表达式
        next_token(p);
        branch->cond_branch.expression = parse_expression(p);
        if (!branch->cond_branch.expression) {
            free_ast_node(branch->cond_branch.condition);
            free(branch);
            break;
        }
        
        // 期望右括号 )
        if (!expect_peek(p, TOKEN_RPAREN)) {
            free_ast_node(branch);
            break;
        }
        
        da_push(node->cond_expr.branches, &branch);
        
        // 检查是否有更多分支
        if (peek_token_is(p, TOKEN_COMMA)) {
            next_token(p);
        } else if (peek_token_is(p, TOKEN_RPAREN)) {
            break;
        }
    }
    
    if (!expect_peek(p, TOKEN_RPAREN)) {
        free_ast_node(node);
        return NULL;
    }
    
    return node;
}

static ASTNode* parse_array_literal(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_ARRAY_LITERAL;
    node->array_literal.elements = da_create(sizeof(ASTNode*));
    
    // 当前 token 是 '[', 检查下一个 token
    if (peek_token_is(p, TOKEN_RBRACKET)) {
        next_token(p);  // 移动到 ']'
        return node;  // 返回空数组
    }
    
    // 解析第一个元素
    next_token(p);  // 移动到第一个元素
    ASTNode* elem = parse_expression(p);
    if (elem) {
        da_push(node->array_literal.elements, &elem);
    }
    
    // 解析剩余元素
    while (peek_token_is(p, TOKEN_COMMA)) {
        next_token(p);  // 跳过逗号
        next_token(p);  // 移动到下一个元素
        elem = parse_expression(p);
        if (elem) {
            da_push(node->array_literal.elements, &elem);
        }
    }
    
    // 期望右括号 ']'
    if (!expect_peek(p, TOKEN_RBRACKET)) {
        free_ast_node(node);
        return NULL;
    }
    
    return node;
}

// === 新增循环解析函数实现 ===

static ASTNode* parse_while_expression(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_WHILE_EXPRESSION;
    
    // 解析 WHILE(condition, body)
    if (!expect_peek(p, TOKEN_LPAREN)) {
        free(node);
        return NULL;
    }
    
    next_token(p);
    node->while_expr.condition = parse_expression(p);
    if (!node->while_expr.condition) {
        free(node);
        return NULL;
    }
    
    if (!expect_peek(p, TOKEN_COMMA)) {
        free_ast_node(node->while_expr.condition);
        free(node);
        return NULL;
    }
    
    next_token(p);
    node->while_expr.body = parse_expression(p);
    if (!node->while_expr.body) {
        free_ast_node(node->while_expr.condition);
        free(node);
        return NULL;
    }
    
    if (!expect_peek(p, TOKEN_RPAREN)) {
        free_ast_node(node);
        return NULL;
    }
    
    return node;
}

static ASTNode* parse_for_expression(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_FOR_EXPRESSION;
    
    // 解析 FOR(init, condition, step, body)
    if (!expect_peek(p, TOKEN_LPAREN)) {
        free(node);
        return NULL;
    }
    
    // 解析初始化表达式
    next_token(p);
    node->for_expr.init = parse_expression(p);
    if (!node->for_expr.init) {
        free(node);
        return NULL;
    }
    
    if (!expect_peek(p, TOKEN_COMMA)) {
        free_ast_node(node->for_expr.init);
        free(node);
        return NULL;
    }
    
    // 解析条件表达式
    next_token(p);
    node->for_expr.condition = parse_expression(p);
    if (!node->for_expr.condition) {
        free_ast_node(node->for_expr.init);
        free(node);
        return NULL;
    }
    
    if (!expect_peek(p, TOKEN_COMMA)) {
        free_ast_node(node->for_expr.init);
        free_ast_node(node->for_expr.condition);
        free(node);
        return NULL;
    }
    
    // 解析步进表达式
    next_token(p);
    node->for_expr.step = parse_expression(p);
    if (!node->for_expr.step) {
        free_ast_node(node->for_expr.init);
        free_ast_node(node->for_expr.condition);
        free(node);
        return NULL;
    }
    
    if (!expect_peek(p, TOKEN_COMMA)) {
        free_ast_node(node->for_expr.init);
        free_ast_node(node->for_expr.condition);
        free_ast_node(node->for_expr.step);
        free(node);
        return NULL;
    }
    
    // 解析循环体
    next_token(p);
    node->for_expr.body = parse_expression(p);
    if (!node->for_expr.body) {
        free_ast_node(node->for_expr.init);
        free_ast_node(node->for_expr.condition);
        free_ast_node(node->for_expr.step);
        free(node);
        return NULL;
    }
    
    if (!expect_peek(p, TOKEN_RPAREN)) {
        free_ast_node(node);
        return NULL;
    }
    
    return node;
}

static ASTNode* parse_foreach_expression(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_FOREACH_EXPRESSION;
    
    // 解析 FOREACH(var_name, iterable, body)
    if (!expect_peek(p, TOKEN_LPAREN)) {
        free(node);
        return NULL;
    }
    
    // 解析循环变量名
    if (!expect_peek(p, TOKEN_IDENTIFIER)) {
        free(node);
        return NULL;
    }
    
    node->foreach_expr.var_name = parse_identifier(p);
    
    if (!expect_peek(p, TOKEN_COMMA)) {
        free_ast_node(node->foreach_expr.var_name);
        free(node);
        return NULL;
    }
    
    // 解析可迭代对象
    next_token(p);
    node->foreach_expr.iterable = parse_expression(p);
    if (!node->foreach_expr.iterable) {
        free_ast_node(node->foreach_expr.var_name);
        free(node);
        return NULL;
    }
    
    if (!expect_peek(p, TOKEN_COMMA)) {
        free_ast_node(node->foreach_expr.var_name);
        free_ast_node(node->foreach_expr.iterable);
        free(node);
        return NULL;
    }
    
    // 解析循环体
    next_token(p);
    node->foreach_expr.body = parse_expression(p);
    if (!node->foreach_expr.body) {
        free_ast_node(node->foreach_expr.var_name);
        free_ast_node(node->foreach_expr.iterable);
        free(node);
        return NULL;
    }
    
    if (!expect_peek(p, TOKEN_RPAREN)) {
        free_ast_node(node);
        return NULL;
    }
    
    return node;
}

static ASTNode* parse_break_statement(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_BREAK_STATEMENT;
    
    if (peek_token_is(p, TOKEN_SEMICOLON)) {
        next_token(p);
    }
    
    return node;
}

static ASTNode* parse_block_expression(Parser* p) {
    // 解析语句块：(stmt1; stmt2; ...)
    ASTNode* block = (ASTNode*)malloc(sizeof(ASTNode));
    block->type = NODE_BLOCK_EXPRESSION;
    block->block_expr.statements = da_create(sizeof(ASTNode*));
    
    // 当前token已经是左括号，直接跳过
    next_token(p);
    
    // 如果直接遇到右括号，返回空块
    if (current_token_is(p, TOKEN_RPAREN)) {
        return block;
    }
    
    // 解析语句块中的语句
    while (!current_token_is(p, TOKEN_RPAREN) && !current_token_is(p, TOKEN_EOF)) {
        ASTNode* stmt = parse_statement(p);
        if (stmt != NULL) {
            da_push(block->block_expr.statements, &stmt);
        }
        
        next_token(p);
        
        // 跳过分号
        if (current_token_is(p, TOKEN_SEMICOLON)) {
            next_token(p);
        }
    }
    
    return block;
}

static ASTNode* parse_continue_statement(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_CONTINUE_STATEMENT;
    
    if (peek_token_is(p, TOKEN_SEMICOLON)) {
        next_token(p);
    }
    
    return node;
}