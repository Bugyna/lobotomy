#pragma once
#include <stdbool.h>
#include "obj.h"



typedef enum
{
	GCLS_NOTHING=0,
	GCLS_MARKING,
	GCLS_COLLECTING,
} GCL_STATE;

struct GC
{
	GCL_STATE state;
	OBJ* memory_pool;
	OBJ* ptr;
	size_t size;
	int occupied;
	int top, curr;
	ENV* env;
};

#include "tgc.h"

// void GCL_init(GC* gcl, ENV* env);
// OBJ* GCL_alloc();
// int GCL_mark(GC* gcl, OBJ* root);
// int GCL_collect(GC* gcl);
// int GCL_free(GC* gcl);

// #include "tgc.h"

