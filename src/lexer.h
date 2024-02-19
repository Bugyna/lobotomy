#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "error.h"
#include "util.h"



#define IS_TERMINATOR(c) (strchr("() \n\t[]{}:", c) != NULL)
#define IS_VALID_SYMBOL_CHAR(c) ((c >= 127 || c < 0) || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || strchr("+-_/?|*&^%$#!~<>=", c) != NULL)
#define IS_NUMBER(c) (c >= '0' && c <= '9')
#define IS_WHITESPACE(c) (c == '\n' || c == '\t' || c == ' ')

static enum
{
	TT_,
	TT_LPAREN,
	TT_RPAREN,
	TT_LBRACKET,
	TT_RBRACKET,
	TT_LCBRACKET,
	TT_RCBRACKET,
	TT_NUMBER,
	TT_DECIMAL,
	TT_STR,
	TT_IDENTIFIER,
	TT_COMMENT,
	TT_MUTABLE,
	TT_ACCESS,
	TT_TYPE,
};

static const char* TOKEN_NAMES[] = 
{
	"TT_",
	"TT_LPAREN",
	"TT_RPAREN",
	"TT_LBRACKET",
	"TT_RBRACKET",
	"TT_LCBRACKET",
	"TT_RCBRACKET",
	"TT_NUMBER",
	"TT_DECIMAL",
	"TT_STR",
	"TT_IDENTIFIER",
	"TT_COMMENT",
	"TT_MUTABLE",
	"TT_ACCESS",
	"TT_TYPE",
};

typedef struct
{
	int line, column;
	int index;
} MARK;

typedef struct
{
	MARK start, stop;
	int type;
	int index, len;
	char* text;
} TOKEN;


TOKEN merge_tokens(TOKEN a, TOKEN b)
{
	a.len += b.len;
	a.text = realloc(a.text, a.len*sizeof(char));
	strcat(a.text, b.text);
	a.stop = b.stop;
	return a;
}

typedef struct
{
	/*!
		LEXER struct stores info about the text input and the tokens it stores
	*/

	/*! @param filename */ 
	const char* filename;
	const char* text;
	size_t text_len, text_peek;

	TOKEN* tokens;
	int index, size, peek;
	bool ignore_untill_current_token_end;
	bool ignore_untill_next_token_end;

	int p_count;
	int b_count;
	MARK pos;

} LEXER;


void print_token(TOKEN t)
{
	printd("TOKEN [%s]: %s\n", TOKEN_NAMES[t.type], t.text);
}

void print_token_pos(TOKEN token, const char* s)
{
	printd("%s %d.%d -- %d.%d\n", s, token.start.line, token.start.column, token.stop.line, token.stop.column);
}

void lexer_init(LEXER* lexer, const char* filename, const char* text)
{
	lexer->filename = filename;
	lexer->text_len = strlen(text);
	lexer->text_peek = 0;
	// lexer->text = calloc(lexer->text_len, sizeof(char));
	lexer->text = text;
	// mbstowcs(lexer->text, text, lexer->text_len);

	lexer->index = 0;
	lexer->peek = 0;
	lexer->size = 1000;
	lexer->ignore_untill_current_token_end = false;
	lexer->ignore_untill_next_token_end = false;
	lexer->tokens = calloc(lexer->size, sizeof(TOKEN));
	lexer->p_count = 0;
	lexer->b_count = 0;

	lexer->pos.line = 1;
	lexer->pos.column = 0;
	lexer->pos.index = 0;
}


void add_token(LEXER* lexer, TOKEN token, MARK stop)
{
	token.stop = stop;
	if (lexer->index+1 >= lexer->size) {
		lexer->size += 10;
		lexer->tokens = realloc(lexer->tokens, lexer->size*sizeof(TOKEN));
	}
	lexer->tokens[lexer->index++] = token;
}


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

void init_token(TOKEN* token, MARK start)
{
	token->text = calloc(2, sizeof(wchar_t));
	token->type = TT_;
	token->index = 0;
	token->len = 2;
	token->start = start;
}


void reset_token(TOKEN* token, MARK position)
{
	// not freeing the text because it's still alive in the lexer->tokens list
	token->text = calloc(2, sizeof(wchar_t));
	token->index = 0;
	token->len = 2;
	token->type = TT_;
	token->start = position;
}

