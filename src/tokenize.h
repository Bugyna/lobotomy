#pragma once
#include "util.h"


enum
{
	TT_,
	TT_LPAREN,
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
	int line, column;
} MARK;

typedef struct
{
	MARK start, stop;
	int type;
	int index, len;
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
	lexer->peek = 0;
	lexer->size = 20;
	lexer->tokens = calloc(lexer->size, sizeof(TOKEN));
}

// void add_token(LEXER* lexer, int start, int stop, int type, int len, const char* text)
// {
	// TOKEN token;
	// token.start = start;
	// token.stop = stop;
	// token.type = type;
	// token.len = len+1;
	// token.text = malloc(len);
	// printf("adding token with text: %s\n", text);
	// strcpy(token.text, text);

	// if (lexer->index+1 >= lexer->size) {
		// lexer->size += 10;
		// lexer->tokens = realloc(lexer->tokens, lexer->size*sizeof(TOKEN));
	// }
	// lexer->tokens[lexer->index++] = token;
// }

void add_token(LEXER* lexer, TOKEN token, int type)
{
	token.type = type;
	if (lexer->index+1 >= lexer->size) {
		lexer->size += 10;
		lexer->tokens = realloc(lexer->tokens, lexer->size*sizeof(TOKEN));
	}
	lexer->tokens[lexer->index++] = token;
}


// void str_add_char(char* str, int* index, int* max, char c)
// {
	// why the fuck does realloc not work here is beyond me
	// why it seems to works even without realloc is even more fucking beyond me
	// str[*index] = c;
	// str[*index+1] = '\0';
	// *index = *index + 1;
	// if (*index+1 >= *max) {
		// printf("max: %s %d %d\n", str, *index, *max);
		// *max = *max + 10;
		// printf("max after: %s %d %d \n", str, *index, *max);
		// str = realloc(str, *max);
	// }
// }

void str_add_char(TOKEN* token, char c)
{
	token->text[token->index] = c;
	token->text[token->index+1] = '\0';
	token->index += 1;

	if (token->index >= token->len) {
		// printf("max: %s %d %d\n", str, *index, *max);
		token->len += 10;
		// printf("max after: %s %d %d \n", str, *index, *max);
		token->text = realloc(token->text, token->len);
	}
}

void init_token(TOKEN* token)
{
	token->text = calloc(5, 1);
	token->type = TT_;
	token->index = 0;
	token->len = 5;
}

void reset_token(TOKEN* token, int line, int column)
{
	static const TOKEN reset;
	*token = reset;
	free(token->text);

	init_token(token);
	token->start.line = line;
	token->start.column = column;
}

