#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
	NODE_PROGRAM,
	NODE_FUNC_DEF,
	NODE_VAR_DECL,
	NODE_ASSIGN,
	NODE_PRINT,
	NODE_FUNC_CALL,
	NODE_RETURN,
	NODE_BINARY_OP,
	NODE_LITERAL,
	NODE_IDENTIFIER,
	NODE_BLOCK
} NodeType;

typedef struct ASTNode {
	NodeType type;
	Token* token; // Token principal associado ao nó
	struct ASTNode** children;
	int child_count;
	int child_capacity;
} ASTNode;

typedef struct {
	LexerCtx* lexer;
	Token* current_token;
	int token_idx;
} Parser;

Parser* init_parser(LexerCtx* lexer);
ASTNode* parse_program(Parser* parser);

void free_ast(ASTNode* node);
void print_ast(ASTNode* node, int indent);

#endif
