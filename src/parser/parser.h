#ifndef WL_PARSER_H
#define WL_PARSER_H

#include "../lexer/lexer.h"
#include "../parser/ast.h"
#include "../core/dyn_array.h"

typedef struct Parser {
    Lexer* l;
    Token current_token;
    Token peek_token;
    DynArray errors;
} Parser;

Parser create_parser(Lexer* l);
void free_parser(Parser* p);
void next_token(Parser* p);
ASTNode* parse_program(Parser* p);

#endif