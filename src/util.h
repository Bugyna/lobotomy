#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdint.h>
#include <time.h>

#include "error.h"


#define my_assert(expr, err, err_code, line, column) if (expr) { printf("%s at %d.%d\n", err, line, column); exit(err_code); }


#define DEBUG(s, ...) printf(s __VA_ARGS__); printf("\n\n");
#define lobotomy_warning(...) DEBUG("warning: ", __VA_ARGS__)

#define lobotomy_error_ne(...) DEBUG("ERROR: ", __VA_ARGS__)
#define lobotomy_error_s_ne(n, ...) DEBUG(#n ": ", __VA_ARGS__)

#define lobotomy_error(...) DEBUG("ERROR: ", __VA_ARGS__); exit(-1)
#define lobotomy_error_s(n, ...) DEBUG(#n ": ", __VA_ARGS__); exit(n)


char* read_file(const char path[])
{
	char* dst = "";
	FILE* file;
	file = fopen(path, "r");
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	dst = malloc(size+2);
	fseek(file, 0, SEEK_SET);


	int i = 0, c;
	for (;;i++) {
		c = fgetc(file);
		if (c == EOF)
			break;

		dst[i] = c;

	}
	dst[i] = '\0';
	fclose(file);
	return dst;
	// printf("s: %s\n", dst);
	
}

int hash(char* key)
{
	// my_assert(key == NULL, "fuckery in hash", -1, 0, 0);
	if (key == NULL) {
		lobotomy_error("trying to hash an empty key");
	}

	int mask = 1 << strlen(key);
	for (unsigned int i = 0; i < strlen(key); i++) {
		mask += (int)key[i];
	}
	return mask ^ (mask >> 4);
}

