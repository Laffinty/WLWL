#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_ILLEGAL,
    TOKEN_EOF,

    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,

    TOKEN_LET,
    TOKEN_VAR,
    TOKEN_SET,
    TOKEN_FUN,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_IF,
    TOKEN_COND,
    TOKEN_RETURN,
    TOKEN_NULL,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,

} TokenType;

typedef struct {
    TokenType type;
    char* literal;
} Token;

const char* token_type_to_str(TokenType type);
Token create_token(TokenType type, const char* literal);
void free_token(Token* token);
TokenType lookup_identifier(const char* ident);

#endif