#ifndef WL_PARSER_H
#define WL_PARSER_H

#include "lexer/lexer.h"
#include "parser/ast.h"
#include "core/dyn_array.h"

// The Parser struct now holds the lexer and a DynArray for errors.
// The errors array is now a struct member, not a pointer, simplifying memory management.
typedef struct Parser {
    Lexer* l;
    Token current_token;
    Token peek_token;
    DynArray errors; // DynArray of (char*)
} Parser;

// Function prototypes for the parser
Parser create_parser(Lexer* l);
void free_parser(Parser* p);
void next_token(Parser* p);
ASTNode* parse_program(Parser* p);

#endif // WL_PARSER_H
