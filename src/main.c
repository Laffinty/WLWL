#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wl.h"

// 从文件读取源代码
static char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", filename);
        return NULL;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size == 0) {
        printf("Warning: File '%s' is empty\n", filename);
        fclose(file);
        return NULL;
    }

    // 分配内存并读取文件内容
    char* content = malloc(file_size + 1);
    if (content == NULL) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(content, 1, file_size, file);
    content[bytes_read] = '\0';  // 确保字符串以空字符结尾
    fclose(file);

    printf("Read %zu bytes from file '%s'\n", bytes_read, filename);
    printf("File content:\n---\n%s\n---\n", content);

    return content;
}

// 调试函数：显示token流
static void debug_tokens(const char* source_code) {
    printf("\n=== TOKEN ANALYSIS ===\n");
    Lexer* lexer = create_lexer(source_code);
    Token token;
    int token_count = 0;
    
    do {
        token = lexer_next_token(lexer);
        printf("Token %d: Type=%s, Literal='%s'\n", 
               token_count++, token_type_to_str(token.type), token.literal);
        
        if (token.type == TOKEN_ILLEGAL) {
            printf("  *** ILLEGAL TOKEN DETECTED ***\n");
            printf("  Current position: %zu\n", lexer->position);
            printf("  Current character: '%c' (ASCII: %d)\n", lexer->ch, (int)lexer->ch);
        }
        
        free_token(&token);
    } while (token.type != TOKEN_EOF && token_count < 50); // 限制输出数量防止无限循环
    
    free_lexer(lexer);
    printf("=== END TOKEN ANALYSIS ===\n\n");
}

// 解释执行源文件
static int interpret_file(const char* filename) {
    // 检查文件扩展名
    const char* ext = strrchr(filename, '.');
    if (ext == NULL || strcmp(ext, ".wl") != 0) {
        printf("Error: File must have .wl extension\n");
        return 1;
    }

    // 读取文件内容
    char* source_code = read_file(filename);
    if (source_code == NULL) {
        return 1;
    }

    printf("Interpreting WLWL file: %s\n", filename);

    // 调试：显示token流
    debug_tokens(source_code);

    // 创建环境
    Environment* env = create_environment();
    if (!env) {
        printf("Failed to create environment!\n");
        free(source_code);
        return 1;
    }

    // 注册内置函数
    register_builtins(env);

    // 创建词法分析器
    Lexer* lexer = create_lexer(source_code);
    if (!lexer) {
        printf("Failed to create lexer!\n");
        free(source_code);
        free_environment(env);
        return 1;
    }

    // 创建语法分析器
    Parser parser = create_parser(lexer);

    // 解析程序
    ASTNode* program = parse_program(&parser);
    if (!program) {
        printf("Failed to parse program!\n");
        free_parser(&parser);
        free_lexer(lexer);
        free(source_code);
        free_environment(env);
        return 1;
    }

    // 检查解析错误
    if (da_count(&parser.errors) > 0) {
        printf("Parse errors found:\n");
        for (int i = 0; i < da_count(&parser.errors); i++) {
            char** error_msg_ptr = (char**)da_get(&parser.errors, i);
            if (error_msg_ptr) {
                printf("  - %s\n", *error_msg_ptr);
            }
        }
        free_ast_node(program);
        free_parser(&parser);
        free_lexer(lexer);
        free(source_code);
        free_environment(env);
        return 1;
    }

    // 执行程序
    Object* result = eval(program, env);
    if (result && IS_ERROR(result)) {
        printf("Runtime error: ");
        print_object(result);
        printf("\n");
        free_ast_node(program);
        free_parser(&parser);
        free_lexer(lexer);
        free(source_code);
        free_environment(env);
        return 1;
    }

    printf("Program executed successfully.\n");

    // 清理资源
    free_ast_node(program);
    free_parser(&parser);
    free_lexer(lexer);
    free(source_code);
    free_environment(env);

    return 0;
}

// REPL (Read-Eval-Print-Loop)
static void run_repl() {
    char line[1024];
    Environment* env = create_environment();
    if (!env) {
        printf("Failed to create environment!\n");
        return;
    }
    
    register_builtins(env);

    printf("WLWL Interpreter REPL v0.1\n");
    printf("Type expressions followed by ';' and press Enter.\n");
    printf("Press Ctrl+C to exit.\n");
    fflush(stdout);
    
    while (1) {
        printf(">> ");
        fflush(stdout);
        
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\nGoodbye!\n");
            break;
        }

        // Skip empty lines
        if (strlen(line) <= 1) {
            continue;
        }

        Lexer* l = create_lexer(line);
        if (!l) {
            printf("Failed to create lexer!\n");
            continue;
        }
        
        Parser p = create_parser(l);
        
        ASTNode* program = parse_program(&p);
        if (!program) {
            printf("Failed to parse program!\n");
            free_parser(&p);
            free_lexer(l);
            continue;
        }

        if (da_count(&p.errors) > 0) {
            printf("Parser has %d errors:\n", da_count(&p.errors));
            for (int i = 0; i < da_count(&p.errors); i++) {
                char** error_msg_ptr = (char**)da_get(&p.errors, i);
                if (error_msg_ptr) {
                    printf("\t- %s\n", *error_msg_ptr);
                }
            }
        } else {
            Object* result = eval(program, env);
            if (result) {
                printf("Result: ");
                print_object(result);
                printf("\n");
            }
        }

        free_ast_node(program);
        free_parser(&p);
        free_lexer(l);
    }

    free_environment(env);
}

static void print_usage(const char* program_name) {
    printf("WLWL Interpreter v0.1\n");
    printf("Usage:\n");
    printf("  %s                - Start interactive REPL\n", program_name);
    printf("  %s <file.wl>      - Execute WLWL source file\n", program_name);
    printf("  %s --help         - Show this help message\n", program_name);
}

int main(int argc, char** argv) {
    init_global_objects();

    // 解析命令行参数
    if (argc == 1) {
        // 没有参数，启动REPL
        run_repl();
    } else if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else {
            // 有一个参数，尝试解释文件
            return interpret_file(argv[1]);
        }
    } else {
        // 参数过多
        printf("Error: Too many arguments\n");
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}