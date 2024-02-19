#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
	void* next;
	void* data;
} ARENA_NODE;

typedef struct 
{
	void* data;
	void* curr;
	size_t size, block_size;
	
} ARENA;



ARENA* arena_create(size_t size, size_t block_size)
{
	ARENA* a;
	a = malloc(sizeof(ARENA));
	a.data = calloc(size, block_size);
	a.curr = a.data;
	a.size = size;
	a.block_size = block_size;
	return a;
}

int arena_free(ARENA* a)
{
	free(a->data);
	free(a);
	return 1;
}


void* arena_alloc(size_t block_size)
{
	return 
}