LEXER tokenize(const char text[])
{
	LEXER lexer;
	lexer_init(&lexer);
	int line = 1, column = 0;

	// static const TOKEN reset;
	TOKEN token;
	reset_token(&token, line, column);

	int word_i = 0, word_max = 20;
	// char* word = malloc(word_max);
	int len = strlen(text)+1;
	// printf("len: %d\n", len);
	bool in_word = false, in_number = false,
		in_decimal = false, in_comment = false,
		in_str = false, escaped = false;
	
	int b_count = 0;
	LEXER brackets;
	lexer_init(&brackets);
    
	
	for (int i = 0; i < len; i++) {
		if (in_word && token.index > 0) {
			if (!((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z') || text[i] == '_')
			&& !(text[i] == '+' || text[i] == '-' || text[i] == '*' || text[i] == '/'  || text[i] == '%' || text[i] == '&' || text[i] == '|' || text[i] == '^' || text[i] == '!' || text[i] == '?' || text[i] == '@' || text[i] == '#' || text[i] == '$' || text[i] == '=' || text[i] == '<' || text[i] == '>' || text[i] == ':')
			) {
				// word[word_i++] = '\0';
				// str_add_char(word, &word_i, &word_max, '\0');
				str_add_char(&token, '\0');
				add_token(&lexer, token, TT_IDENTIFIER);
				reset_token(&token, line, column);
				// add_token(&lexer, i-word_i, i, TT_IDENTIFIER, word_i, word);
				in_word = false;
				word_i = 0;
			}
		}

		else if (in_number && token.index > 0) {
			if (!((text[i] >= '0' && text[i] <= '9') || text[i] == '.')) {
				// word[word_i++] = '\0';
				str_add_char(&token, '\0');
				if (in_number & !in_decimal) add_token(&lexer, token, TT_NUMBER);
				else if (in_decimal) add_token(&lexer, token, TT_DECIMAL);
				reset_token(&token, line, column);
				// str_add_char(word, &word_i, &word_max, '\0');
				// if (in_number & !in_decimal) add_token(&lexer, i-word_i, i, TT_NUMBER, word_i, word);
				// else if (in_decimal) add_token(&lexer, i-word_i, i, TT_DECIMAL, word_i, word);
				in_decimal = false;
				in_number = false;
				word_i = 0;
			}
		}

		else if (in_comment) {
			if (text[i] == '\n' || text[i] == '\0') {
				in_comment = false;
				// str_add_char(word, &word_i, &word_max, '\0');
				// add_token(&lexer, i-word_i, i, TT_COMMENT, word_i, word);
				// str_add_char(&token, '\0');
				add_token(&lexer, token, TT_COMMENT);
				reset_token(&token, line, column);
				word_i = 0;
			}

			else {
				// word[word_i++] = text[i];
				// str_add_char(word, &word_i, &word_max, text[i]);
				str_add_char(&token, text[i]);
				column++;
				continue;
			}
		}

		else if (text[i] == '\\' && !escaped) {
			escaped = true;
			continue;
		}

		else if (in_str) {
			if (text[i] == '"' && !escaped) {
				// str_add_char(word, &word_i, &word_max, '\0');
				// add_token(&lexer, i-word_i, i, TT_STR, word_i, word);
				// str_add_char(&token, '\0');
				add_token(&lexer, token, TT_STR);
				reset_token(&token, line, column);
				in_str = false;
				word_i = 0;
				column++;
				continue;
			}

			else if (escaped) {
				escaped = false;
				// switch (text[i])
				// {
					// case 'a':
						// str_add_char(word, &word_i, &word_max, '\a');
					// break;

					// case 'b':
						// str_add_char(word, &word_i, &word_max, '\b');
					// break;

					// case 'f':
						// str_add_char(word, &word_i, &word_max, '\f');
					// break;

					// case 'n':
						// str_add_char(word, &word_i, &word_max, '\n');
					// break;

					// case 'r':
						// str_add_char(word, &word_i, &word_max, '\r');
					// break;

					// case 't':
						// str_add_char(word, &word_i, &word_max, '\t');
					// break;

					// case '\\':
						// str_add_char(word, &word_i, &word_max, '\\');
					// break;

					// case '\'':
						// str_add_char(word, &word_i, &word_max, '\'');
					// break;

					// case '"':
						// str_add_char(word, &word_i, &word_max, '"');
					// break;
				// }
				
				column++;
				continue;
			}

			else {	
				// str_add_char(word, &word_i, &word_max, text[i]);
				str_add_char(&token, text[i]);
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


		else if ((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z') || text[i] == '_'
		|| (text[i] == '+' || text[i] == '-' || text[i] == '*' || text[i] == '/'  || text[i] == '%' || text[i] == '&' || text[i] == '|' || text[i] == '^' || text[i] == '!' || text[i] == '?' || text[i] == '@' || text[i] == '#' || text[i] == '$' || text[i] == '=' || text[i] == '<' || text[i] == '>' || text[i] == ':')
		) {
			my_assert(in_number, "unexpected character", ERR_WORD_IN_NUMBER, line, column);
			in_word = true;
			// word[word_i++] = text[i];
			// str_add_char(word, &word_i, &word_max, text[i]);
			str_add_char(&token, text[i]);
		}

		else if (text[i] >= '0' && text[i] <= '9') {
			if (!in_word) {
				in_number = true;
			}

			// word[word_i++] = text[i];
			// str_add_char(word, &word_i, &word_max, text[i]);
			str_add_char(&token, text[i]);
		}

		else if (text[i] == '.') {
			if (in_number) {
				in_decimal = true;
			}
			// word[word_i++] = text[i];
			// str_add_char(word, &word_i, &word_max, text[i]);
			str_add_char(&token, text[i]);
		}

		else if (text[i] == '(') {
			// add_token(&lexer, i, i, TT_LPAREN, 2, (char[]){text[i], '\0'});
			// add_token(&brackets, i, i, TT_LPAREN, 2, (char[]){text[i], '\0'});
			// token.text = (char[]){text[i], '\0'};
			// token.text = "(";
			str_add_char(&token, '(');
			add_token(&lexer, token, TT_LPAREN);
			add_token(&brackets, token, TT_LPAREN);
			reset_token(&token, line, column);
			b_count++;
		}

		else if (text[i] == '"') {
			in_str = true;
		}

		// else if (text[i] == ' ') {
			// column++;
		// }
        
		else if (text[i] == ')') {
			// add_token(&lexer, i, i, TT_RPAREN, 2, (char[]){text[i], '\0'});
			// add_token(&brackets, i, i, TT_RPAREN, 2, (char[]){text[i], '\0'});
			// token.text = (char[]){text[i], '\0'};
			// token.text = ")";
			str_add_char(&token, ')');
			add_token(&lexer, token, TT_RPAREN);
			add_token(&brackets, token, TT_RPAREN);
			reset_token(&token, line, column);
			b_count--;
			// my_assert((b_count < 0), "too many brackets", ERR_TOO_MANY_BRACKETS, line, column);
			if (b_count < 0) {
				lobotomy_error_s_ne(ERR_TOO_MANY_BRACKETS, "too many brackets at %d.%d", line, column);
			}
		}
        
		// else if (text[i] == '+' || text[i] == '-' || text[i] == '*' || text[i] == '/'  || text[i] == '%' || text[i] == '&' || text[i] == '|' || text[i] == '^' || text[i] == '!' || text[i] == '?' || text[i] == '@' || text[i] == '#' || text[i] == '$' || text[i] == '=' || text[i] == '<' || text[i] == '>' || text[i] == ':') {
			// add_token(&lexer, i, i, TT_IDENTIFIER, 2, (char[]){text[i], '\0'});
			// token.text = (char[]){text[i], '\0'};
			// str_add_char(&token, text[i]);
			// add_token(&lexer, token, TT_IDENTIFIER);
			// reset_token(&token, line, column);
		// }

		column++;
	}

	int b_i = 0, b_j = 0;
	if (b_count > 0) {
		for (; b_i < brackets.index; b_i++) {
			if (brackets.tokens[b_i].type == TT_LPAREN) {
				for (b_j = b_i + 1; b_j <= brackets.index; b_j++) {
					if (brackets.tokens[b_j].type == TT_RPAREN) {
						break;
					}
					lobotomy_error_s_ne(ERR_NO_MATCHING_BRACKET, "bracket at %d.%d is not closed", brackets.tokens[b_i].start.line, brackets.tokens[b_i].start.column);
				}
			}
		}		
	}
	// my_assert((b_count > 0), "bracket is not closed", ERR_NO_MATCHING_BRACKET, line, column);

	for (int i = 0; i < lexer.index; i++) {
		if (lexer.tokens[i].text == NULL)
			break;
    
		// printf("t: %d | text: %s | type: %d\n", i, lexer.tokens[i].text, lexer.tokens[i].type);
		// printf("%s ", lexer.tokens[i].text);
	}
	// printf("\n\n----TOKENIZER DONE----\n\n");


	// free(word);
	return lexer;
	// free(lexer.tokens);
}



