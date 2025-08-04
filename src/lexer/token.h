#ifndef TOKEN_H
#define TOKEN_H

// --- Token 类型枚举 ---
// 根据你的设计规范，操作符 (+, =, >) 被视为标识符
// 关键字 (LET, FUN) 有专门的Token类型
typedef enum {
    TOKEN_ILLEGAL,    // 非法字符
    TOKEN_EOF,        // 文件结束

    // 标识符 + 字面量
    TOKEN_IDENTIFIER, // add, foobar, x, y, +, *, ...
    TOKEN_NUMBER,     // 123.45
    TOKEN_STRING,     // "hello world"

    // 关键字
    TOKEN_LET,        // LET - 不可变变量声明
    TOKEN_VAR,        // VAR - 可变变量声明
    TOKEN_SET,        // SET - 变量重新赋值
    TOKEN_FUN,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_IF,
    TOKEN_RETURN,
    TOKEN_NULL,

    // 分隔符
    TOKEN_LPAREN,     // (
    TOKEN_RPAREN,     // )
    TOKEN_LBRACE,     // {
    TOKEN_RBRACE,     // }
    TOKEN_COMMA,      // ,
    TOKEN_SEMICOLON,  // ;

} TokenType;

// Token 结构体
typedef struct {
    TokenType type;
    char* literal; // Token的字面值
} Token;

// --- 辅助函数 ---
const char* token_type_to_str(TokenType type);
Token create_token(TokenType type, const char* literal);
void free_token(Token* token);  // 新增：释放Token内存
TokenType lookup_identifier(const char* ident);

#endif // TOKEN_H