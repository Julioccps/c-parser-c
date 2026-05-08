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

// Function to create a new token (to be implemented in lexer.c)
Token* create_token(TokenType type, const char* value, int line, int column);

// Function to free token memory
void free_token(Token* token);

#endif
