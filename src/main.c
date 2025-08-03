#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wl.h"

// REPL (Read-Eval-Print-Loop)
static void run_repl() {
    printf("Initializing REPL...\n");
    fflush(stdout);
    
    char line[1024];
    Environment* env = create_environment();
    if (!env) {
        printf("Failed to create environment!\n");
        return;
    }
    printf("Environment created successfully.\n");
    fflush(stdout);
    
    register_builtins(env);
    printf("Builtins registered successfully.\n");
    fflush(stdout);

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

        printf("Input received: %s", line);
        fflush(stdout);

        Lexer* l = create_lexer(line);
        if (!l) {
            printf("Failed to create lexer!\n");
            continue;
        }
        printf("Lexer created successfully.\n");
        fflush(stdout);
        
        Parser p = create_parser(l);
        printf("Parser created successfully.\n");
        fflush(stdout);
        
        ASTNode* program = parse_program(&p);
        if (!program) {
            printf("Failed to parse program!\n");
            free_parser(&p);
            free_lexer(l);
            continue;
        }
        printf("Program parsed successfully.\n");
        fflush(stdout);

        if (da_count(&p.errors) > 0) {
            printf("Parser has %d errors:\n", da_count(&p.errors));
            for (int i = 0; i < da_count(&p.errors); i++) {
                char** error_msg_ptr = (char**)da_get(&p.errors, i);
                if (error_msg_ptr) {
                    printf("\t- %s\n", *error_msg_ptr);
                }
            }
        } else {
            printf("Evaluating...\n");
            fflush(stdout);
            
            Object* result = eval(program, env);
            if (result) {
                printf("Result: ");
                print_object(result);
                printf("\n");
            } else {
                printf("No result returned.\n");
            }
        }

        free_ast_node(program);
        free_parser(&p);
        free_lexer(l);
    }

    free_environment(env);
}

int main(int argc, char** argv) {
    printf("Starting WLWL Interpreter...\n");
    fflush(stdout);
    
    // Initialize global objects
    printf("Initializing global objects...\n");
    fflush(stdout);
    init_global_objects();
    printf("Global objects initialized.\n");
    fflush(stdout);
    
    // For now, we just run the REPL
    run_repl();
    
    printf("Exiting...\n");
    return 0;
}