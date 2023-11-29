#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// #include "util.h"

#include "linked_list.h"
#include "../../SHM/hashmap.h"
// #include "hashmap.h"


typedef enum
{
	T_UNDEFINED,
	T_NIL,
	T_IDENTIFIER,
	T_EXPR,
	T_LIST,
	T_C_FN,
	T_FN,
	T_STR,
	T_NUM,
	T_DECIMAL,
	
} OBJ_TYPE;


#define STRINGIFY(x) #x

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
	}


	return "NO IDEA";
}

typedef struct FN FN;
typedef struct OBJ OBJ;
typedef struct CONS CONS;
typedef struct ENV ENV;



typedef OBJ* (*C_FUNC_DEC)(OBJ*, ...);


struct OBJ
{
	OBJ_TYPE type;
	char* name;

	union
	{
		int64_t num;
		double decimal;
		char* str;
		FN* fn;
		C_FUNC_DEC c_fn;
		OBJ* car;
	};

	OBJ* cdr;
};



#define ITERATE_OBJECT(O, curr)\
for(OBJ* curr = O; curr != NULL && curr->type != T_UNDEFINED && curr->type != T_NIL; curr = curr->cdr)


#define ITERATE_OBJECT_PTR(O, curr)\
for(OBJ** curr = &O; *curr != NULL && (*curr)->type != T_UNDEFINED && (*curr)->type != T_NIL; curr = &(*curr)->cdr)



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
			printf("[%s %s]", type_name(o->type), o->name);
		break;

		case T_STR:
			// printf("[%s %s: %s]", type_name(o->type), o->name, o->str);
			printf("\"%s\"", o->str);
		break;

		case T_LIST: case T_EXPR:
			printf("%s <%s>: ( ", type_name(o->type), o->name);
			// printf("[%s <%s>]", type_name(o->car->type), o->car->name);
			__print_obj_simple(o->car);
			ITERATE_OBJECT(o->car->cdr, curr)
			{
				printf(" . ");
				__print_obj_simple(curr);
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
		break;

	}
}

void print_obj_simple(OBJ* o)
{

	__print_obj_simple(o);
	printf("\n");
}

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

ENV global_env;

struct FN
{
	OBJ* body;
	OBJ* args;
	ENV* env;
};


void env_add(ENV* e, OBJ* o)
{
	ENV_ADD(e, o->name, o);
}

#define NEW() calloc(1, sizeof(OBJ))
OBJ* empty_obj()
{
	
	OBJ* ret = NEW();
	ret->type=T_NIL;
	// ret->name = malloc(5);
	// ret->name = "2";
	// return ret;
	return ret;
}

