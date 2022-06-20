#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "error.h"


#define my_assert(expr, err, err_code) if (expr) { printf("%s\n", err); exit(err_code); }


const char* read_file(const char path[])
{
	char* dst = "";
	FILE* file;
	file = fopen(path, "r");
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	dst = malloc(size+1);
	fseek(file, 0, SEEK_SET);


	int i = 0, c;
	for (;;i++) {
		c = fgetc(file);
		if (c == EOF)
			break;

		dst[i] = c;

	}
	fclose(file);
	return dst;
	// printf("s: %s\n", dst);
	
}