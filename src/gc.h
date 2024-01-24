#pragma once
#include <stdbool.h>
#include "obj.h"



typedef enum
{
	GCLS_NOTHING=0,
	GCLS_MARKING,
	GCLS_COLLECTING,
} GCL_STATE;

typedef struct
{
	GCL_STATE state;
	OBJ* memory_pool;
	OBJ* ptr;
	size_t size;
	int top;
	ENV* env;
} GC;


GC* gcl;

void GCL_init(ENV* env)
{
	gcl = calloc(1, sizeof(GC));
	gcl->state = GCLS_NOTHING;
	gcl->size = 5000;
	gcl->memory_pool = calloc(gcl->size, sizeof(OBJ*));
	gcl->ptr = &gcl->memory_pool[0];
	gcl->top = 0;
	gcl->env = env;
}

OBJ* GCL_alloc()
{
	// gcl->top += sizeof(OBJ);
	gcl->ptr = &gcl->memory_pool[gcl->top++];
	return gcl->ptr;
}


int GCL_collect()
{
	for (int i = 0; i < gcl->env->size; i++)
	{
		if (&gcl->env->list[i] != NULL && gcl->env->list[i].val != NULL && gcl->env->list[i].key != NULL) {
			// print_objf("aaaaa: ", gcl->env->list[i].val);
			gcl->env->list[i].val->marked = 1;
			// printf("aa: %s %s\n", gcl->env->list[i].key, type_name(gcl->env->list[i].val->type));
		}
	}

	// for (int i = 0; i < gcl->size; i++)
	// {
		// if (!gcl->memory_pool[i].marked) gcl->
	// }

	gcl->top = 0;
	for (int i = 0; i < gcl->size; i++)
	{
		// printf("aa: %d %s\n", i, type_name(gcl->memory_pool[i].type));
		if (gcl->memory_pool[i].type != T_UNDEFINED && gcl->memory_pool[i].type <= T_OTHER && gcl->memory_pool[i].marked) printf("aa: %d %s %d\n", i, type_name(gcl->memory_pool[i].type), gcl->memory_pool[i].type);
	}
}

int GCL_free()
{
	
}


