#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "linked_list.h"
#include "hashmap.h"


typedef enum
{
	T_UNDEFINED,
	T_IDENTIFIER,
	T_LIST,
	T_C_FN,
	T_FN,
	T_STR,
	T_NUM,
	T_DECIMAL,
	
} OBJ_TYPE;

typedef struct FN FN;
typedef struct OBJ OBJ;
typedef struct ENV ENV;


struct OBJ
{
	OBJ_TYPE type;
	char* name;

	union
	{
		int64_t num;
		double decimal;
		char* str;
		OBJ* list;
		FN* fn;
		ENV* scope;
	};
};


DEFINE_HASHMAP(ENV, OBJ)
DEFINE_LINKED_LIST(OBJ, OBJ)

ENV global_env;

struct FN
{
	OBJ* body;
	OBJ* args;
	ENV* env;
};


