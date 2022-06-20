#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdint.h>

#include "util.h"


enum
{
	TT_LPAREN=1,
	TT_RPAREN,
	TT_PLUS,
	TT_MINUS,
	TT_MUL,
	TT_DIV,
	TT_AND,
	TT_OR,
	TT_NOT,
	TT_LESS,
	TT_GREATER,
	TT_EQUAL,
	TT_BIN_LESS,
	TT_BIN_GREATER,
	TT_BIN_EQUAL,
	TT_NUMBER,
	TT_IDENTIFIER,
};


typedef struct
{
	int start, stop;
	int type, len;
	char* text;
} TOKEN;


typedef struct
{
	TOKEN* tokens;
	int index, size;
} LEXER;

void add_token(LEXER* lexer, int start, int stop, int type, int len, char text[])
{
	TOKEN token;
	token.start = start;
	token.stop = stop;
	token.type = type;
	token.len = len;
	token.text = malloc(len);
	strcpy(token.text, text);


	if (lexer->index >= lexer->size) {
		lexer->size += 20;
		lexer->tokens = realloc(lexer->tokens, lexer->size);
	}
	lexer->tokens[lexer->index++] = token;
}

void lexer_init(LEXER* lexer)
{
	lexer->index = 0;
	lexer->size = 20;
	lexer->tokens = calloc(lexer->size, sizeof(TOKEN));
}

LEXER tokenize(const char text[])
{
	LEXER lexer;
	lexer_init(&lexer);
	char* word = malloc(20);
	

	int len = strlen(text);

	int b_count = 0;

	
	for (int i = 0; i < len; i++) {
		// printf("loop: %c\n", text[i]);
		if (text[i] == '(') {			
			add_token(&lexer, i, i, TT_LPAREN, 2, (char[]){text[i], '\0'});
			b_count++;
		}

		else if (text[i] == ')') {
			add_token(&lexer, i, i, TT_RPAREN, 2, (char[]){text[i], '\0'});
			b_count--;
			my_assert((b_count < 0), "too many brackets", ERR_TOO_MANY_BRACKETS);
			// if (b_count < 0)
				// printf("too little brackets\n");
		}

		else if (text[i] == '+') {
			add_token(&lexer, i, i, TT_PLUS, 2, (char[]){text[i], '\0'});
		}

		else if (text[i] >= '0' && text[i] <= '9') {
			add_token(&lexer, i, i, TT_NUMBER, 2, (char[]){text[i], '\0'});
		}

	}

	// printf("brackets: %d\n", b_count);
	my_assert((b_count > 0), "too many brackets", ERR_TOO_MANY_BRACKETS);

	printf("\n----TOKENIZER DONE----\n");
	// for (int i = 0; i < lexer.size; i++) {
		// if (lexer.tokens[i].text == NULL)
			// break;

		// printf("t: %d | text: %s | type: %d\n", i, lexer.tokens[i].text, lexer.tokens[i].type);
	// }

	return lexer;
	// free(lexer.tokens);
}



