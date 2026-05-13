#include "lexer.h"
#include "lang_defs.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


Token* create_token(TokenType type, const char *value, int line, int column){
	Token* token = malloc(sizeof(Token));
	token->type = type;
	token->value = strdup(value);
	token->line = line;
	token->column = column;

	return token;
}

LexerCtx* init_lexer(const char* source){
	LexerCtx* lexer_ctx = malloc(sizeof(LexerCtx));
	lexer_ctx->source = strdup(source);
	lexer_ctx->line = 1;
	lexer_ctx->column = 1;
	lexer_ctx->cursor = 0;
	lexer_ctx->token_count = 0;
	lexer_ctx->current_idx = 0;
	lexer_ctx->token_capacity = INITIAL_TOKEN_COUNT;
	lexer_ctx->tokens = malloc(sizeof(Token*) * lexer_ctx->token_capacity);

	return lexer_ctx;
}

Token* advance_token(LexerCtx* ctx){
	if (ctx->token_count >= ctx->token_capacity){
		ctx->token_capacity *= 2;
		ctx->tokens = realloc(ctx->tokens, sizeof(Token*) * ctx->token_capacity);
	}
	char* value = NULL;
	TokenType type = TOKEN_ERROR;
	const char tmp[2] = {ctx->source[ctx->cursor], '\0'};	
	switch (ctx->source[ctx->cursor]){
		case OPEN_PAR:
		case CLOSE_PAR:
		case OPEN_BRACE:
		case CLOSE_BRACE:
		case SEMICOLON:
		case ASSIGN:
			type = TOKEN_SYMBOL;
			value = strdup(tmp);
			ctx->cursor++;
			ctx->column++;
			break;
		case '<':
			if (ctx->source[ctx->cursor + 1] == '<'){
				value = strdup(BITSHIFT_LEFT);
				type = TOKEN_SYMBOL;
				ctx->cursor += 2;
				ctx->column += 2;
				break;
			}
			value = strdup(tmp);
			ctx->cursor++;
			ctx->column++;
			break;
		case ':':
			if (ctx->source[ctx->cursor + 1] == ':'){
				value = strdup(SCOPE_RESOLUTION);
				type = TOKEN_SYMBOL;
				ctx->cursor += 2;
				ctx->column += 2;
				break;
			}
			value = strdup(tmp);
			ctx->cursor++;
			ctx->column++;
			break;
		default:
			if (isdigit(ctx->source[ctx->cursor])){
				int start = ctx->cursor;

				while (isdigit(ctx->source[ctx->cursor])){
					ctx->cursor++;
					ctx->column++;
				}	
				int length = ctx->cursor - start;
				type = TOKEN_LITERAL_INT;
				value = strndup(&ctx->source[start], length);
				break;
			}else if (isalpha(ctx->source[ctx->cursor]) || ctx->source[ctx->cursor] == '_'){
				int start = ctx->cursor;

				while (isalpha(ctx->source[ctx->cursor]) || ctx->source[ctx->cursor] == '_'){
					ctx->cursor++;
					ctx->column++;
				}	
				int length = ctx->cursor - start;
				value = strndup(&ctx->source[start], length);
				if ((strcmp(value, INT_DEF) == 0) || (strcmp(value, VOID_DEF) == 0) 
				    || (strcmp(value, STD_NAMESPACE) == 0) || (strcmp(value, CONSOLE_OUT) == 0)
				    || (strcmp(value, END_LINE) == 0)){
						type = TOKEN_KEYWORD;
				}
				else {
					type = TOKEN_IDENTIFIER;
				}
				break;
			}else if (isspace(ctx->source[ctx->cursor])){
				while (isspace(ctx->source[ctx->cursor])){
				if (ctx->source[ctx->cursor] == '\n'){
					ctx->line++;
					ctx->column = 1;
				}
				else {
					ctx->column++;}
					ctx->cursor++;
				}
				
				break;
			}
			else if (ctx->source[ctx->cursor] == '\0'){
				type = TOKEN_EOF;
			}
	}
	if (value != NULL){
		Token* token = create_token(type, value, ctx->line, ctx->column);
		free(value);
		ctx->tokens[ctx->token_count] = token;
		ctx->token_count++;
		return token; 
	}
	free(value);
	return NULL;
}

Token* peek_token(LexerCtx* ctx, int offset){
	return ctx->tokens[ctx->current_idx + offset];
}

int match_token(LexerCtx *ctx, TokenType type){
	return (ctx->tokens[ctx->current_idx]->type == type);
}

void free_token(Token *token){
	if (token){
		free(token->value);
		free(token);
	}
}

void free_context(LexerCtx* ctx){
	if (ctx){
		for (int i = 0; i < ctx->token_count; i++){
			free_token(ctx->tokens[i]);
		}
		free(ctx->tokens);
		free(ctx->source);
		free(ctx);
	}
}


