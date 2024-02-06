#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

typedef int (*_print_func)(const char* restrict, ...);
_print_func _printf = &printf;
#define printf _printf

#include "util.h"
#include "SHM/hashmap.h"
#include "linked_list.h"
#include "io.h"


#define OBJ_FN_ARGS int argc, OBJ* o


typedef enum
{
	T_UNDEFINED, /*! T_UNDEFINED */
	T_NIL, /*! NIL/NULL value */
	T_IDENTIFIER, /*! NIL/NULL value */
	T_TRUE,
	T_FALSE,
	T_EXPR,
	T_LIST,
	T_MAP,
	T_C_FN,
	T_FN,
	T_STR,
	T_NUM,
	T_DECIMAL,
	T_REF,
	T_FILE,

	// C Types that are different from the types above
	T_C_INT32,
	T_C_UINT32,
	T_FLOAT,
	T_VOID_PTR,

	// looks info up
	T_OTHER,
	
} OBJ_TYPE;

#define STRINGIFY(x) #x
#define NT(X) X->cdr
#define NEXT(X) X = X->cdr

const char* type_name(OBJ_TYPE i);

typedef struct FN FN;
typedef struct OBJ OBJ;
typedef struct CONS CONS;
typedef struct ENV ENV;
// typedef struct GC GC;
typedef OBJ* (*C_FUNC_DEC)(OBJ_FN_ARGS);


struct OBJ
/*!
	does stuff
*/
{
	u8 marked;
	OBJ_TYPE type;
	char* name;

	union
	{
		int64_t num;
		double decimal;
		char* str;
		struct { OBJ* args; OBJ* body; };
		C_FUNC_DEC c_fn;
		ENV* map;
		int len;
		void* data;
		FILE* file;
	};

	OBJ* car;
	OBJ* cdr;
	ENV* env;
	const char* env_name;
};

static OBJ* NIL;
static OBJ* O_TRUE;
static OBJ* O_FALSE;

static const OBJ DEFAULT_OBJ = {.type=T_UNDEFINED};
static OBJ* NIL = &((OBJ){.type=T_NIL});
static OBJ* O_TRUE = &((OBJ){.type=T_TRUE, .name="TRUE"});
static OBJ* O_FALSE = &((OBJ){.type=T_FALSE});

DEFINE_HASHMAP(ENV, OBJ, char* name; u64 id; ENV* parent;)

typedef void(*load_lib_func)(ENV*);

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


void __print_obj_simple(OBJ* o);
void print_obj_simple(OBJ* o);
#define print_of(o, ...) printf(__VA_ARGS__); print_obj_simple(o);
#define print_objf(fmt, o, ...) printf(fmt __VA_ARGS__); print_obj_simple(o);
#define print_objff(fmt, o, ...) printf(fmt __VA_ARGS__); print_obj_full(o);
void __print_obj_full(OBJ* o);

void env_add(ENV* e, OBJ* o);
OBJ* env_get(ENV* e, const char* key);


void GC_init();
OBJ* GC_alloc();
void GC_free(OBJ* o);


void reset_obj(OBJ* o);
OBJ* empty_obj();
OBJ* empty_obj_t(OBJ_TYPE type);
OBJ* create_cfn(char* name, C_FUNC_DEC fn);


#define L_CHECK_ARITY(ARGC, N) if (ARGC != -1 && ARGC != N) { LOBOTOMY_ERROR_S(ERR_ARITY_MISMATCH, "Expected '%d' arguments, but got '%d'", ARGC, N); }

#define L_DEF_C_FN(NAME, ARITY, DOC, EXEC)\
OBJ* NAME(OBJ_FN_ARGS)\
{\
	/*! ## DOC ## */\
	L_CHECK_ARITY(ARITY, argc)\
	EXEC\
}\


