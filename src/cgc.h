#pragma once
#include <stdbool.h>
#include "obj.h"



typedef struct GC
{
	OBJ* memory_pool;
	OBJ* from_space;
	OBJ* to_space;
	size_t size;
	size_t half_size;
	int top, curr;
	OBJ* ptr;
	ENV* env;
};



// #ifdef _EXTERN_LIB
// extern GC* gcl;
// #else
GC* gcl;
static GC** _gcl;
// #endif

OBJ* GCL_alloc()
{
	if (gcl->top++ >= gcl->half_size) GCL_collect(gcl);
	gcl->ptr = &(gcl->from_space[gcl->top]);
	return gcl->ptr;
}

void GCL_init(ENV* env)
{
	gcl = calloc(1, sizeof(GC));
	gcl->size = 5000;
	gcl->half_size = gcl->size / 2;
	gcl->memory_pool = calloc(gcl->size, sizeof(OBJ*));
	gcl->from_space = &gcl->memory_pool[0];
	gcl->to_space = &gcl->memory_pool[gcl->half_size];
	gcl->top = 0;
	gcl->curr = 0;
	gcl->env = env;
}

OBJ* GCL_copy(OBJ* o)
{
	
}

int GCL_mark(GC* gcl, OBJ* root)
{
	int marked = 0;
	if (root->marked) goto ret;
	root->marked = true;
	marked++;
	if (root->type == T_LIST || root->type == T_EXPR)
	{
		ITERATE_OBJECT(root->car, cur)
		{
			marked += GCL_mark(gcl, cur);
		}
	}
	else if (root->type == T_FN)
	{
		marked += GCL_mark(gcl, root->body->car);
		marked += GCL_mark(gcl, root->args->car);
	}

	ret:
		return marked;
}


void GCL_collect(GC* gcl)
{
	for (size_t i = 0; i < gcl->env->size; i++)
	{
		if (gcl->env->list[i].val != NULL && gcl->env->list[i].key != NULL) {
			// print_objf("aaaaa: ", gcl->env->list[i].val);
			__ITERATE_HASHMAP(ENV, gcl->env, OBJ, gcl->env->list[i].key)
			{
				// val->marked = 1;
				printf("marked: %s %s ", BUCKET->key, type_name(val->type));
				printf("%d\n", GCL_mark(gcl, val));
			}
			// gcl->env->list[i].val->marked = 1;
		}
	}

	
}


int GCL_free()
{

	return 0;
}



