#include "lexer.h"
#include <stdlib.h>
#include <string.h>


Token* create_token(TokenType type, const char *value, int line, int column){
	Token* tk = malloc(sizeof(Token));
	tk->type = type;
	tk->value = strdup(value);
	tk->line = line;
	tk->column = column;

	return tk;
}

void free_token(Token *token){
	if (token){
		free(token->value);
		free(token);
	}
}