TOKEN new_token(MARK position, int type)
{
	TOKEN token;
	token.text = calloc(2, sizeof(wchar_t));
	token.index = 0;
	token.len = 2;
	token.type = type;
	token.start = position;
	return token;
}

TOKEN pop_token(LEXER* lexer)
{
	if (lexer->index == 0)
	{
		LOBOTOMY_ERROR_S(ERR_LEXER, "Tried popping a token at index -1");
	}

	lexer->index--;
	return lexer->tokens[lexer->index];
}

TOKEN lex_word(LEXER* lexer)
{
	TOKEN token = new_token(lexer->pos, TT_IDENTIFIER);
	for (; lexer->text_peek < lexer->text_len; lexer->text_peek++, lexer->pos.column++)
	{
		char c = lexer->text[lexer->text_peek];
		// if (c == '\n') { lexer->pos.column = 0; lexer->pos.line++; }
		if (IS_VALID_SYMBOL_CHAR(c)) { str_add_char(&token, c); }
		else if (IS_NUMBER(c)) { str_add_char(&token, c); }
		else if (IS_TERMINATOR(c)) { goto exit; }
	}

	exit:
	lexer->pos.column--;
	lexer->text_peek--; // return so the main lexer loop acknowledges the terminator symbol also
	return token;
}


TOKEN lex_number(LEXER* lexer)
{
	TOKEN token = new_token(lexer->pos, TT_NUMBER);
	bool in_decimal = false;
	for (; lexer->text_peek < lexer->text_len; lexer->text_peek++, lexer->pos.column++)
	{
		char c = lexer->text[lexer->text_peek];

		if (IS_NUMBER(c)) {
			str_add_char(&token, c);
		}

		else if (c == '.') {
			if (in_decimal) {
				lobotomy_error_s(ERR_INVALID_DECIMAL, "invalid decimal at %d.%d\n", lexer->pos.line, lexer->pos.column);
			}
			str_add_char(&token, c);
			token.type = TT_DECIMAL;
		}
		
		else if (c == 'x' && token.index == 1) {
			
		}
		
		else if (c == 'b' && token.index == 1) {
			
		}

		else if (IS_TERMINATOR(c))
		{
			goto exit;
		}

		else {
			lobotomy_error_s(ERR_INVALID_CHARACTER_IN_NUMBER, "found invalid character '%c' in number at %d.%d\n", c, lexer->pos.line, lexer->pos.column);
		}
	}

	exit:
	lexer->pos.column--;
	lexer->text_peek--; // return so the main lexer loop acknowledges the terminator symbol also
	return token;
}

TOKEN lex_string(LEXER* lexer)
{
	TOKEN token = new_token(lexer->pos, TT_STR);
	bool escaped = false;

	for (; lexer->text_peek < lexer->text_len; lexer->text_peek++, lexer->pos.column++)
	{
		char c = lexer->text[lexer->text_peek];
		if (c == '\n') {
			if (!escaped) {
				lobotomy_error_s(ERR_UNESCAPED_NEW_LINE_IN_STRING_LITERAL, "found a new line in string literal at %d.%d", lexer->pos.line, lexer->pos.column);
			}
			lexer->pos.column = 0; lexer->pos.line++;
		}

		if (!escaped) {
			if (c == '\\') {
				escaped = true;
			}
		
			else if (c == '"') {
				goto exit;
			}

			else {
				str_add_char(&token, c);
			}
		}

		else if (escaped) {
			escaped = false;
			switch (c)
			{
				case 'a':
					str_add_char(&token, '\a');
				break;

				case 'b':
					str_add_char(&token, '\b');
				break;

				case 'f':
					str_add_char(&token, '\f');
				break;

				case 'n':
					str_add_char(&token, '\n');
				break;

				case 'r':
					str_add_char(&token, '\r');
				break;

				case 't':
					str_add_char(&token, '\t');
				break;

				case '\\':
					str_add_char(&token, '\\');
				break;


				case '\n':
					str_add_char(&token, '\n');
				break;

				case '"':
					str_add_char(&token, '"');
				break;
			}
		}
	}
	exit:
	return token;
}

static inline void lex_ignore_untill_x(LEXER* lexer, const char* blacklist)
{
	for (; lexer->text_peek < lexer->text_len; lexer->text_peek++)
	{
		int c = lexer->text[lexer->text_peek];
		if (c == '\n') { lexer->pos.column = 0; lexer->pos.line++; }
		if (strchr(blacklist, c) != NULL) break;
	}
	lexer->text_peek--; // lexer needs to see the terminator character
}

