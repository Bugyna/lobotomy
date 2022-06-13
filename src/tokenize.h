#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdint.h>


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
};


typedef struct
{
	int start, stop;
	int type, len;
	char* text;
} TOKEN;


void tokenize(const char text[])
{
	TOKEN* tokens;
	TOKEN token;
	tokens = calloc(20, sizeof(TOKEN));
	size_t len = strlen(text);
	for (size_t i = 0; i < len; i++) {
		if (text[i] == '(') {
			token.start = i;
			token.stop = i;
			char t[2] = {text[i], '\0'};
			token.text = t;
			token.len = 2;
			tokens[0] = token;
		}
	}

	printf("t: %s\n", tokens[0].text);
}



