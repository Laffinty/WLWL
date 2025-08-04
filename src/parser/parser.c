#include "wl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations for static functions
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

// Helper to record a parsing error.
static void parser_error(Parser* p, const char* msg) {
    char* error_msg = strdup(msg);
    da_push(&p->errors, &error_msg);
}

// Check if current token matches expected type
static bool current_token_is(Parser* p, TokenType expected) {
    return p->current_token.type == expected;
}

// Check if peek token matches expected type
static bool peek_token_is(Parser* p, TokenType expected) {
    return p->peek_token.type == expected;
}

// Expect and consume a token of a specific type
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

// Advances the tokens - 修复：添加Token内存管理
void next_token(Parser* p) {
    // 释放当前token的内存
    free_token(&p->current_token);
    
    // 移动token
    p->current_token = p->peek_token;
    p->peek_token = lexer_next_token(p->l);
}

// Creates and initializes a new parser.
Parser create_parser(Lexer* l) {
    Parser p;
    p.l = l;
    da_init(&p.errors, sizeof(char*));

    // 初始化token为空，避免释放未初始化的内存
    p.current_token.literal = NULL;
    p.peek_token.literal = NULL;

    // Read two tokens, so current_token and peek_token are both set
    next_token(&p);
    next_token(&p);
    
    return p;
}

// Frees the parser and its error messages.
void free_parser(Parser* p) {
    // 释放token内存
    free_token(&p->current_token);
    free_token(&p->peek_token);
    
    // 释放错误消息
    for (int i = 0; i < da_count(&p->errors); i++) {
        char** error_ptr = (char**)da_get(&p->errors, i);
        if (error_ptr) {
            free(*error_ptr);
        }
    }
    da_clear(&p->errors);
}

// Main parsing function.
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

// Parse a statement
static ASTNode* parse_statement(Parser* p) {
    switch (p->current_token.type) {
        case TOKEN_LET:
            return parse_let_statement(p);
        case TOKEN_VAR:
            return parse_var_statement(p);
        case TOKEN_SET:
            return parse_set_statement(p);
        default:
            return parse_expression_statement(p);
    }
}

// Parse LET statement: LET(name, value);
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
    
    // Consume semicolon if present
    if (peek_token_is(p, TOKEN_SEMICOLON)) {
        next_token(p);
    }
    
    return stmt;
}

// Parse VAR statement: VAR(name, value);
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
    
    // Consume semicolon if present
    if (peek_token_is(p, TOKEN_SEMICOLON)) {
        next_token(p);
    }
    
    return stmt;
}

// Parse SET statement: SET(name, value);
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
    
    // Consume semicolon if present
    if (peek_token_is(p, TOKEN_SEMICOLON)) {
        next_token(p);
    }
    
    return stmt;
}

// Parse an expression statement
static ASTNode* parse_expression_statement(Parser* p) {
    ASTNode* stmt = (ASTNode*)malloc(sizeof(ASTNode));
    stmt->type = NODE_EXPRESSION_STATEMENT;
    stmt->expr_stmt.expression = parse_expression(p);
    
    // Consume semicolon if present
    if (peek_token_is(p, TOKEN_SEMICOLON)) {
        next_token(p);
    }
    
    return stmt;
}

// Parse an expression
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
        default:
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), 
                    "no prefix parse function for %s found", 
                    token_type_to_str(p->current_token.type));
            parser_error(p, error_msg);
            return NULL;
    }
    
    // Check for call expression: identifier followed by '('
    if (left && left->type == NODE_IDENTIFIER && peek_token_is(p, TOKEN_LPAREN)) {
        left = parse_call_expression(p, left);
    }
    
    return left;
}

// Parse an identifier
static ASTNode* parse_identifier(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_IDENTIFIER;
    node->identifier.value = strdup(p->current_token.literal);
    return node;
}

// Parse a number literal
static ASTNode* parse_number_literal(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_NUMBER_LITERAL;
    node->number_literal.value = atof(p->current_token.literal);
    return node;
}

// Parse a string literal
static ASTNode* parse_string_literal(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_STRING_LITERAL;
    node->string_literal.value = strdup(p->current_token.literal);
    return node;
}

// Parse a boolean literal
static ASTNode* parse_boolean_literal(Parser* p) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_BOOLEAN_LITERAL;
    node->boolean_literal.value = (strcmp(p->current_token.literal, "TRUE") == 0);
    return node;
}

// Parse a call expression: func(arg1, arg2, ...)
static ASTNode* parse_call_expression(Parser* p, ASTNode* function) {
    ASTNode* call = (ASTNode*)malloc(sizeof(ASTNode));
    call->type = NODE_CALL_EXPRESSION;
    call->call_expr.function = function;
    call->call_expr.arguments = parse_expression_list(p, TOKEN_RPAREN);
    return call;
}

// Parse expression list: (expr1, expr2, ...)
static DynArray* parse_expression_list(Parser* p, TokenType end_token) {
    DynArray* args = da_create(sizeof(ASTNode*));
    
    if (!expect_peek(p, TOKEN_LPAREN)) {
        da_free(args);
        return NULL;
    }
    
    if (peek_token_is(p, end_token)) {
        next_token(p);
        return args; // Empty argument list
    }
    
    next_token(p);
    ASTNode* arg = parse_expression(p);
    if (arg) {
        da_push(args, &arg);
    }
    
    while (peek_token_is(p, TOKEN_COMMA)) {
        next_token(p); // consume comma
        next_token(p); // move to next expression
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