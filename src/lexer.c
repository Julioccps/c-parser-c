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

LexerCtx* init_lexer(const char* source){
	LexerCtx* lexer_ctx = malloc(sizeof(LexerCtx));
	lexer_ctx->source = strdup(source);
	lexer_ctx->line = 0;
	lexer_ctx->column = 1;
	lexer_ctx->cursor = 1;
	return lexer_ctx;
}

void free_context(LexerCtx* ctx){
	if (ctx){
		free(ctx->source);
		free(ctx);
	}
}

void free_token(Token *token){
	if (token){
		free(token->value);
		free(token);
	}
}