static inline void lex_ignore_untill_whitespace(LEXER* lexer)
{
	for (; lexer->text_peek < lexer->text_len; lexer->text_peek++)
	{
		int c = lexer->text[lexer->text_peek];
		if (strchr(" \t", c) != NULL) break;
	}
	lexer->text_peek--;
}


static inline void lex_ignore_whitespace(LEXER* lexer)
{
	for (; lexer->text_peek < lexer->text_len; lexer->text_peek++)
	{
		int c = lexer->text[lexer->text_peek];
		if (strchr(" \t", c) == NULL) break;
	}
	lexer->text_peek--;
}


static inline void lex_ignore_comment(LEXER* lexer)
{
	lex_ignore_untill_x(lexer, "\n");
}


static inline void lexer_print_until_eol(LEXER* lexer, size_t n)
{
	for (char c = lexer->text[n++]; n < lexer->text_len && c != '\n' && c != '\r' && c != '\0'; c = lexer->text[n++]) {		
		putchar(c);
	}
	printf(LINE_TERMINATOR);
}

static inline void lexer_print_highlight_at_pos(LEXER* lexer, MARK pos)
{
	for (int i = 0; i < pos.column; i++) {
		putchar(' ');
	}
	putchar('^');
	putchar(' ');
	// printf(LINE_TERMINATOR);
}


void lexer_highlight_error_token(LEXER* lexer, MARK pos, size_t n)
{
	lexer_print_until_eol(lexer, n);
	lexer_print_highlight_at_pos(lexer, pos);
}

int lexer_get_last_newline_index(LEXER* lexer, TOKEN t)
{
	MARK m = t.start;
	int ret = m.index;
	printd("getting shit: %d.%d %d\n", m.line, m.column, m.index);
	for (int i = m.index; i > 0; i--)
	{
		if (lexer->text[i] == '\n') 
		{
			ret -= i;
			break;
		}
	}

	return ret;
}

