#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "parser.h"

typedef enum {
	SYM_VARIABLE,
	SYM_FUNCTION
} SymbolType;

typedef struct Symbol {
	char* name;
	SymbolType type;
	int line;
	int is_used;
	char* return_type;
	struct Symbol* next;
} Symbol;

typedef struct SymbolTable {
	Symbol* head;
	struct SymbolTable* parent;
} SymbolTable;

SymbolTable* create_symbol_table(SymbolTable* parent);
void add_symbol(SymbolTable* table, const char* name, SymbolType type, const char* return_type, int line);
Symbol* find_symbol(SymbolTable* table, const char* name);
void free_symbol_table(SymbolTable* table);
void check_unused_variables(SymbolTable* table);

const char* check_semantics(ASTNode* node, SymbolTable* table);

#endif
