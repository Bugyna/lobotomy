#pragma once
#include "tokenize.h"


void parse(const char text[])
{
	LEXER lexer = tokenize(text);
	
	for (int i = 0; i < lexer.size; i++) {
		if (lexer.tokens[i].text == NULL)
			break;


		printf("t: %d | text: %s | type: %d\n", i, lexer.tokens[i].text, lexer.tokens[i].type);

	}
}