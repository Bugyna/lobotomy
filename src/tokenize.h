#pragma once
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
	TT_DECIMAL,
	TT_STR,
	TT_IDENTIFIER,
	TT_COMMENT,
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
	int index, size, peek;
} LEXER;

void lexer_init(LEXER* lexer)
{
	lexer->index = 0;
	lexer->size = 20;
	lexer->tokens = calloc(lexer->size, sizeof(TOKEN));
}

void add_token(LEXER* lexer, int start, int stop, int type, int len, const char* text)
{
	TOKEN token;
	token.start = start;
	token.stop = stop;
	token.type = type;
	token.len = len+1;
	token.text = malloc(len);
	// printf("adding token with text: %s\n", text);
	strcpy(token.text, text);

	if (lexer->index+1 >= lexer->size) {
		lexer->size += 10;
		lexer->tokens = realloc(lexer->tokens, lexer->size*sizeof(TOKEN));
	}
	lexer->tokens[lexer->index++] = token;
}


void str_add_char(char* str, int* index, int* max, char c)
{
	// why the fuck does realloc not work here is beyond me
	// why it seems to works even without realloc is even more fucking beyond me
	str[*index] = c;
	str[*index+1] = '\0';
	*index = *index + 1;
	if (*index+1 >= *max) {
		// printf("max: %s %d %d\n", str, *index, *max);
		*max = *max + 10;
		// printf("max after: %s %d %d \n", str, *index, *max);
		str = realloc(str, *max);
	}

	// if (c == '\0') {
		// printf("end: %s %d %d\n", str, *index, *max);
		// free(str);
		// str = malloc(*max);
		// char* tmp = malloc(*index);
		// strcpy(tmp, str);
		// printf("test: %s\n", tmp);
		// free(str);
		// str = malloc(*index);
		// strcpy(str, tmp);
		// str = tmp;
		// *max = *index + 10;	
	// }

}

LEXER tokenize(const char text[])
{
	LEXER lexer;
	lexer_init(&lexer);
	int line = 1, column = 0;
	
	int word_i = 0, word_max = 20;
	char* word = malloc(word_max);
	int len = strlen(text)+1;
	// printf("len: %d\n", len);
	bool in_word = false, in_number = false, in_decimal = false, in_comment = false, in_str = false;
    
	int b_count = 0;
    
	
	for (int i = 0; i < len; i++) {
		if (in_word && word_i > 0) {
			if (!((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z') || text[i] == '_')) {
				// word[word_i++] = '\0';
				str_add_char(word, &word_i, &word_max, '\0');
				add_token(&lexer, i-word_i, i, TT_IDENTIFIER, word_i, word);
				in_word = false;
				word_i = 0;
			}
		}

		else if (in_number && word_i > 0) {
			if (!((text[i] >= '0' && text[i] <= '9') || text[i] == '.')) {
				// word[word_i++] = '\0';
				str_add_char(word, &word_i, &word_max, '\0');
				if (in_number & !in_decimal) add_token(&lexer, i-word_i, i, TT_NUMBER, word_i, word);
				else if (in_decimal) add_token(&lexer, i-word_i, i, TT_DECIMAL, word_i, word);
				in_decimal = false;
				in_number = false;
				word_i = 0;
			}
		}

		else if (in_comment) {
			if (text[i] == '\n' || text[i] == '\0') {
				in_comment = false;
				str_add_char(word, &word_i, &word_max, '\0');
				add_token(&lexer, i-word_i, i, TT_COMMENT, word_i, word);
				word_i = 0;
			}

			else {
				// word[word_i++] = text[i];
				str_add_char(word, &word_i, &word_max, text[i]);
				column++;
				continue;
			}
		}

		else if (in_str) {
			if (text[i] == '"') {
				str_add_char(word, &word_i, &word_max, '\0');
				add_token(&lexer, i-word_i, i, TT_STR, word_i, word);
				in_str = false;
				word_i = 0;
				column++;
				continue;
			}

			else {
				str_add_char(word, &word_i, &word_max, text[i]);
				column++;
				continue;
			}
		}

		else if (text[i] == ';' && text[i+1] == ';') {
			in_comment = true;
			i++;
			continue;
		}

		if (text[i] == '\n') {
			line++;
			column = 0;
		}


		else if ((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z') || text[i] == '_') {
			my_assert(in_number, "unexpected character", ERR_WORD_IN_NUMBER, line, column);
			in_word = true;
			// word[word_i++] = text[i];
			str_add_char(word, &word_i, &word_max, text[i]);
		}

		else if (text[i] >= '0' && text[i] <= '9') {
			if (!in_word) {
				in_number = true;
			}

			// word[word_i++] = text[i];
			str_add_char(word, &word_i, &word_max, text[i]);
		}

		else if (text[i] == '.') {
			if (in_number) {
				in_decimal = true;
			}
			// word[word_i++] = text[i];
			str_add_char(word, &word_i, &word_max, text[i]);
		}

		else if (text[i] == '(') {
			add_token(&lexer, i, i, TT_LPAREN, 2, (char[]){text[i], '\0'});
			b_count++;
			
		}

		else if (text[i] == '"') {
			in_str = true;
		}

		// else if (text[i] == ' ') {
			
		// }
        
		else if (text[i] == ')') {
			add_token(&lexer, i, i, TT_RPAREN, 2, (char[]){text[i], '\0'});
			b_count--;
			my_assert((b_count < 0), "too many brackets", ERR_TOO_MANY_BRACKETS, line, column);
		}
        
		else if (text[i] == '+' || text[i] == '-' || text[i] == '*' || text[i] == '/'  || text[i] == '%' || text[i] == '&' || text[i] == '|' || text[i] == '^' || text[i] == '!' || text[i] == '?' || text[i] == '@' || text[i] == '#' || text[i] == '$' || text[i] == '=' || text[i] == '<' || text[i] == '>' || text[i] == ':') {
			add_token(&lexer, i, i, TT_IDENTIFIER, 2, (char[]){text[i], '\0'});
		}

		column++;
	}

	// printf("brackets: %d\n", b_count);
	my_assert((b_count > 0), "too many brackets", ERR_TOO_MANY_BRACKETS, line, column);

	for (int i = 0; i < lexer.index; i++) {
		if (lexer.tokens[i].text == NULL)
			break;
    
		// printf("t: %d | text: %s | type: %d\n", i, lexer.tokens[i].text, lexer.tokens[i].type);
		// printf("%s ", lexer.tokens[i].text);
	}
	// printf("\n\n----TOKENIZER DONE----\n\n");


	free(word);
	return lexer;
	// free(lexer.tokens);
}



