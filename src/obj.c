#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "obj.h"
#include "gc.h"



const char* type_name(OBJ_TYPE i)
{
	#define STRINGIFY_TYPENAME(t) case t: return #t;
	switch (i)
	{
		STRINGIFY_TYPENAME(T_UNDEFINED);
		// case T_UNDEFINED: return STRINGIFY(T_UNDEFINED);
		case T_NIL: return STRINGIFY(T_NIL);
		case T_IDENTIFIER: return STRINGIFY(T_IDENTIFIER);
		STRINGIFY_TYPENAME(T_MAP);
		case T_EXPR: return STRINGIFY(T_EXPR);
		case T_LIST: return STRINGIFY(T_LIST);
		case T_C_FN: return STRINGIFY(T_C_FN);
		case T_FN: return STRINGIFY(T_FN);
		case T_STR: return STRINGIFY(T_STR);
		case T_NUM: return STRINGIFY(T_NUM);
		case T_DECIMAL: return STRINGIFY(T_DECIMAL);
		case T_REF: return STRINGIFY(T_REF);
		STRINGIFY_TYPENAME(T_TRUE);
		STRINGIFY_TYPENAME(T_FALSE);
		STRINGIFY_TYPENAME(T_OTHER);
		STRINGIFY_TYPENAME(T_FLOAT);
		STRINGIFY_TYPENAME(T_C_INT32);
		STRINGIFY_TYPENAME(T_C_UINT32);
		STRINGIFY_TYPENAME(T_VOID_PTR);
		
	}


	return "NO IDEA";
}


struct FN
{
	OBJ* args;
	OBJ* body;
	ENV* env;
};



static const OBJ DEFAULT_OBJ = ((OBJ){.type=T_UNDEFINED});
static OBJ* NIL = &((OBJ){.type=T_NIL});
static OBJ* O_TRUE = &((OBJ){.type=T_TRUE, .name="TRUE"});
static OBJ* O_FALSE = &((OBJ){.type=T_FALSE});


// DEFINE_HASHMAP(ENV, OBJ, char* name; u64 id; ENV* parent;)
DEFINE_LINKED_LIST(OBJ_LIST, OBJ)

static ENV* global_env;

void __print_obj_simple(OBJ* o)
{
	switch (o->type)
	{
		case T_UNDEFINED:
			// printf("[%s %s: UNDEFINED]", type_name(o->type), o->name);
			printf("!UNDEFINED!");
		break;

		case T_NIL:
			// printf("[%s %s: UNDEFINED]", type_name(o->type), o->name);
			printf("#NIL#");
		break;

		case T_MAP:
			for (int i = 0; i < o->map->size; i++) {
				if (o->env->list[i].key == NULL) continue;
				__ITERATE_HASHMAP(ENV, o->map, OBJ, o->map->list[i].key)
				{
					printf("::> '%s'\n", BUCKET->key);
				}
				// if (o->map->list[i].key != NULL)
					// printf("::> '%s'\n", o->map->list[i].key);
			}
			// printf("MAP");
		break;

		case T_TRUE: case T_FALSE:
			printf(type_name(o->type));
		break;

		case T_NUM:
			// printf("[%s %s: %ld]", type_name(o->type), o->name, o->num);
			printf("%ld", o->num);
		break;

		case T_DECIMAL:
			// printf("[%s %s: %f]", type_name(o->type), o->name, o->decimal);
			printf("%f", o->decimal);
		break;

		case T_IDENTIFIER:
			printf("[%s %s]", type_name(o->type), o->name);
		break;

		case T_REF:
			printf("[%s %s] = ", type_name(o->type), o->name);
			if (o->car != NULL) __print_obj_simple(o->car);
			else printf("#NIL#");
		break;

		case T_STR:
			// printf("[%s %s: %s]", type_name(o->type), o->name, o->str);
			printf("%s", o->str);
		break;

		case T_LIST: case T_EXPR:
			printf("%s <%s>: ( ", type_name(o->type), o->name);
			// printf("[%s <%s>]", type_name(o->car->type), o->car->name);
			__print_obj_simple(o->car);
			ITERATE_OBJECT_PTR(o->car->cdr, curr)
			{
				printf(" . ");
				__print_obj_simple(*curr);
			}
			// printf(" )\n\t( ");

			// ITERATE_OBJECT(o->cdr, curr)
			// {
				// printf(" . ");
				// __print_obj_simple(curr);
			// }
			printf(" )\n");
		break;

		case T_C_FN:
			printf("[%s %s]", type_name(o->type), o->name);
		break;

		case T_FN:
			printf("[%s %s]", type_name(o->type), o->name);
			// ITERATE_OBJECT(o->fn->args, curr)
			// {
				// printf(" . ");
				// __print_obj_simple(curr);
			// }
		break;

		default:
			printf("what the fuck %d", o->type);

	}
}


