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
	int occupied;
	int top, curr;
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
	gcl->occupied = 0;
	gcl->top = 0;
	gcl->curr = 0;
	gcl->env = env;
}

OBJ* GCL_alloc()
{
	// gcl->top += sizeof(OBJ);
	gcl->ptr = &gcl->memory_pool[gcl->top++];
	gcl->occupied++;
	return gcl->ptr;
}

int GCL_mark(OBJ* root)
{
	int marked = 0;
	if (root->marked) goto ret;
	root->marked = true;
	marked++;
	if (root->type == T_LIST || root->type == T_EXPR)
	{
		ITERATE_OBJECT(root, cur)
		{
			marked += GCL_mark(cur);
		}
		ITERATE_OBJECT(root->car, cur)
		{
			marked += GCL_mark(cur);
		}
	}
	else if (root->type == T_FN)
	{
		// GCL_mark(root->body);
		ITERATE_OBJECT(root->body->car, cur)
		{
			marked += GCL_mark(cur);
		}
		GCL_mark(root->args);
		ITERATE_OBJECT(root->args->car, cur)
		{
			marked += GCL_mark(cur);
		}
	}

	ret:
		// printf("Collected: %d\n", marked);
		return marked;
}


int GCL_collect()
{
	for (size_t i = 0; i < gcl->env->size; i++)
	{
		if (gcl->env->list[i].val != NULL && gcl->env->list[i].key != NULL) {
			// print_objf("aaaaa: ", gcl->env->list[i].val);
			__ITERATE_HASHMAP(ENV, gcl->env, OBJ, gcl->env->list[i].key)
			{
				// val->marked = 1;
				printf("marked: %s %s ", BUCKET->key, type_name(val->type));
				printf("%d\n", GCL_mark(val));
			}
			// gcl->env->list[i].val->marked = 1;
		}
	}

	// for (int i = 0; i < gcl->size; i++)
	// {
		// if (!gcl->memory_pool[i].marked) gcl->
	// }

	int tmp_top = gcl->curr;
	// gcl->top = 0;
	int collected = 0;
	printf("gcl-top: %d\n", gcl->occupied);
	for (int i = 0; i < tmp_top; i++)
	{
		// printf("aa: %d %s\n", i, type_name(gcl->memory_pool[i].type));
		if (gcl->memory_pool[i].type >= T_NIL && gcl->memory_pool[i].type <= T_OTHER && !gcl->memory_pool[i].marked) {
			gcl->occupied--;

			if (gcl->memory_pool[i].name != NULL)
				printf("sweep: %s %s\n", gcl->memory_pool[i].name, type_name(gcl->memory_pool[i].type));
			else
				print_objf("sweep:", &gcl->memory_pool[i]);
			printf("aa: %d %s %d\n", i, type_name(gcl->memory_pool[i].type), gcl->memory_pool[i].type);
			collected++;
			reset_obj(&gcl->memory_pool[i]);
			// gcl->memory_pool[i].type = T_UNDEFINED;
			// print_objf("aa", gcl->memory_pool[i]);
		}
		gcl->memory_pool[i].marked = false;
	}
	printf("gc: collected: %d\n", collected);
	printf("gcl-top: %d\n", gcl->occupied);
	return 1;
}



int GCL_free()
{

	return 0;
}


