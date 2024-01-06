#pragma once
#include <stdio.h>
#include <stdlib.h>

enum {
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

#define my_assert(expr, err, err_code, line, column) if (expr) { printf("%s at %d.%d\n", err, line, column); exit(err_code); }


#define DEBUG(s, ...) printf(s __VA_ARGS__); printf("\n\n");
#define lobotomy_warning(...) DEBUG("warning: ", __VA_ARGS__)
#define LOBOTOMY_WARNING(...) DEBUG("warning: ", __VA_ARGS__)

#define lobotomy_error_ne(...) DEBUG("ERROR: ", __VA_ARGS__)
#define lobotomy_error_s_ne(n, ...) DEBUG(#n ": ", __VA_ARGS__)

#define lobotomy_error(...) DEBUG("ERROR: ", __VA_ARGS__); exit(-1)
#define lobotomy_error_s(n, ...) DEBUG(#n ": ", __VA_ARGS__); exit(n)
