#pragma once
#include "util.h"

typedef struct STACK STACK;

struct STACK
{
	int ptr, max;
	OBJ* values;
};


void gc_alloc(STACK* stack, OBJ obj)
{
	// what
}

void gc_free(STACK* stack, OBJ* obj)
{
	// what
}

void gc_realloc(STACK* stack, OBJ* obj)
{
	
}

void gc_run(STACK* stack)
{
	for (int i = 0; i < stack->ptr; i++) {
		// do shit
	}
}