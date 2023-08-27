#pragma once
#include "error.h"


#define ITERATE_LIST_PTR(L_TYPE, L_NAME, NAME)\
for (L_TYPE##_NODE* NAME = L_NAME->first; NAME != NULL && NAME->val != NULL; NAME = NAME->next)




#define ITERATE_LINKED_LIST(LIST_TYPE, LIST, VAL_TYPE)\
LIST_TYPE##_NODE* NODE = LIST_TYPE##_GET_NODE(LIST);\
VAL_TYPE* val = NULL;\
if (NODE != NULL) val = NODE->val;\
for (; NODE != NULL && (val = NODE->val) != NULL; NODE = NODE->next)


#define ITERATE_LINKED_LIST_INDEX(LIST_TYPE, LIST, VAL_TYPE, N)\
LIST_TYPE##_NODE* NODE = LIST_TYPE##_GET_NODE_AT_INDEX(LIST, N);\
VAL_TYPE* val = NULL;\
if (NODE != NULL) val = NODE->val;\
for (; NODE != NULL && (val = NODE->val) != NULL; NODE = NODE->next)



#define ITERATE_LINKED_LIST_VN(LIST_TYPE, LIST, VAL_TYPE, VAL_NAME)\
LIST_TYPE##_NODE* NODE = LIST_TYPE##_GET_NODE(LIST);\
VAL_TYPE* VAL_NAME = NULL;\
if (NODE != NULL) VAL_NAME = NODE->val;\
for (; NODE != NULL && (VAL_NAME = NODE->val) != NULL; NODE = NODE->next)


#define ITERATE_LINKED_LIST_INDEX_VN(LIST_TYPE, LIST, VAL_TYPE, VAL_NAME, N)\
LIST_TYPE##_NODE* NODE = LIST_TYPE##_GET_NODE_AT_INDEX(LIST, N);\
VAL_TYPE* VAL_NAME = NULL;\
if (NODE != NULL) VAL_NAME = NODE->val;\
for (; NODE != NULL && (VAL_NAME = NODE->val) != NULL; NODE = NODE->next)

// TODO: Same functions but without custom name



#define DEFINE_LINKED_LIST(NAME, VAL_TYPE)\
typedef struct NAME##_NODE NAME##_NODE;\
typedef struct NAME NAME;\
struct NAME##_NODE\
{\
	VAL_TYPE* val;\
	NAME##_NODE* prev;\
	NAME##_NODE* next;\
};\
struct NAME\
{\
	int length;\
	NAME##_NODE* first;\
	NAME##_NODE* last;\
};\
void NAME##_APPEND_VAL(NAME* l, VAL_TYPE val)\
{\
	if (l->last->val == NULL) {\
		l->last->val = malloc(sizeof(VAL_TYPE));\
		*l->last->val = val;\
		l->last->next = NULL;\
		return;\
	}\
	NAME##_NODE* tmp = malloc(sizeof(NAME##_NODE));\
	tmp->val = malloc(sizeof(VAL_TYPE));\
	*tmp->val = val;\
	tmp->prev = l->last;\
	tmp->next = NULL;\
	l->last->next = tmp;\
	l->last = tmp;\
	l->length++;\
}\
void NAME##_APPEND(NAME* l, VAL_TYPE* val)\
	{\
	if (l->last->val == NULL) {\
		l->last->val = val;\
		l->last->next = NULL;\
	return;\
}\
	NAME##_NODE* tmp = malloc(sizeof(NAME##_NODE));\
	tmp->val = val;\
	tmp->prev = l->last;\
	tmp->next = NULL;\
	l->last->next = tmp;\
	l->last = tmp;\
	l->length++;\
}\
VAL_TYPE* NAME##_GET(NAME* l, int i)\
{\
	return l->first->val;\
}\
VAL_TYPE* NAME##_GET_AT_INDEX(NAME* l, int i)\
{\
	int j = 0;\
	ITERATE_LIST_PTR(NAME, l, n)\
	{\
		if (j == i)\
		return n->val;\
		j++;\
	}\
	return NULL;\
}\
NAME##_NODE* NAME##_GET_NODE(NAME* l)\
{\
	return l->first;\
}\
NAME##_NODE* NAME##_GET_NODE_AT_INDEX(NAME* l, int i)\
{\
	int j = 0;\
	ITERATE_LIST_PTR(NAME, l, n)\
	{\
		if (j == i)\
		return n;\
		j++;\
	}\
return NULL;\
}\
void NAME##_POP_AT_PTR(NAME* l, NAME##_NODE* n)\
{\
	if (l->length <= 0) return;\
	free(n->val);\
	if (n == l->first && n == l->last) {\
		n->val = NULL;\
		return;\
	}\
	if (n == l->first) l->first = n->next;\
	if (n == l->last) l->last = n->prev;\
	if (n->prev != NULL) n->prev->next = n->next;\
	if (n->next != NULL) n->next->prev = n->prev;\
	free(n);\
	l->length--;\
}\
void NAME##_POP(NAME* l)\
{\
	NAME##_POP_AT_PTR(l, l->last);\
}\
void NAME##_POP_AT_INDEX(NAME* l, int i)\
{\
	if (l->length < 2) return;\
	if (i > l->length) return;\
	NAME##_POP_AT_PTR(l, NAME##_GET_NODE_AT_INDEX(l, i));\
}\
void NAME##_INIT(NAME* l)\
{\
	NAME##_NODE* tmp = malloc(sizeof(NAME##_NODE));\
	tmp->prev = NULL;\
	tmp->next = NULL;\
	tmp->val = NULL;\
	l->length = 1;\
	l->first = tmp;\
	l->last = tmp;\
}\
void NAME##_INIT_VAL(NAME* l, VAL_TYPE val)\
{\
	NAME##_INIT(l);\
	NAME##_APPEND_VAL(l, val);\
}\