void __print_obj_expand(OBJ* o)
{
	switch (o->type)
	{
		case T_UNDEFINED:
			// printf("[%s %s: UNDEFINED]", type_name(o->type), o->name);
			printf("!UNDEFINED!");
		break;

		case T_NIL:
			// printf("[%s %s: UNDEFINED]", type_name(o->type), o->name);
			printf("#NIL#");
		break;

		case T_MAP:
			for (int i = 0; i < o->map->size; i++) {
				if (o->env->list[i].key == NULL) continue;
				__ITERATE_HASHMAP(ENV, o->map, OBJ, o->map->list[i].key)
				{
					printf("::> '%s'\n", BUCKET->key);
				}
				// if (o->map->list[i].key != NULL)
					// printf("::> '%s'\n", o->map->list[i].key);
			}
			// printf("MAP");
		break;

		case T_TRUE: case T_FALSE:
			printf(type_name(o->type));
		break;

		case T_NUM:
			// printf("[%s %s: %ld]", type_name(o->type), o->name, o->num);
			printf("%ld", o->num);
		break;

		case T_DECIMAL:
			// printf("[%s %s: %f]", type_name(o->type), o->name, o->decimal);
			printf("%f", o->decimal);
		break;

		case T_IDENTIFIER:
			printf("[%s %s]", type_name(o->type), o->name);
		break;

		case T_REF:
			printf("[%s %s] = ", type_name(o->type), o->name);
			if (o->car != NULL) __print_obj_simple(o->car);
			else printf("#NIL#");
		break;

		case T_STR:
			// printf("[%s %s: %s]", type_name(o->type), o->name, o->str);
			printf("%s", o->str);
		break;

		case T_LIST: case T_EXPR:
			printf("%s <%s>: ( ", type_name(o->type), o->name);
			// printf("[%s <%s>]", type_name(o->car->type), o->car->name);
			__print_obj_simple(o->car);
			ITERATE_OBJECT_PTR(o->car->cdr, curr)
			{
				printf(" . ");
				__print_obj_simple(*curr);
			}
			// printf(" )\n\t( ");

			// ITERATE_OBJECT(o->cdr, curr)
			// {
				// printf(" . ");
				// __print_obj_simple(curr);
			// }
			printf(" )\n");
		break;

		case T_C_FN:
			printf("[%s %s]", type_name(o->type), o->name);
		break;

		case T_FN:
			printf("[%s %s]\n", type_name(o->type), o->name);
			printf("ARGS: (");
			ITERATE_OBJECT(o->args->car, curr)
			{
				printf(" . ");
				__print_obj_simple(curr);
			}
			printf(" )\n");
			
			printf("BODY: (");
			ITERATE_OBJECT(o->body->car, curr)
			{
				printf(" . ");
				__print_obj_simple(curr);
			}
			printf(" )\n");
		break;

		default:
			printf("what the fuck %d", o->type);

	}
}

void print_obj_simple(OBJ* o)
{

	__print_obj_simple(o);
	printf("\n");
}


inline void print_obj_expand(OBJ* o)
{
	__print_obj_expand(o);
	printf("\n");
}


inline void __print_obj_full(OBJ* o)
{
	// printf("[%s <%s>] (", type_name(o->type), o->name);
	ITERATE_OBJECT(o, curr)
	{
		// printf(" .\n\t");
		__print_obj_simple(curr);
		printf(" ");
	}
}



inline void print_obj_full(OBJ* o)
{
	__print_obj_full(o);
	printf("\n");
}


void env_add(ENV* e, OBJ* o)
{
	ENV_ADD(e, o->name, o);
}


OBJ* env_get(ENV* e, const char* key)
{
	for (ENV* tmp_env = e; tmp_env != NULL; tmp_env = tmp_env->parent)
	{
		// printf("searching in: %s for %s\n", tmp_env->name, key);
		OBJ* tmp = ENV_GET(tmp_env, key);
		if (tmp != NULL) return tmp;
	}

	return NULL;
}


// #define GC_ENABLED


// struct GC
// {
	// OBJ** roots[100];
	// OBJ* heap;
	// OBJ* top;
	// size_t allocated;
	
// };

// static GC gc;


// void GC_init()
// {
	// gc.top = gc.heap = malloc(1000*sizeof(OBJ));
	// gc.allocated = 0;
// }

// OBJ* GC_alloc()
// {
	// gc.allocated++;
	// // printf("allocated: %d\n", gc.allocated);
	// #ifdef GC_ENABLED
		// // printf("alloc: %d\n", gc.top - gc.heap);
		// return gc.top++;
	// #else
		// return calloc(1, sizeof(OBJ));
	// #endif
// }

// void GC_free(OBJ* o)
// {
	// #ifndef GC_ENABLED
	// free(o);
	// #endif
// }


#define NEW() GCL_alloc()
// #define NEW() calloc(1, sizeof(OBJ))




void reset_obj(OBJ* o)
{
	*o = DEFAULT_OBJ;
}

OBJ* empty_obj()
{
	
	OBJ* ret = NEW();
	ret->type = T_NIL;
	ret->env = global_env;
	ret->marked = false;
	ret->car = NULL;
	return ret;
}


OBJ* empty_obj_t(OBJ_TYPE type)
{
	
	OBJ* ret = NEW();
	ret->type = type;
	ret->env = global_env;
	ret->marked = false;
	ret->car = NULL;
	return ret;
}


OBJ* create_cfn(char* name, C_FUNC_DEC fn)
{
	OBJ* o = empty_obj();
	o->type = T_C_FN;
	o->name = name;
	o->c_fn = fn;
	o->car = NULL;
	// o->marked = false;
	return o;
}
