#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"

void generate_bytecode(ASTNode* ast, const char* output_filename);

#endif
