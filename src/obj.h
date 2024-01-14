#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>

typedef void (*_print_func)(const char* fmt, ...);
_print_func _printf = &printf;
#define printf _printf

#include "util.h"
#include "SHM/hashmap.h"
#include "linked_list.h"



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

const char* type_name(OBJ_TYPE i);

typedef struct FN FN;
typedef struct OBJ OBJ;
typedef struct CONS CONS;
typedef struct ENV ENV;
typedef struct GC GC;
typedef OBJ* (*C_FUNC_DEC)(OBJ*);

static OBJ* NIL;
static OBJ* O_TRUE;
static OBJ* O_FALSE;

typedef void (*load_lib_func)(ENV*);

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
#define print_objf(fmt, o, ...) printf(fmt __VA_ARGS__); print_obj_simple(o);
void __print_obj_full(OBJ* o);

void env_add(ENV* e, OBJ* o);
OBJ* env_get(ENV* e, const char* key);


void GC_init();
OBJ* GC_alloc();
void GC_free(OBJ* o);
#define NEW() GC_alloc()
OBJ* empty_obj();
OBJ* empty_obj_t(OBJ_TYPE type);
OBJ* create_cfn(const char* name, C_FUNC_DEC fn);

