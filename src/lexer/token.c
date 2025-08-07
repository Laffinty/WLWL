#include "wl.h"
#include <string.h>

const char* token_type_to_str(TokenType type) {
    switch (type) {
        case TOKEN_ILLEGAL: return "ILLEGAL";
        case TOKEN_EOF: return "EOF";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_LET: return "LET";
        case TOKEN_VAR: return "VAR";
        case TOKEN_SET: return "SET";
        case TOKEN_FUN: return "FUN";
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";
        case TOKEN_IF: return "IF";
        case TOKEN_COND: return "COND";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_NULL: return "NULL";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_FOREACH: return "FOREACH";
        case TOKEN_BREAK: return "BREAK";
        case TOKEN_CONTINUE: return "CONTINUE";
        case TOKEN_LPAREN: return "(";
        case TOKEN_RPAREN: return ")";
        case TOKEN_LBRACE: return "{";
        case TOKEN_RBRACE: return "}";
        case TOKEN_LBRACKET: return "[";
        case TOKEN_RBRACKET: return "]";
        case TOKEN_COMMA: return ",";
        case TOKEN_SEMICOLON: return ";";
        default: return "UNKNOWN";
    }
}

Token create_token(TokenType type, const char* literal) {
    Token tok;
    tok.type = type;
    tok.literal = strdup(literal ? literal : "");
    return tok;
}

void free_token(Token* token) {
    if (token && token->literal) {
        free(token->literal);
        token->literal = NULL;
    }
}

TokenType lookup_identifier(const char* ident) {
    if (strcmp(ident, "LET") == 0) return TOKEN_LET;
    if (strcmp(ident, "VAR") == 0) return TOKEN_VAR;
    if (strcmp(ident, "SET") == 0) return TOKEN_SET;
    if (strcmp(ident, "FUN") == 0) return TOKEN_FUN;
    if (strcmp(ident, "TRUE") == 0) return TOKEN_TRUE;
    if (strcmp(ident, "FALSE") == 0) return TOKEN_FALSE;
    if (strcmp(ident, "IF") == 0) return TOKEN_IF;
    if (strcmp(ident, "COND") == 0) return TOKEN_COND;
    if (strcmp(ident, "RETURN") == 0) return TOKEN_RETURN;
    if (strcmp(ident, "NULL") == 0) return TOKEN_NULL;
    
    // 新增循环关键字识别
    if (strcmp(ident, "WHILE") == 0) return TOKEN_WHILE;
    if (strcmp(ident, "FOR") == 0) return TOKEN_FOR;
    if (strcmp(ident, "FOREACH") == 0) return TOKEN_FOREACH;
    if (strcmp(ident, "BREAK") == 0) return TOKEN_BREAK;
    if (strcmp(ident, "CONTINUE") == 0) return TOKEN_CONTINUE;
    
    return TOKEN_IDENTIFIER;
}