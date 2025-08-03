#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wl.h"

// REPL (Read-Eval-Print-Loop)
static void run_repl() {
    char line[1024];
    Environment* env = create_environment();
    register_builtins(env);

    printf("WLWL Interpreter REPL\n");
    while (1) {
        printf(">> ");
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        Lexer* l = create_lexer(line);
        Parser p = create_parser(l);
        ASTNode* program = parse_program(&p);

        if (da_count(&p.errors) > 0) {
            printf("Parser has %d errors:\n", da_count(&p.errors));
            for (int i = 0; i < da_count(&p.errors); i++) {
                // Correctly get the error string (char*) from the array of char**
                char** error_msg_ptr = (char**)da_get(&p.errors, i);
                printf("\t- %s\n", *error_msg_ptr);
            }
        } else {
             Object* result = eval(program, env);
             if (result) {
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

int main(int argc, char** argv) {
    // For now, we just run the REPL
    run_repl();
    return 0;
}
