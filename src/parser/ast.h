#ifndef AST_H
#define AST_H

#include "../core/dyn_array.h"
#include "../lexer/token.h"

// --- AST 节点类型枚举 ---
typedef enum {
    NODE_PROGRAM,
    NODE_LET_STATEMENT,       // LET(name, value);
    NODE_VAR_STATEMENT,       // VAR(name, value);
    NODE_SET_STATEMENT,       // SET(name, value);
    NODE_EXPRESSION_STATEMENT,
    NODE_IDENTIFIER,
    NODE_NUMBER_LITERAL,
    NODE_STRING_LITERAL,
    NODE_BOOLEAN_LITERAL,
    NODE_CALL_EXPRESSION,
} ASTNodeType;

// --- AST 节点具体结构定义 ---
struct ASTNode;

typedef struct {
    DynArray* statements; // DynArray of struct ASTNode*
} ProgramNode;

typedef struct {
    struct ASTNode* name; // Identifier
    struct ASTNode* value;
} LetStatementNode;

typedef struct {
    struct ASTNode* name; // Identifier
    struct ASTNode* value;
} VarStatementNode;

typedef struct {
    struct ASTNode* name; // Identifier
    struct ASTNode* value;
} SetStatementNode;

typedef struct {
    struct ASTNode* expression;
} ExpressionStatementNode;

typedef struct {
    char* value; // 例如 "x", "+", "my_function"
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

// 适用于所有前缀调用, e.g., add(2, 3), LET(x, 5)
typedef struct {
    struct ASTNode* function; // Identifier
    DynArray* arguments;      // DynArray of struct ASTNode*
} CallExpressionNode;

// --- ASTNode 核心结构 (使用 union) ---
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
    };
} ASTNode;

// --- 辅助函数 ---
void free_ast_node(ASTNode* node);

#endif // AST_H