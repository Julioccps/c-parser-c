#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser* init_parser(LexerCtx* lexer) {
	Parser* parser = malloc(sizeof(Parser));
	parser->lexer = lexer;
	parser->token_idx = 0;
	parser->current_token = (lexer->token_count > 0) ? lexer->tokens[0] : NULL;
	return parser;
}

static void advance_parser(Parser* parser) {
	if (parser->token_idx < parser->lexer->token_count - 1) {
		parser->token_idx++;
		parser->current_token = parser->lexer->tokens[parser->token_idx];
	}
}

static ASTNode* create_node(NodeType type, Token* token) {
	ASTNode* node = malloc(sizeof(ASTNode));
	node->type = type;
	node->token = token;
	node->child_count = 0;
	node->child_capacity = 4;
	node->children = malloc(sizeof(ASTNode*) * node->child_capacity);
	return node;
}

static void add_child(ASTNode* parent, ASTNode* child) {
	if (!child) return;
	if (parent->child_count >= parent->child_capacity) {
		parent->child_capacity *= 2;
		parent->children = realloc(parent->children, sizeof(ASTNode*) * parent->child_capacity);
	}
	parent->children[parent->child_count++] = child;
}

static ASTNode* parse_expression(Parser* parser);
static ASTNode* parse_statement(Parser* parser);

static void expect(Parser* parser, const char* value) {
	if (strcmp(parser->current_token->value, value) != 0) {
		fprintf(stderr, "Syntax Error: Expected '%s' at line %d, column %d (found '%s')\n", 
			value, parser->current_token->line, parser->current_token->column, parser->current_token->value);
	} else {
		advance_parser(parser);
	}
}

static void parse_parameters(Parser* parser, ASTNode* parent) {
	expect(parser, "(");
	if (strcmp(parser->current_token->value, ")") != 0) {
		while (1) {
			Token* ptype = parser->current_token;
			advance_parser(parser); // type
			Token* pid = parser->current_token;
			advance_parser(parser); // id
			ASTNode* param = create_node(NODE_PARAM, pid);
			add_child(param, create_node(NODE_LITERAL, ptype));
			add_child(parent, param);

			if (strcmp(parser->current_token->value, ",") == 0) {
				advance_parser(parser);
			} else {
				break;
			}
		}
	}
	expect(parser, ")");
}

static ASTNode* parse_lambda(Parser* parser) {
	Token* start_token = parser->current_token;
	expect(parser, "[");
	expect(parser, "]");
	
	ASTNode* node = create_node(NODE_LAMBDA, start_token);
	parse_parameters(parser, node);
	
	expect(parser, "{");
	ASTNode* body = create_node(NODE_BLOCK, NULL);
	while (strcmp(parser->current_token->value, "}") != 0 && parser->current_token->type != TOKEN_EOF) {
		ASTNode* stmt = parse_statement(parser);
		if (stmt) add_child(body, stmt);
	}
	add_child(node, body);
	expect(parser, "}");
	
	return node;
}

static ASTNode* parse_primary(Parser* parser) {
	Token* t = parser->current_token;
	if (t->type == TOKEN_IDENTIFIER) {
		ASTNode* node = create_node(NODE_LITERAL, t);
		advance_parser(parser);
		return node;
	} else if (t->type == TOKEN_LITERAL_INT || t->type == TOKEN_LITERAL_STRING) {
		ASTNode* node = create_node(NODE_LITERAL, t);
		advance_parser(parser);
		return node;
	} else if (strcmp(t->value, "(") == 0) {
		advance_parser(parser);
		ASTNode* expr = parse_expression(parser);
		if (strcmp(parser->current_token->value, ")") == 0) advance_parser(parser);
		return expr;
	} else if (strcmp(t->value, "[") == 0) {
		return parse_lambda(parser);
	}
	return NULL;
}

static ASTNode* parse_postfix(Parser* parser) {
	ASTNode* node = parse_primary(parser);
	while (parser->current_token && strcmp(parser->current_token->value, "(") == 0) {
		Token* open_par = parser->current_token;
		advance_parser(parser); // (
		ASTNode* call = create_node(NODE_FUNC_CALL, open_par);
		add_child(call, node);
		if (strcmp(parser->current_token->value, ")") != 0) {
			while (1) {
				add_child(call, parse_expression(parser));
				if (strcmp(parser->current_token->value, ",") == 0) {
					advance_parser(parser);
				} else {
					break;
				}
			}
		}
		expect(parser, ")");
		node = call;
	}
	return node;
}

static ASTNode* parse_multiplicative(Parser* parser) {
	ASTNode* left = parse_postfix(parser);
	while (strcmp(parser->current_token->value, "*") == 0 || strcmp(parser->current_token->value, "/") == 0) {
		Token* op = parser->current_token;
		advance_parser(parser);
		ASTNode* node = create_node(NODE_BINARY_OP, op);
		add_child(node, left);
		add_child(node, parse_postfix(parser));
		left = node;
	}
	return left;
}

