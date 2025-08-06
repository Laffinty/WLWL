#ifndef AST_H
#define AST_H

#include "../core/dyn_array.h"
#include "../lexer/token.h"

typedef enum {
    NODE_PROGRAM,
    NODE_LET_STATEMENT,
    NODE_VAR_STATEMENT,
    NODE_SET_STATEMENT,
    NODE_EXPRESSION_STATEMENT,
    NODE_IDENTIFIER,
    NODE_NUMBER_LITERAL,
    NODE_STRING_LITERAL,
    NODE_BOOLEAN_LITERAL,
    NODE_CALL_EXPRESSION,
    
    NODE_IF_EXPRESSION,
    NODE_COND_EXPRESSION,
    NODE_COND_BRANCH,
    
    NODE_ARRAY_LITERAL,
} ASTNodeType;

struct ASTNode;

typedef struct {
    DynArray* statements;
} ProgramNode;

typedef struct {
    struct ASTNode* name;
    struct ASTNode* value;
} LetStatementNode;

typedef struct {
    struct ASTNode* name;
    struct ASTNode* value;
} VarStatementNode;

typedef struct {
    struct ASTNode* name;
    struct ASTNode* value;
} SetStatementNode;

typedef struct {
    struct ASTNode* expression;
} ExpressionStatementNode;

typedef struct {
    char* value;
} IdentifierNode;

typedef struct {
    double value;
} NumberLiteralNode;

typedef struct {
    char* value;
} StringLiteralNode;

typedef struct {
    bool value;
} BooleanLiteralNode;

typedef struct {
    struct ASTNode* function;
    DynArray* arguments;
} CallExpressionNode;

typedef struct {
    struct ASTNode* condition;
    struct ASTNode* then_branch;
    struct ASTNode* else_branch;
} IfExpressionNode;

typedef struct {
    DynArray* branches;
} CondExpressionNode;

typedef struct {
    struct ASTNode* condition;
    struct ASTNode* expression;
} CondBranchNode;

typedef struct {
    DynArray* elements;
} ArrayLiteralNode;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        ProgramNode             program;
        LetStatementNode        let_stmt;
        VarStatementNode        var_stmt;
        SetStatementNode        set_stmt;
        ExpressionStatementNode expr_stmt;
        IdentifierNode          identifier;
        NumberLiteralNode       number_literal;
        StringLiteralNode       string_literal;
        BooleanLiteralNode      boolean_literal;
        CallExpressionNode      call_expr;
        IfExpressionNode        if_expr;
        CondExpressionNode      cond_expr;
        CondBranchNode          cond_branch;
        ArrayLiteralNode        array_literal;
    };
} ASTNode;

void free_ast_node(ASTNode* node);

#endif