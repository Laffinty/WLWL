#ifndef LEXER_H
#define LEXER_H

#include "token.h"

// Lexer 结构体 - 修正：补全所有必要的成员
typedef struct {
    const char* input;      // 输入的源代码字符串
    int position;           // 当前字符位置 (指向当前 ch)
    int read_position;      // 下一个要读取的字符位置
    char ch;                // 当前正在查看的字符
} Lexer;

// --- 公共函数 ---
Lexer create_lexer(const char* input);
Token lexer_next_token(Lexer* l);

#endif // LEXER_H
