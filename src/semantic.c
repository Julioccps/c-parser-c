#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SymbolTable* create_symbol_table(SymbolTable* parent) {
	SymbolTable* table = malloc(sizeof(SymbolTable));
	table->head = NULL;
	table->parent = parent;
	return table;
}

void add_symbol(SymbolTable* table, const char* name, SymbolType type, int line) {
	if (find_symbol(table, name)) {

		Symbol* current = table->head;
		while(current) {
			if (strcmp(current->name, name) == 0) {
				fprintf(stderr, "Semantic Error: Redeclaration of '%s' at line %d\n", name, line);
				return;
			}
			current = current->next;
		}
	}
	Symbol* sym = malloc(sizeof(Symbol));
	sym->name = strdup(name);
	sym->type = type;
	sym->line = line;
	sym->next = table->head;
	table->head = sym;
}

Symbol* find_symbol(SymbolTable* table, const char* name) {
	if (!table) return NULL;
	Symbol* sym = table->head;
	while (sym) {
		if (strcmp(sym->name, name) == 0) return sym;
		sym = sym->next;
	}
	return find_symbol(table->parent, name);
}

void free_symbol_table(SymbolTable* table) {
	if (!table) return;
	Symbol* sym = table->head;
	while (sym) {
		Symbol* tmp = sym;
		sym = sym->next;
		free(tmp->name);
		free(tmp);
	}
	free(table);
}

void check_semantics(ASTNode* node, SymbolTable* table) {
	if (!node) return;

	switch (node->type) {
	case NODE_FUNC_DEF: {
		add_symbol(table, node->token->value, SYM_FUNCTION, node->token->line);
		SymbolTable* func_scope = create_symbol_table(table);
		for (int i = 0; i < node->child_count; i++) {
			check_semantics(node->children[i], func_scope);
		}
		break;
	}

	case NODE_LAMBDA: {
		SymbolTable* lambda_scope = create_symbol_table(table);
		for (int i = 0; i < node->child_count; i++) {
			check_semantics(node->children[i], lambda_scope);
		}
		break;
	}

	case NODE_PARAM:
		add_symbol(table, node->token->value, SYM_VARIABLE, node->token->line);
		break;

	case NODE_VAR_DECL:
		add_symbol(table, node->token->value, SYM_VARIABLE, node->token->line);
		for (int i = 0; i < node->child_count; i++) check_semantics(node->children[i], table);
		break;

	case NODE_ASSIGN:
	case NODE_IDENTIFIER:
		if (!find_symbol(table, node->token->value)) {
			fprintf(stderr, "Semantic Error: Use of undeclared identifier '%s' at line %d\n", 
				node->token->value, node->token->line);
		}
		break;

	case NODE_FUNC_CALL:
		for (int i = 0; i < node->child_count; i++) {
			check_semantics(node->children[i], table);
		}
		break;

	case NODE_LITERAL:
		if (node->token && node->token->type == TOKEN_IDENTIFIER) {
			if (!find_symbol(table, node->token->value)) {
				fprintf(stderr, "Semantic Error: Use of undeclared identifier '%s' at line %d\n", 
					node->token->value, node->token->line);
			}
		}
		break;

	default:
		for (int i = 0; i < node->child_count; i++) {
			check_semantics(node->children[i], table);
		}
		break;
	}
}
