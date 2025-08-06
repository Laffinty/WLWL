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
    
    // 条件语句节点类型
    NODE_IF_EXPRESSION,       // IF(condition, then_branch, else_branch)
    NODE_COND_EXPRESSION,     // COND((cond1, branch1), (cond2, branch2), ...)
    NODE_COND_BRANCH,         // COND分支：(condition, expression)
    
    // 数组字面量（用于COND分支）
    NODE_ARRAY_LITERAL,       // [expr1, expr2, ...]
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

// 条件表达式：IF(condition, then_branch, else_branch)
typedef struct {
    struct ASTNode* condition;    // 条件表达式
    struct ASTNode* then_branch;  // 真分支
    struct ASTNode* else_branch;  // 假分支（可选）
} IfExpressionNode;

// COND多分支条件表达式：COND((cond1, branch1), (cond2, branch2), ...)
typedef struct {
    DynArray* branches;          // DynArray of struct ASTNode* (COND_BRANCH nodes)
} CondExpressionNode;

// COND分支：(condition, expression)
typedef struct {
    struct ASTNode* condition;   // 条件表达式
    struct ASTNode* expression;  // 分支表达式
} CondBranchNode;

// 数组字面量：[expr1, expr2, ...]
typedef struct {
    DynArray* elements;         // DynArray of struct ASTNode*
} ArrayLiteralNode;

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
        IfExpressionNode        if_expr;
        CondExpressionNode      cond_expr;
        CondBranchNode          cond_branch;
        ArrayLiteralNode        array_literal;
    };
} ASTNode;

// --- 辅助函数 ---
void free_ast_node(ASTNode* node);

#endif // AST_H