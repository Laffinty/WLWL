#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stddef.h> // Include for size_t

// Lexer 结构体
typedef struct {
    const char* input;      // 输入的源代码字符串
    size_t position;        // 当前字符位置 (指向当前 ch), use size_t to match strlen
    size_t read_position;   // 下一个要读取的字符位置, use size_t
    char ch;                // 当前正在查看的字符
} Lexer;

// --- 公共函数 ---
Lexer* create_lexer(const char* input);
void free_lexer(Lexer* l);
Token lexer_next_token(Lexer* l);

#endif // LEXER_H
