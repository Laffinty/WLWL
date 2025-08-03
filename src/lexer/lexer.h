#ifndef LEXER_H
#define LEXER_H

#include "token.h"

// Lexer 结构体
typedef struct {
    const char* input;      // 输入的源代码字符串
    int position;           // 当前字符位置 (指向当前 ch)
    int read_position;      // 下一个要读取的字符位置
    char ch;                // 当前正在查看的字符
} Lexer;

// --- 公共函数 ---
Lexer* create_lexer(const char* input);  // 修正：返回指针
void free_lexer(Lexer* l);               // 添加：释放函数
Token lexer_next_token(Lexer* l);

#endif // LEXER_H