LEXER tokenize(const char filename[], const char text[])
{
	// printf("wchar: %u\n", sizeof(wchar_t));
	LEXER lexer;
	lexer_init(&lexer, filename, text);

	// static const TOKEN reset;
	TOKEN token;
	reset_token(&token, lexer.pos);
	
	int b_count = 0;
	LEXER brackets;
	lexer_init(&brackets, "brackets", "");
	size_t current_line_index = 0;

	
    
	
	for (; lexer.text_peek < lexer.text_len; lexer.text_peek++, lexer.pos.column++, lexer.pos.index++)
	{
		char c = lexer.text[lexer.text_peek];
		// printf("c: %c\n", c);
		// if (c & 0b10000000 >= 0) {
			// c <<= 8;
			// c &= text[lexer.text_peek++];
		// }
		
		if (c == '\n') {
			lexer.pos.line++;
			lexer.pos.column = 0;
			lexer.text_peek++;
			lex_ignore_whitespace(&lexer);
			current_line_index = lexer.text_peek+1;
			continue;
		}

		if (c == ';' && lexer.text[lexer.text_peek+1] == ';') {
			lexer.text_peek++;
			lexer.pos.column++;
			lex_ignore_comment(&lexer);
		}

		else if (c == ':' && lexer.text[lexer.text_peek+1] == ':') {
			token.type = TT_ACCESS;
			
			// token.text = "::";
			token.stop = lexer.pos;
			TOKEN tmp = pop_token(&lexer);
			if (tmp.type != TT_IDENTIFIER) {
				LOBOTOMY_ERROR_FULL(
					lexer_highlight_error_token(&lexer, token.start, current_line_index),
					lexer.filename, token.start, ERR_LEXER, "Invalid token. Cannot use TT_ACCESS after '%s', '%s'", TOKEN_NAMES[tmp.type], tmp.text
				);
			}
			lexer.tokens[lexer.peek-1].type = TT_ACCESS;
			// token = merge_tokens(tmp, token);
			// print_token(token);
			add_token(&lexer, token, lexer.pos);
			reset_token(&token, lexer.pos);
			lexer.text_peek++;
			lexer.pos.column++;
		}

		else if (c == ':' && IS_VALID_SYMBOL_CHAR(lexer.text[lexer.text_peek+1])) {
			lexer.text_peek++;
			lexer.pos.column++;
			token.type = TT_ACCESS;
			token.text = "::";
			token.start = lexer.pos;
			add_token(&lexer, token, lexer.pos);
			reset_token(&token, lexer.pos);
		}

		else if (IS_WHITESPACE(c))
		{
			lex_ignore_whitespace(&lexer);
		}


		else if (c == '@')
		{
			str_add_char(&token, c);
			token.start = lexer.pos;
			token.text = "@";
			token.type = TT_MUTABLE;
			add_token(&lexer, token, lexer.pos);
			reset_token(&token, lexer.pos);
		}

		else if (c == '"')
		{
			// printf("before: %d.%d\n", lexer.pos.line, lexer.pos.column);
			lexer.text_peek++; // skip the first quote
			add_token(&lexer, lex_string(&lexer), lexer.pos);
			continue;
			// printf("after: %d.%d\n", lexer.pos.line, lexer.pos.column);
		}

		else if (IS_NUMBER(c)) {
			add_token(&lexer, lex_number(&lexer), lexer.pos);
		}

		else if (IS_VALID_SYMBOL_CHAR(c)) {
			add_token(&lexer, lex_word(&lexer), lexer.pos);
		}

		else {
			// printf("paren pos: %d %d\n", token.start.column, lexer.pos.column);
			// token.start = lexer.pos;
			str_add_char(&token, c);
			switch (c)
			{
				case '(':
					token.type = TT_LPAREN;
					b_count++;
				break;
				case ')':
					token.type = TT_RPAREN;
					b_count--;
					if (b_count < 0) {
						// lobotomy_error_s_ne(ERR_TOO_MANY_BRACKETS, "too many brackets at %d.%d", lexer.pos.line, lexer.pos.column);
						LOBOTOMY_ERROR_FULL_NO_EXIT(
							lexer_highlight_error_token(&lexer, token.start, current_line_index),
							lexer.filename, token.start, ERR_TOO_MANY_BRACKETS, "Too many parenthesis %s", token.text
						);
						b_count = 0;
						continue;
					}
				break;
				case '[':
					token.type = TT_LBRACKET;
				break;
				case ']':
					token.type = TT_RBRACKET;
					// if (b_count < 0) {
						// lobotomy_error_s_ne(ERR_TOO_MANY_BRACKETS, "too many brackets at %d.%d", line, column);
					// }
				break;
				case '{':
					token.type = TT_LCBRACKET;
				break;
				
				case '}':
					token.type = TT_RCBRACKET;
				break;

				default:
					// lobotomy_error_s_ne(ERR_UNKNOWN_CHARACTER, "ENCOUNTERED UNKNOWN CHARACTER '%c' at %d.%d", c, lexer.pos.line, lexer.pos.column);
					LOBOTOMY_ERROR_FULL(
						lexer_highlight_error_token(&lexer, lexer.pos, current_line_index),
						lexer.filename, lexer.pos, ERR_LEXER, "ENCOUNTERED UNKNOWN CHARACTER '%c'", c
					);
			}
			add_token(&lexer, token, token.start);
			add_token(&brackets, token, token.start);
			reset_token(&token, lexer.pos);
		}
		}

	// int b_i = 0, b_j = 0;
	// if (b_count > 0) {
		// for (; b_i < brackets.index; b_i++) {
			// if (brackets.tokens[b_i].type == TT_LPAREN) {
				// for (b_j = b_i + 1; b_j <= brackets.index; b_j++) {
					// if (brackets.tokens[b_j].type == TT_RPAREN) {
						// break;
					// }
					// lobotomy_error_s_ne(ERR_NO_MATCHING_BRACKET, "bracket at %d.%d is not closed", brackets.tokens[b_i].start.line, brackets.tokens[b_i].start.column);
				// }
			// }
		// }		
	// }
	// my_assert((b_count > 0), "bracket is not closed", ERR_NO_MATCHING_BRACKET, line, column);

	for (int i = 0; i < lexer.index; i++) {
		if (lexer.tokens[i].text == NULL)
			break;
		// print_token(lexer.tokens[i]);
    
		// printf("t: %d | text: %s | type: %d\n", i, lexer.tokens[i].text, lexer.tokens[i].type);
		// printf("%s ", lexer.tokens[i].text);
	}
	// printf("\n\n----TOKENIZER DONE----\n\n");


	// free(word);
	printd("text size; %d; tokens; %d\n", lexer.text_len, lexer.index);
	return lexer;
	// free(lexer.tokens);
}





