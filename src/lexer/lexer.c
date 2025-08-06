#include "wl.h"

// --- 辅助函数 ---
static bool is_letter(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}

static bool is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

static bool is_operator_char(char ch) {
    return strchr("+-*/%^=!<>&|", ch) != NULL;
}

static void read_char(Lexer* l) {
    if (l->read_position >= strlen(l->input)) {
        l->ch = 0; // EOF
    } else {
        l->ch = l->input[l->read_position];
    }
    l->position = l->read_position;
    l->read_position += 1;
}

static char peek_char(Lexer* l) {
    if (l->read_position >= strlen(l->input)) {
        return 0;
    } else {
        return l->input[l->read_position];
    }
}

static char* read_identifier(Lexer* l) {
    size_t start_pos = l->position;
    while (is_letter(l->ch) || is_digit(l->ch) || is_operator_char(l->ch)) {
        read_char(l);
    }
    size_t length = l->position - start_pos;
    char* ident = malloc(length + 1);
    strncpy(ident, l->input + start_pos, length);
    ident[length] = '\0';
    return ident;
}

static char* read_number(Lexer* l) {
    size_t start_pos = l->position;
    while (is_digit(l->ch)) {
        read_char(l);
    }
    if (l->ch == '.') {
        read_char(l);
        while (is_digit(l->ch)) {
            read_char(l);
        }
    }
    size_t length = l->position - start_pos;
    char* num = malloc(length + 1);
    strncpy(num, l->input + start_pos, length);
    num[length] = '\0';
    return num;
}

static char* read_string(Lexer* l) {
    size_t start_pos = l->position + 1; // Skip opening quote
    
    read_char(l); // Move past opening quote
    
    while (l->ch != '"' && l->ch != 0) {
        if (l->ch == '\\') {
            read_char(l); // Skip escape character
            if (l->ch != 0) {
                read_char(l); // Skip escaped character
            }
        } else {
            read_char(l);
        }
    }
    
    size_t length = l->position - start_pos;
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

static void skip_line_comment(Lexer* l) {
    while (l->ch != '\n' && l->ch != 0) {
        read_char(l);
    }
}

static void skip_block_comment(Lexer* l) {
    read_char(l); // Skip '/'
    read_char(l); // Skip '*'
    
    while (l->ch != 0) {
        if (l->ch == '*' && peek_char(l) == '/') {
            read_char(l); // Skip '*'
            read_char(l); // Skip '/'
            break;
        }
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

    // 处理注释
    if (l->ch == '/') {
        char next = peek_char(l);
        if (next == '/') {
            skip_line_comment(l);
            return lexer_next_token(l);
        } else if (next == '*') {
            skip_block_comment(l);
            return lexer_next_token(l);
        }
    }

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
        case '[':  // 新增：方括号支持
            tok = create_token(TOKEN_LBRACKET, "["); 
            read_char(l);
            break;
        case ']':  // 新增：方括号支持
            tok = create_token(TOKEN_RBRACKET, "]"); 
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
            free(str_literal);
            read_char(l);
            break;
        }
        case 0: 
            tok = create_token(TOKEN_EOF, ""); 
            break;
        default:
            if (is_letter(l->ch) || is_operator_char(l->ch)) {
                char* literal = read_identifier(l);
                tok.type = lookup_identifier(literal);
                tok.literal = literal; // 直接使用，不需要strdup
                return tok;
            } else if (is_digit(l->ch)) {
                char* literal = read_number(l);
                tok = create_token(TOKEN_NUMBER, literal);
                free(literal);
                return tok;
            } else {
                char illegal_char[2] = {l->ch, '\0'};
                tok = create_token(TOKEN_ILLEGAL, illegal_char);
                read_char(l);
            }
    }

    return tok;
}