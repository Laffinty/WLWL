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
    NODE_BLOCK_EXPRESSION,  // 新增语句块类型
    
    // 循环相关节点
    NODE_WHILE_EXPRESSION,
    NODE_FOR_EXPRESSION,
    NODE_FOREACH_EXPRESSION,
    NODE_BREAK_STATEMENT,
    NODE_CONTINUE_STATEMENT,
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

// 新增语句块节点结构
typedef struct {
    DynArray* statements;
} BlockExpressionNode;

// 循环节点结构
typedef struct {
    struct ASTNode* condition;
    struct ASTNode* body;
} WhileExpressionNode;

typedef struct {
    struct ASTNode* init;      // 初始化表达式
    struct ASTNode* condition; // 条件表达式
    struct ASTNode* step;      // 步进表达式
    struct ASTNode* body;      // 循环体
} ForExpressionNode;

typedef struct {
    struct ASTNode* var_name;  // 循环变量名
    struct ASTNode* iterable;  // 可迭代对象
    struct ASTNode* body;      // 循环体
} ForeachExpressionNode;

typedef struct {
    // 空结构，仅表示break语句
} BreakStatementNode;

typedef struct {
    // 空结构，仅表示continue语句
} ContinueStatementNode;

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
        BlockExpressionNode     block_expr;  // 新增语句块联合体成员
        
        // 循环节点联合体成员
        WhileExpressionNode     while_expr;
        ForExpressionNode       for_expr;
        ForeachExpressionNode   foreach_expr;
        BreakStatementNode      break_stmt;
        ContinueStatementNode   continue_stmt;
    };
} ASTNode;

void free_ast_node(ASTNode* node);

#endif