static ASTNode* parse_expression(Parser* parser) {
	ASTNode* left = parse_multiplicative(parser);
	while (strcmp(parser->current_token->value, "+") == 0 || strcmp(parser->current_token->value, "-") == 0) {
		Token* op = parser->current_token;
		advance_parser(parser);
		ASTNode* node = create_node(NODE_BINARY_OP, op);
		add_child(node, left);
		add_child(node, parse_multiplicative(parser));
		left = node;
	}
	return left;
}

static ASTNode* parse_statement(Parser* parser) {
	if (parser->current_token->type == TOKEN_IDENTIFIER) {
		Token* id = parser->current_token;
		Token* next = (parser->token_idx + 1 < parser->lexer->token_count) ? parser->lexer->tokens[parser->token_idx + 1] : NULL;

		if (next && strcmp(next->value, "=") == 0) {
			ASTNode* node = create_node(NODE_ASSIGN, id);
			advance_parser(parser); // ID
			advance_parser(parser); // =
			add_child(node, parse_expression(parser));
			expect(parser, ";");
			return node;
		} else {
			ASTNode* expr = parse_expression(parser);
			expect(parser, ";");
			return expr;
		}
	} else if (parser->current_token->type == TOKEN_KEYWORD) {
		if (strcmp(parser->current_token->value, "std") == 0) {
			ASTNode* node = create_node(NODE_PRINT, parser->current_token);
			while (strcmp(parser->current_token->value, ";") != 0 && parser->current_token->type != TOKEN_EOF) {
				if (parser->current_token->type == TOKEN_LITERAL_STRING || 
				    parser->current_token->type == TOKEN_IDENTIFIER || 
				    parser->current_token->type == TOKEN_LITERAL_INT) {
					add_child(node, create_node(NODE_LITERAL, parser->current_token));
				}
				advance_parser(parser);
			}
			expect(parser, ";");
			return node;
		} else if (strcmp(parser->current_token->value, "int") == 0) {
			advance_parser(parser); // int
			ASTNode* node = create_node(NODE_VAR_DECL, parser->current_token);
			advance_parser(parser); // ID
			if (strcmp(parser->current_token->value, "=") == 0) {
				advance_parser(parser); // =
				add_child(node, parse_expression(parser));
			}
			expect(parser, ";");
			return node;
		} else if (strcmp(parser->current_token->value, "return") == 0) {
			ASTNode* node = create_node(NODE_RETURN, parser->current_token);
			advance_parser(parser);
			if (strcmp(parser->current_token->value, ";") != 0) {
				add_child(node, parse_expression(parser));
			}
			expect(parser, ";");
			return node;
		}
	}
	advance_parser(parser);
	return NULL;
}

static ASTNode* parse_function(Parser* parser) {
	Token* type_token = parser->current_token;
	advance_parser(parser); // Skip type
	ASTNode* node = create_node(NODE_FUNC_DEF, parser->current_token);
	advance_parser(parser); // Skip ID
	
	// Add return type as first child
	add_child(node, create_node(NODE_LITERAL, type_token));

	parse_parameters(parser, node);
	expect(parser, "{");

	ASTNode* body = create_node(NODE_BLOCK, NULL);
	while (strcmp(parser->current_token->value, "}") != 0 && parser->current_token->type != TOKEN_EOF) {
		ASTNode* stmt = parse_statement(parser);
		if (stmt) add_child(body, stmt);
	}
	add_child(node, body);
	if (strcmp(parser->current_token->value, "}") == 0) advance_parser(parser);
	return node;
}

ASTNode* parse_program(Parser* parser) {
	ASTNode* root = create_node(NODE_PROGRAM, NULL);
	while (parser->current_token && parser->current_token->type != TOKEN_EOF) {
		if (parser->current_token->type == TOKEN_KEYWORD && 
		    (strcmp(parser->current_token->value, "void") == 0 || strcmp(parser->current_token->value, "int") == 0)) {

			Token* next = (parser->token_idx + 1 < parser->lexer->token_count) ? parser->lexer->tokens[parser->token_idx + 1] : NULL;
			Token* after = (parser->token_idx + 2 < parser->lexer->token_count) ? parser->lexer->tokens[parser->token_idx + 2] : NULL;

			if (next && next->type == TOKEN_IDENTIFIER && after && strcmp(after->value, "(") == 0) {
				add_child(root, parse_function(parser));
			} else {
				add_child(root, parse_statement(parser));
			}
		} else {
			advance_parser(parser);
		}
	}
	return root;
}

void free_ast(ASTNode* node) {
	if (!node) return;
	for (int i = 0; i < node->child_count; i++) free_ast(node->children[i]);
	free(node->children);
	free(node);
}

void print_ast(ASTNode* node, int indent) {
	if (!node) return;
	for (int i = 0; i < indent; i++) printf("  ");
	printf("[%d] %s\n", node->type, node->token ? node->token->value : "BLOCK");
	for (int i = 0; i < node->child_count; i++) print_ast(node->children[i], indent + 1);
}
