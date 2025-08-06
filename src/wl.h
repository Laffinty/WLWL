#ifndef WL_H
#define WL_H

#include "core/common.h"
#include "core/logger.h"
#include "core/dyn_array.h"

struct ASTNode;
struct Environment;
struct Object;

#include "lexer/token.h"
#include "lexer/lexer.h"

#include "parser/ast.h"
#include "parser/parser.h"

#include "runtime/object.h"
#include "runtime/environment.h"
#include "runtime/builtins.h"

Object* eval(struct ASTNode* node, struct Environment* env);

#endif