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

Symbol* peek_symbol(SymbolTable* table, const char* name) {
	if (!table) return NULL;
	Symbol* sym = table->head;
	while (sym) {
		if (strcmp(sym->name, name) == 0) return sym;
		sym = sym->next;
	}
	return peek_symbol(table->parent, name);
}

Symbol* find_symbol(SymbolTable* table, const char* name) {
	Symbol* sym = peek_symbol(table, name);
	if (sym) sym->is_used = 1;
	return sym;
}

void add_symbol(SymbolTable* table, const char* name, SymbolType type, const char* return_type, int line) {
	// Shadowing check - use peek_symbol to avoid marking it as used
	if (table->parent && peek_symbol(table->parent, name)) {
		printf("Semantic Warning: '%s' at line %d shadows a previous declaration\n", name, line);
	}

	// Redeclaration check in current scope
	Symbol* current = table->head;
	while (current) {
		if (strcmp(current->name, name) == 0) {
			fprintf(stderr, "Semantic Error: Redeclaration of '%s' at line %d\n", name, line);
			return;
		}
		current = current->next;
	}

	Symbol* sym = malloc(sizeof(Symbol));
	sym->name = strdup(name);
	sym->type = type;
	sym->line = line;
	sym->is_used = 0;
	sym->return_type = return_type ? strdup(return_type) : NULL;
	sym->next = table->head;
	table->head = sym;
}

void free_symbol_table(SymbolTable* table) {
	if (!table) return;
	Symbol* sym = table->head;
	while (sym) {
		Symbol* tmp = sym;
		sym = sym->next;
		free(tmp->name);
		if (tmp->return_type) free(tmp->return_type);
		free(tmp);
	}
	free(table);
}

void check_unused_variables(SymbolTable* table) {
	if (!table) return;
	Symbol* sym = table->head;
	while (sym) {
		if (sym->type == SYM_VARIABLE && !sym->is_used) {
			printf("Semantic Warning: Unused variable '%s' at line %d\n", sym->name, sym->line);
		}
		sym = sym->next;
	}
}

static int has_return(ASTNode* node) {
	if (!node) return 0;
	if (node->type == NODE_RETURN) return 1;
	for (int i = 0; i < node->child_count; i++) {
		if (has_return(node->children[i])) return 1;
	}
	return 0;
}

const char* check_semantics(ASTNode* node, SymbolTable* table) {
	if (!node) return "void";

	switch (node->type) {
	case NODE_PROGRAM:
		for (int i = 0; i < node->child_count; i++) {
			const char* stmt_type = check_semantics(node->children[i], table);
			if (node->children[i]->type == NODE_FUNC_CALL && strcmp(stmt_type, "void") != 0) {
				printf("Semantic Warning: Ignoring return value of function at line %d\n", 
					node->children[i]->token->line);
			}
		}
		return "void";

	case NODE_FUNC_DEF: {
		const char* ret_type = "void";
		if (node->child_count > 0 && node->children[0]->type == NODE_LITERAL) {
			ret_type = node->children[0]->token->value;
		}
		add_symbol(table, node->token->value, SYM_FUNCTION, ret_type, node->token->line);
		
		SymbolTable* func_scope = create_symbol_table(table);
		for (int i = 1; i < node->child_count; i++) {
			check_semantics(node->children[i], func_scope);
		}
		
		if (strcmp(ret_type, "int") == 0 && !has_return(node)) {
			fprintf(stderr, "Semantic Error: Function '%s' at line %d must return a value\n", 
				node->token->value, node->token->line);
		}
		
		check_unused_variables(func_scope);
		free_symbol_table(func_scope);
		return ret_type;
	}

	case NODE_LAMBDA: {
		SymbolTable* lambda_scope = create_symbol_table(table);
		for (int i = 0; i < node->child_count; i++) {
			check_semantics(node->children[i], lambda_scope);
		}
		check_unused_variables(lambda_scope);
		free_symbol_table(lambda_scope);
		return "int";
	}

	case NODE_PARAM:
		add_symbol(table, node->token->value, SYM_VARIABLE, "int", node->token->line);
		return "int";

	case NODE_VAR_DECL: {
		const char* type = "int";
		add_symbol(table, node->token->value, SYM_VARIABLE, type, node->token->line);
		for (int i = 0; i < node->child_count; i++) {
			const char* init_type = check_semantics(node->children[i], table);
			if (strcmp(init_type, "void") == 0) {
				fprintf(stderr, "Semantic Error: Cannot initialize variable '%s' at line %d with void value\n", 
					node->token->value, node->token->line);
			}
		}
		return "void";
	}

	case NODE_ASSIGN: {
		Symbol* sym = peek_symbol(table, node->token->value);
		if (!sym) {
			fprintf(stderr, "Semantic Error: Use of undeclared identifier '%s' at line %d\n", 
				node->token->value, node->token->line);
		} else {
            sym->is_used = 1;
        }
		if (node->child_count > 0) {
			const char* rhs_type = check_semantics(node->children[0], table);
			if (strcmp(rhs_type, "void") == 0) {
				fprintf(stderr, "Semantic Error: Cannot assign void to variable '%s' at line %d\n", 
					node->token->value, node->token->line);
			}
		}
		return "int";
	}

	case NODE_FUNC_CALL: {
		ASTNode* func_node = node->children[0];
		Symbol* sym = NULL;
		if (func_node->type == NODE_LITERAL && func_node->token->type == TOKEN_IDENTIFIER) {
			sym = find_symbol(table, func_node->token->value);
			if (!sym || sym->type != SYM_FUNCTION) {
				fprintf(stderr, "Semantic Error: '%s' at line %d is not a function\n", 
					func_node->token->value, func_node->token->line);
				return "void";
			}
		}
		
		for (int i = 1; i < node->child_count; i++) {
			check_semantics(node->children[i], table);
		}
		
		return sym ? sym->return_type : "void";
	}

	case NODE_PRINT:
		for (int i = 0; i < node->child_count; i++) {
			check_semantics(node->children[i], table);
		}
		return "void";

	case NODE_RETURN: {
		const char* ret_type = "void";
		if (node->child_count > 0) {
			ret_type = check_semantics(node->children[0], table);
		}
		return ret_type;
	}

	case NODE_BINARY_OP:
		check_semantics(node->children[0], table);
		check_semantics(node->children[1], table);
		return "int";

	case NODE_LITERAL:
		if (node->token && node->token->type == TOKEN_IDENTIFIER) {
			Symbol* sym = find_symbol(table, node->token->value);
			if (!sym) {
				fprintf(stderr, "Semantic Error: Use of undeclared identifier '%s' at line %d\n", 
					node->token->value, node->token->line);
				return "int";
			}
			return "int";
		}
		return (node->token && node->token->type == TOKEN_LITERAL_STRING) ? "string" : "int";

	case NODE_BLOCK:
		for (int i = 0; i < node->child_count; i++) {
			const char* stmt_type = check_semantics(node->children[i], table);
			if (node->children[i]->type == NODE_FUNC_CALL && strcmp(stmt_type, "void") != 0) {
				printf("Semantic Warning: Ignoring return value of function at line %d\n", 
					node->children[i]->token->line);
			}
		}
		return "void";

	default:
		for (int i = 0; i < node->child_count; i++) {
			check_semantics(node->children[i], table);
		}
		return "void";
	}
}
