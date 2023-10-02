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



#define u64 uint64_t
#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t

#define i64 int64_t
#define i32 int32_t
#define i16 int16_t
#define i8 int8_t

char* read_file(const char path[])
{
	char* dst = "";
	FILE* file;
	file = fopen(path, "r");
	if (file == NULL) {
		lobotomy_error("failed to read file %s", path);
	}
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

int hash(const char* key)
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

