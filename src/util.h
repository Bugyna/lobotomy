#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdint.h>

#include "error.h"


#define my_assert(expr, err, err_code, line, column) if (expr) { printf("%s at %d.%d\n", err, line, column); exit(err_code); }
#define _DEBUG 0


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
	// my_assert((key == NULL), 
	int mask = 1 << strlen(key);
	for (unsigned int i = 0; i < strlen(key); i++) {
		mask += (int)key[i];
	}
	return mask ^ (mask >> 4);
}