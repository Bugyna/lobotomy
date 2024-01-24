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
#include <wchar.h>


#include "os.h"

typedef void (*_lobotomy_exit_func)(int n);
_lobotomy_exit_func _lobotomy_exit = &exit;
_lobotomy_exit_func _real_exit = &exit;

#define exit _lobotomy_exit
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


#if DEBUGGING == 1
#define printd(...) printf(__VA_ARGS__)
#else
#define printd(...)
#endif

#define HASHMAP_HASH_INIT 2166136261u

static inline uint32_t nhash(const char* data)
{
	size_t size = strlen(data);
	size_t nblocks = size / 8;
	uint64_t hash = HASHMAP_HASH_INIT;
	for (size_t i = 0; i < nblocks; ++i)
	{
		hash ^= (uint64_t)data[0] << 0 | (uint64_t)data[1] << 8 |
			 (uint64_t)data[2] << 16 | (uint64_t)data[3] << 24 |
			 (uint64_t)data[4] << 32 | (uint64_t)data[5] << 40 |
			 (uint64_t)data[6] << 48 | (uint64_t)data[7] << 56;
		hash *= 0xbf58476d1ce4e5b9;
		data += 8;
	}

	uint64_t last = size & 0xff;
	switch (size % 8)
	{
	case 7:
		last |= (uint64_t)data[6] << 56; /* fallthrough */
	case 6:
		last |= (uint64_t)data[5] << 48; /* fallthrough */
	case 5:
		last |= (uint64_t)data[4] << 40; /* fallthrough */
	case 4:
		last |= (uint64_t)data[3] << 32; /* fallthrough */
	case 3:
		last |= (uint64_t)data[2] << 24; /* fallthrough */
	case 2:
		last |= (uint64_t)data[1] << 16; /* fallthrough */
	case 1:
		last |= (uint64_t)data[0] << 8;
		hash ^= last;
		hash *= 0xd6e8feb86659fd93;
	}

	// compress to a 32-bit result.
	// also serves as a finalizer.
	return (uint32_t)(hash ^ hash >> 32);
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
	return (mask ^ (mask >> 4));
}



