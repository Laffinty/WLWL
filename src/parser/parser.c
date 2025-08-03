#include "wl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations for static functions
static ASTNode* parse_statement(Parser* p);
static ASTNode* parse_expression_statement(Parser* p);
static ASTNode* parse_expression(Parser* p, int precedence);
static ASTNode* parse_identifier(Parser* p);
static ASTNode* parse_number_literal(Parser* p);
static ASTNode* parse_prefix_expression(Parser* p);
static ASTNode* parse_infix_expression(Parser* p, ASTNode* left);
static ASTNode* parse_call_expression(Parser* p, ASTNode* function);
static DynArray* parse_expression_list(Parser* p, TokenType end_token);


// Helper to record a parsing error.
static void parser_error(Parser* p, const char* msg) {
    char* error_msg = strdup(msg);
    da_push(&p->errors, &error_msg);
}

// Advances the tokens.
void next_token(Parser* p) {
    p->current_token = p->peek_token;
    p->peek_token = next_token_from_lexer(p->l);
}

// Creates and initializes a new parser.
Parser create_parser(Lexer* l) {
    Parser p;
    p.l = l;
    da_init(&p.errors, sizeof(char*)); // Use da_init for struct member

    // Read two tokens, so current_token and peek_token are both set
    next_token(&p);
    next_token(&p);
    
    return p;
}

// Frees the parser and its error messages.
void free_parser(Parser* p) {
    for (int i = 0; i < da_count(&p->errors); i++) {
        free(*(char**)da_get(&p->errors, i));
    }
    da_clear(&p->errors);
}

// Main parsing function.
ASTNode* parse_program(Parser* p) {
    ASTNode* program = (ASTNode*)malloc(sizeof(ASTNode));
    program->type = NODE_PROGRAM;
    program->program.statements = da_create(sizeof(ASTNode*));

    while (p->current_token.type != TOKEN_EOF) {
        ASTNode* stmt = parse_statement(p);
        if (stmt != NULL) {
            da_push(program->program.statements, &stmt);
        }
        next_token(p);
    }
    return program;
}

// --- Other parsing functions would go here ---
// (parse_statement, parse_expression, etc. are needed for a full implementation)
// For now, providing stubs to ensure it compiles.

static ASTNode* parse_statement(Parser* p) {
    // Stub implementation
    parser_error(p, "parse_statement is not implemented");
    return NULL;
}

static ASTNode* parse_expression_statement(Parser* p) {
    // Stub implementation
    return NULL;
}
static ASTNode* parse_expression(Parser* p, int precedence) {
    // Stub implementation
    return NULL;
}
static ASTNode* parse_identifier(Parser* p) {
    // Stub implementation
    return NULL;
}
static ASTNode* parse_number_literal(Parser* p) {
    // Stub implementation
    return NULL;
}
static ASTNode* parse_prefix_expression(Parser* p) {
    // Stub implementation
    return NULL;
}
static ASTNode* parse_infix_expression(Parser* p, ASTNode* left) {
    // Stub implementation
    return NULL;
}
static ASTNode* parse_call_expression(Parser* p, ASTNode* function) {
    // Stub implementation
    return NULL;
}
static DynArray* parse_expression_list(Parser* p, TokenType end_token) {
    // Stub implementation
    return NULL;
}
