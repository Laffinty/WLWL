#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wl.h"

// REPL (Read-Eval-Print-Loop)
static void run_repl() {
#ifdef DEBUG_MODE
    printf("Initializing REPL...\n");
    fflush(stdout);
#endif
    
    char line[1024];
    Environment* env = create_environment();
    if (!env) {
        printf("Failed to create environment!\n");
        return;
    }
#ifdef DEBUG_MODE
    printf("Environment created successfully.\n");
    fflush(stdout);
#endif
    
    register_builtins(env);
#ifdef DEBUG_MODE
    printf("Builtins registered successfully.\n");
    fflush(stdout);
#endif

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

#ifdef DEBUG_MODE
        printf("Input received: %s", line);
        fflush(stdout);
#endif

        Lexer* l = create_lexer(line);
        if (!l) {
            printf("Failed to create lexer!\n");
            continue;
        }
#ifdef DEBUG_MODE
        printf("Lexer created successfully.\n");
        fflush(stdout);
#endif
        
        Parser p = create_parser(l);
#ifdef DEBUG_MODE
        printf("Parser created successfully.\n");
        fflush(stdout);
#endif
        
        ASTNode* program = parse_program(&p);
        if (!program) {
            printf("Failed to parse program!\n");
            free_parser(&p);
            free_lexer(l);
            continue;
        }
#ifdef DEBUG_MODE
        printf("Program parsed successfully.\n");
        fflush(stdout);
#endif

        if (da_count(&p.errors) > 0) {
            printf("Parser has %d errors:\n", da_count(&p.errors));
            for (int i = 0; i < da_count(&p.errors); i++) {
                char** error_msg_ptr = (char**)da_get(&p.errors, i);
                if (error_msg_ptr) {
                    printf("\t- %s\n", *error_msg_ptr);
                }
            }
        } else {
#ifdef DEBUG_MODE
            printf("Evaluating...\n");
            fflush(stdout);
#endif
            
            Object* result = eval(program, env);
            if (result) {
                printf("Result: ");
                print_object(result);
                printf("\n");
            }
#ifdef DEBUG_MODE
            else {
                printf("No result returned.\n");
            }
#endif
        }

        free_ast_node(program);
        free_parser(&p);
        free_lexer(l);
    }

    free_environment(env);
}

int main(int argc, char** argv) {
    // Mark parameters as unused to prevent compiler warnings.
    // This is a standard way to tell the compiler that we are intentionally not using them.
    (void)argc;
    (void)argv;

#ifdef DEBUG_MODE
    printf("Starting WLWL Interpreter...\n");
    fflush(stdout);
    
    // Initialize global objects
    printf("Initializing global objects...\n");
    fflush(stdout);
#endif

    init_global_objects();

#ifdef DEBUG_MODE
    printf("Global objects initialized.\n");
    fflush(stdout);
#endif
    
    // For now, we just run the REPL
    run_repl();
    
#ifdef DEBUG_MODE
    printf("Exiting...\n");
#endif

    return 0;
}
