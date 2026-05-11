#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_SYMBOL,
    TOKEN_IDENTIFIER,
    TOKEN_LITERAL_INT,
    TOKEN_LITERAL_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
} Token;

typedef struct{
	char* source;
	int cursor;
	int line;
	int column;
}LexerCtx;

// Function to create a new token (to be implemented in lexer.c)
Token* create_token(TokenType type, const char* value, int line, int column);

LexerCtx* init_lexer(const char* source);
// Function to free token and context memory
void free_context(LexerCtx* ctx);
void free_token(Token* token);

#endif
