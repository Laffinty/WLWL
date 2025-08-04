#include "wl.h"

// --- 辅助函数 ---
static bool is_letter(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}

static bool is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

// 根据设计规范，操作符也是标识符的一部分
static bool is_operator_char(char ch) {
    return strchr("+-*/%^=!<>&|", ch) != NULL;
}

static void read_char(Lexer* l) {
    // The comparison is now between two size_t types, resolving the warning.
    if (l->read_position >= strlen(l->input)) {
        l->ch = 0; // EOF
    } else {
        l->ch = l->input[l->read_position];
    }
    l->position = l->read_position;
    l->read_position += 1;
}

static char* read_identifier(Lexer* l) {
    size_t start_pos = l->position; // Use size_t for consistency
    while (is_letter(l->ch) || is_digit(l->ch) || is_operator_char(l->ch)) {
        read_char(l);
    }
    size_t length = l->position - start_pos; // length is also size_t
    char* ident = malloc(length + 1);
    strncpy(ident, l->input + start_pos, length);
    ident[length] = '\0';
    return ident;
}

static char* read_number(Lexer* l) {
    size_t start_pos = l->position; // Use size_t
    while (is_digit(l->ch)) {
        read_char(l);
    }
    if (l->ch == '.') {
        read_char(l);
        while (is_digit(l->ch)) {
            read_char(l);
        }
    }
    size_t length = l->position - start_pos; // Use size_t
    char* num = malloc(length + 1);
    strncpy(num, l->input + start_pos, length);
    num[length] = '\0';
    return num;
}

static char* read_string(Lexer* l) {
    size_t start_pos = l->position + 1; // Skip opening quote, use size_t
    
    read_char(l); // Move past opening quote
    
    while (l->ch != '"' && l->ch != 0) {
        // Handle escape sequences (basic implementation)
        if (l->ch == '\\') {
            read_char(l); // Skip escape character
            if (l->ch != 0) {
                read_char(l); // Skip escaped character
            }
        } else {
            read_char(l);
        }
    }
    
    size_t length = l->position - start_pos; // Use size_t
    char* str = malloc(length + 1);
    strncpy(str, l->input + start_pos, length);
    str[length] = '\0';
    
    return str;
}

static void skip_whitespace(Lexer* l) {
    while (l->ch == ' ' || l->ch == '\t' || l->ch == '\n' || l->ch == '\r') {
        read_char(l);
    }
}

// --- 公共函数 ---
Lexer* create_lexer(const char* input) {
    Lexer* l = malloc(sizeof(Lexer));
    l->input = input;
    l->position = 0;
    l->read_position = 0;
    l->ch = 0;
    read_char(l);
    return l;
}

void free_lexer(Lexer* l) {
    if (l) {
        free(l);
    }
}

Token lexer_next_token(Lexer* l) {
    Token tok;
    skip_whitespace(l);

    switch (l->ch) {
        case '(': 
            tok = create_token(TOKEN_LPAREN, "("); 
            read_char(l);
            break;
        case ')': 
            tok = create_token(TOKEN_RPAREN, ")"); 
            read_char(l);
            break;
        case '{': 
            tok = create_token(TOKEN_LBRACE, "{"); 
            read_char(l);
            break;
        case '}': 
            tok = create_token(TOKEN_RBRACE, "}"); 
            read_char(l);
            break;
        case ',': 
            tok = create_token(TOKEN_COMMA, ","); 
            read_char(l);
            break;
        case ';': 
            tok = create_token(TOKEN_SEMICOLON, ";"); 
            read_char(l);
            break;
        case '"': {
            char* str_literal = read_string(l);
            tok = create_token(TOKEN_STRING, str_literal);
            free(str_literal); // 释放临时字符串
            read_char(l); // Skip closing quote
            break;
        }
        case 0: 
            tok = create_token(TOKEN_EOF, ""); 
            break;
        default:
            if (is_letter(l->ch) || is_operator_char(l->ch)) {
                char* literal = read_identifier(l);
                tok.type = lookup_identifier(literal);
                tok.literal = literal; // 直接使用，不需要再次strdup
                return tok; // 直接返回，避免再次前进
            } else if (is_digit(l->ch)) {
                char* literal = read_number(l);
                tok = create_token(TOKEN_NUMBER, literal);
                free(literal); // 释放临时字符串
                return tok; // 直接返回
            } else {
                char illegal_char[2] = {l->ch, '\0'};
                tok = create_token(TOKEN_ILLEGAL, illegal_char);
                read_char(l);
            }
    }

    return tok;
}