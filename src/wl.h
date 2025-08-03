#ifndef WL_H
#define WL_H

// This is the master header file for the WLWL interpreter.

// --- Core Modules ---
// These are fundamental and have no dependencies on other interpreter modules.
#include "core/common.h"
#include "core/logger.h"
#include "core/dyn_array.h" // Must be included early!

// --- Forward Declarations ---
// Forward-declare structs to break circular dependencies between headers.
struct ASTNode;
struct Environment;
struct Object;

// --- Lexer Modules ---
#include "lexer/token.h"
#include "lexer/lexer.h"

// --- Parser Modules ---
#include "parser/ast.h"
#include "parser/parser.h"

// --- Runtime Modules ---
#include "runtime/object.h"
#include "runtime/environment.h"
#include "runtime/builtins.h"
// #include "runtime/evaluator.h" // We will remove this to avoid conflicts.

// --- Main Evaluator Function ---
// The main entry point for evaluation is declared in environment.h now.
// Object* eval(ASTNode* node, Environment* env);

#endif // WL_H
