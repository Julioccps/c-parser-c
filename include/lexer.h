#ifndef LEXER_H
#define LEXER_H

#define INITIAL_TOKEN_COUNT 32

typedef enum {
	TOKEN_KEYWORD,
	TOKEN_SYMBOL,
	TOKEN_IDENTIFIER,
	TOKEN_LITERAL_INT,
	TOKEN_LITERAL_STRING,
	TOKEN_EOF,
	TOKEN_ERROR
} TokenType;

typedef enum {
	LEX_ERR_NONE,
	LEX_ERR_UNRECOGNIZED_CHAR,
	// TODO: Add more possible Lexer errors
} LexerErrorCode;

typedef struct {
	TokenType type;
	char* value;
	int line;
	int column;
	LexerErrorCode error_code;
} Token;

typedef struct{
	char* source;
	int cursor;
	int line;
	int column;

	Token** tokens;
	int token_count;
	int token_capacity;
	int current_idx;
}LexerCtx;

Token* create_token(TokenType type, const char* value, int line, int column);

LexerCtx* init_lexer(const char* source);
Token* advance_token(LexerCtx* ctx);
Token* peek_token(LexerCtx* ctx, int offset);
int match_token(LexerCtx* ctx, TokenType type);

// Function to free token and context memory

void free_context(LexerCtx* ctx);
void free_token(Token* token);

#endif
