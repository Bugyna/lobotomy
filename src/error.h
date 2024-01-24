#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

enum {
	ERR_LEXER,
	ERR_PARSER,
	ERR_EVAL,
	ERR_RUNTIME,

	ERR_NO_MATCHING_BRACKET,
	ERR_TOO_MANY_BRACKETS,
	ERR_WORD_IN_NUMBER,
	ERR_INVALID_DECIMAL,
	ERR_INVALID_CHARACTER_IN_NUMBER,
	ERR_UNESCAPED_NEW_LINE_IN_STRING_LITERAL,
	ERR_UNKNOWN_CHARACTER,
	ERR_UNEXPECTED,
	ERR_UNDEFINED,
};

// #define my_assert(expr, err, err_code, line, column) if (expr) { printf("%s at %d.%d\n", err, line, column); exit(err_code); }


#define DEBUG(s, ...) printf(s __VA_ARGS__); printf("\n\n");
#define lobotomy_warning(...) DEBUG("warning: ", __VA_ARGS__)
#define LOBOTOMY_WARNING(...) DEBUG("warning: ", __VA_ARGS__)

#define lobotomy_error_ne(...) DEBUG("ERROR: ", __VA_ARGS__)
#define lobotomy_error_s_ne(n, ...) DEBUG(#n ": ", __VA_ARGS__)

#define lobotomy_error(...) DEBUG("ERROR: ", __VA_ARGS__); exit(-1)
#define lobotomy_error_s(n, ...) DEBUG(#n ": ", __VA_ARGS__); exit(n)

#define LOBOTOMY_ERROR lobotomy_error
#define LOBOTOMY_ERROR_S lobotomy_error_s

#define LOBOTOMY_ERROR_FULL(EXEC, FILE, POS, ERR, MSG, ...) \
printf("\n%s:%ld.%ld %s\n\t" MSG LINE_TERMINATOR, FILE, POS.line, POS.column, #ERR, __VA_ARGS__);\
EXEC;\
printf("\n\n"); exit(ERR);
