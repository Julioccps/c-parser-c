#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

const char* token_type_to_str(TokenType type) {
	switch (type) {
		case TOKEN_KEYWORD: return "KEYWORD";
		case TOKEN_SYMBOL: return "SYMBOL";
		case TOKEN_IDENTIFIER: return "IDENTIFIER";
		case TOKEN_LITERAL_INT: return "LITERAL_INT";
		case TOKEN_LITERAL_STRING: return "LITERAL_STRING";
		case TOKEN_EOF: return "EOF";
		case TOKEN_ERROR: return "ERROR";
		default: return "UNKNOWN";
	}
}

int main(int argc, char** argv){
	if (argc < 2) {
		fprintf(stderr, "Error: Program needs more than one argument.\n");
		return 1;
	}
	char* filename = NULL;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
			printf("Usage: %s -f <filename>\n", argv[0]);
			return 0;
		}
		else if(strcmp(argv[i], "-f") == 0) {
			if (i + 1 < argc) {
				filename = argv[i + 1];
				i++;
			} else {
				fprintf(stderr, "Error: -f option requires a filename argument.\n");
				return 1;
			}
		}
		else {
			fprintf(stderr, "Error: Unknown argument '%s'.\n", argv[i]);
			return 1;
		}
	}

	if (filename == NULL) {
		fprintf(stderr, "Error: No input file provided. Use -f <filename>.\n");
		return 1;
	}
	
	FILE* file = fopen(filename, "r");
	
	if (file == NULL) {
		fprintf(stderr, "Error: Could not open file '%s'.\n", filename);
		return 1;
	}

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buffer = malloc(length + 1);
	if (buffer) {
		fread(buffer, 1, length, file);
		buffer[length] = '\0';
	}
	fclose(file);

	if (buffer == NULL) {
		fprintf(stderr, "Error: Could not allocate memory for file buffer.\n");
		return 1;
	}

	LexerCtx* lexer = init_lexer(buffer);
	tokenize_all(lexer);

	printf("--- TOKENS FOUND ---\n");
	printf("%-15s | %-15s | %-5s | %-5s\n", "TYPE", "VALUE", "LINE", "COL");
	printf("----------------------------------------------------------\n");
	for (int i = 0; i < lexer->token_count; i++) {
		Token* t = lexer->tokens[i];
		printf("%-15s | %-15s | %-5d | %-5d\n", 
			token_type_to_str(t->type), 
			t->value, 
			t->line, 
			t->column);
	}
	printf("----------------------------------------------------------\n");

	free_context(lexer);
	free(buffer);
	
	return 0;
}
