#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>

// #include "util.h"

#include "linked_list.h"
#include "SHM/hashmap.h"
// #include "hashmap.h"


typedef enum
{
	T_UNDEFINED,
	T_NIL,
	T_IDENTIFIER,
	T_TRUE,
	T_FALSE,
	T_EXPR,
	T_LIST,
	T_C_FN,
	T_FN,
	T_STR,
	T_NUM,
	T_DECIMAL,
	T_REF,
} OBJ_TYPE;


#define STRINGIFY(x) #x
#define NT(X) X->cdr
#define NEXT(X) X = X->cdr

const char* type_name(OBJ_TYPE i)
{
	switch (i)
	{
		case T_UNDEFINED: return STRINGIFY(T_UNDEFINED);
		case T_NIL: return STRINGIFY(T_NIL);
		case T_IDENTIFIER: return STRINGIFY(T_IDENTIFIER);
		case T_EXPR: return STRINGIFY(T_EXPR);
		case T_LIST: return STRINGIFY(T_LIST);
		case T_C_FN: return STRINGIFY(T_C_FN);
		case T_FN: return STRINGIFY(T_FN);
		case T_STR: return STRINGIFY(T_STR);
		case T_NUM: return STRINGIFY(T_NUM);
		case T_DECIMAL: return STRINGIFY(T_DECIMAL);
		case T_REF: return STRINGIFY(T_REF);
	}


	return "NO IDEA";
}

typedef struct FN FN;
typedef struct OBJ OBJ;
typedef struct CONS CONS;
typedef struct ENV ENV;



typedef OBJ* (*C_FUNC_DEC)(OBJ*);

struct FN
{
	OBJ* args;
	OBJ* body;
	ENV* env;
};

struct OBJ
{
	OBJ_TYPE type;
	char* name;

	union
	{
		int64_t num;
		double decimal;
		char* str;
		struct { OBJ* args; OBJ* body; };
		C_FUNC_DEC c_fn;
	};

	OBJ* car;
	OBJ* cdr;
	ENV* env;
};


static OBJ* NIL = &((OBJ){.type=T_NIL});
static OBJ* O_TRUE = &((OBJ){.type=T_TRUE});
static OBJ* O_FALSE = &((OBJ){.type=T_FALSE});

#define ITERATE_OBJECT(O, curr)\
for(OBJ* curr = O; curr != NULL && curr->type != T_UNDEFINED && curr->type != T_NIL; curr = curr->cdr)


#define ITERATE_OBJECT_PTR(O, curr)\
for(OBJ** curr = &O; curr != NULL && *curr != NULL && (*curr)->type != T_UNDEFINED && (*curr)->type != T_NIL; curr = &(*curr)->cdr)


#define ITERATE_OBJECT_PAIR(O, curr, curr1)\
for(OBJ *curr = O, *curr1 = NT(O); curr1 != NULL && curr1->type != T_UNDEFINED && curr1->type != T_NIL; curr = NT(curr1), curr1 = NT(curr))



#define ZIP_ITERATE_OBJECT(O, P, curr, curr1, exprs)\
	OBJ* curr = O;\
	OBJ* curr1 = P;\
	for(; curr != NULL && curr->type != T_UNDEFINED && curr->type != T_NIL && curr1 != NULL && curr1->type != T_UNDEFINED && curr1->type != T_NIL; curr = curr->cdr, curr1 = curr1->cdr)\
	{\
		exprs\
}\


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

		case T_NUM:
			// printf("[%s %s: %ld]", type_name(o->type), o->name, o->num);
			printf("%ld", o->num);
		break;

		case T_DECIMAL:
			// printf("[%s %s: %f]", type_name(o->type), o->name, o->decimal);
			printf("%f", o->decimal);
		break;

		case T_IDENTIFIER:
			printf("[%s %s] = ", type_name(o->type), o->name);
			if (o->car != NULL) __print_obj_simple(o->car);
			else printf("#NIL#");
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

	}
}

void print_obj_simple(OBJ* o)
{

	__print_obj_simple(o);
	printf("\n");
}


#define print_objf(fmt, o, ...) printf(fmt __VA_ARGS__); print_obj_simple(o);

void __print_obj_full(OBJ* o)
{
	printf("[%s <%s>] (", type_name(o->type), o->name);
	ITERATE_OBJECT(o->cdr, curr)
	{
		printf(" .\n\t");
		__print_obj_simple(curr);
	}
	printf("\n)");
}

DEFINE_HASHMAP(ENV, OBJ, char* name; u64 id; ENV* parent;)
DEFINE_LINKED_LIST(OBJ_LIST, OBJ)

static ENV* global_env;


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

typedef struct
{
	OBJ** roots[100];
	OBJ* heap;
	OBJ* top;
	size_t allocated;
	
} GC;

GC gc;


void GC_init()
{
	gc.top = gc.heap = malloc(1000*sizeof(OBJ));
	gc.allocated = 0;
}

OBJ* GC_alloc()
{
	gc.allocated++;
	// printf("allocated: %d\n", gc.allocated);
	#ifdef GC_ENABLED
		// printf("alloc: %d\n", gc.top - gc.heap);
		return gc.top++;
	#else
		return calloc(1, sizeof(OBJ));
	#endif
}

void GC_free(OBJ* o)
{
	#ifndef GC_ENABLED
	free(o);
	#endif
}


#define NEW() GC_alloc()
// #define NEW() calloc(1, sizeof(OBJ))
OBJ* empty_obj()
{
	
	OBJ* ret = NEW();
	ret->type = T_NIL;
	ret->env = global_env;
	return ret;
}


OBJ* empty_obj_t(OBJ_TYPE type)
{
	
	OBJ* ret = NEW();
	ret->type = type;
	ret->env = global_env;
	return ret;
}


OBJ* create_cfn(const char* name, C_FUNC_DEC fn)
{
	OBJ* o = empty_obj();
	o->type = T_C_FN;
	o->name = name;
	o->c_fn = fn;
	return o;
}

