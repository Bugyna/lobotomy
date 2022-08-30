#pragma once
#include "util.h"


typedef struct SCOPE SCOPE;
typedef struct OBJ OBJ;
typedef struct TREE TREE;
typedef struct FUNC FUNC;



enum
{
	T_UNDEFINED,
	T_ERROR,
	T_EXPR,
	T_NULL,
	T_CFUNC,
	T_FUNC_DEF,
	T_FUNC,
	T_IDENTIFIER,
	T_REF,
	T_NUMBER,
	T_DECIMAL,
	T_STR,
	T_LIST,
	T_CONST,
};


struct GC
{
	SCOPE* scope;
};

struct SCOPE
{
	char* name;
	int size, occupied;
	OBJ* values;

	int func_index;
	SCOPE* func_scope;
};

SCOPE global;

// struct FUNC
// {
	// union
	// {
		// OBJ(*func_ptr)(OBJ);
		// struct
		// {
			// uint8_t n;	// ion know why but it won't work properly without this
			// SCOPE scope;
		// };
	// };
	// int min, max;
// };


// struct OBJ
// {
	// int type;
	// int list_i, list_max;
	// char* name; // NULL if literal
	
	// union
	// {
		// char* str;
		// int64_t number;
		// double decimal;
		// OBJ* list;
		// OBJ* ref;
	// };

// };


struct OBJ
{
	uint8_t type;
	int index;
	char* name;
	SCOPE* scope;

	union
	{
		char* str;
		int64_t number;
		double decimal;
		OBJ(*func)(OBJ*);
		OBJ* list;
		OBJ* ref;
		OBJ** args;
	};
};


OBJ undefined()
{
	OBJ obj;
	obj.name = NULL;
	obj.type = T_UNDEFINED;
	return obj;
}

OBJ lobotomy_exception(char* msg)
{
	OBJ obj;
	obj.name = NULL;
	obj.type = T_ERROR;
	obj.str = msg;
	return obj;
}


struct TREE
{
	OBJ* expr;
	int index, max, peek;
};

void add_expr(TREE* tree, const OBJ expr)
{
	tree->expr[tree->index++] = expr;
	if (tree->index+1 >= tree->max) {
		tree->max += 10;
		tree->expr = realloc(tree->expr, tree->max);
	}
}


char* type_name(int type)
{
	switch (type)
	{
		case T_UNDEFINED:
			return "UNDEFINED";
		case T_EXPR:
			return "EXPR";
		case T_NULL:
			return "NULL";
		case T_CFUNC:
			return "C FUNCTION";
		case T_FUNC:
			return "FUNCTION";
		case T_FUNC_DEF:
			return "FUNCTION DEFINITION";
		case T_IDENTIFIER:
			return "IDENTIFIER";
		case T_NUMBER:
			return "NUMBER";
		case T_DECIMAL:
			return "DECIMAL";	
		case T_STR:
			return "STR";
		case T_LIST:
			return "LIST";
	}

	// printf("type_name: %d\n", type);
	return "FUCK";
}

OBJ create_copy(OBJ* expr)
{
	OBJ ret;
	ret.index = expr->index;
	ret.type = expr->type;
	ret.list = malloc(expr->index*sizeof(OBJ));
	
	for (int i = 0; i < expr->index; i++) {
		if (expr->list[i].type == T_LIST) {
			printf("copying: %s\n", expr->list[i].list[0].name);
			ret.list[i] = create_copy(&expr->list[i]);
		}
		else {
			OBJ tmp = expr->list[i];
			ret.list[i] = tmp;
		}
	}

	return ret;
}

void print_scope(SCOPE scope)
{
	for (int i = 0; i < scope.size; i++)
	{
		if (scope.values[i].type == 0 || scope.values[i].name == NULL)
			continue;
		printf("init[%d]: %d |%s<%s>|\n",
			i, scope.values[i].type, scope.values[i].name, type_name(scope.values[i].type)
		);
	}
}


void _print_obj_full(OBJ obj, int indent_size)
{
	if (obj.type == 0)
		return;


	char* indent = malloc(indent_size);
	if (indent_size > 0 && indent_size < 100) {
		memset(indent, '\t', indent_size);
	}
	indent[indent_size] = '\0';
	// printf("\n\n%s----- OBJECT [%s] -------\n\n%s%s", indent, type_name(obj.type), indent, indent);
	
	// printf("\n\n-----OBJECT-------\n\n");
	// printf("debg: %d\n", obj.type);
	// if (obj.type == 0)
		// return;

	if ((obj.type == T_IDENTIFIER || obj.type == T_EXPR || obj.type == T_CFUNC) && obj.name != NULL) {
		printf("%s", obj.name);
	}

	if (obj.type == T_NULL) {
		printf("NULL");
	}

	else if (obj.type == T_NUMBER) {
		printf("%ld ", obj.number);
	}

	else if (obj.type == T_DECIMAL) {
		printf("%f ", obj.decimal);
	}

	 else if (obj.type == T_STR) {
		printf("%s ", obj.str);
	}

	else if (obj.type == T_EXPR || obj.type == T_LIST) {
		for (int i = 0; i < obj.index; i++) {
			// printf("ddd: %d\n", obj.list[i].type);
			_print_obj_full(obj.list[i], indent_size+1);
		}
	}
	printf("\n\n%s------------------\n\n", indent);
	// printf("\n\n------------------\n\n");
}

void print_obj_full(OBJ obj)
{
	_print_obj_full(obj, 0);
}

void print_obj_(OBJ obj)
{
	if (obj.type == T_UNDEFINED) {
		// lobotomy_warning("UNDEFINED");
		printf("UNDEFINED");
	}

	if ((obj.type == T_IDENTIFIER || obj.type == T_EXPR || obj.type == T_CFUNC) && obj.name != NULL) {
		printf("%s [%s]\n", obj.name, type_name(obj.type));
	}

	if (obj.type == T_NULL) {
		printf("NULL");
	}

	else if (obj.type == T_FUNC) {
		// printf("%s [%s] %s", obj.name, type_name(obj.type), obj.list[1].name);
		// printf("\n\t");
		printf("FUNC: ");
		for (int i = 0; i < obj.index; i++) {
			if (obj.list[i].type == T_UNDEFINED)
				break;
			print_obj_(obj.list[i]);
		}
		printf("END OF FUNC");
	}

	else if (obj.type == T_NUMBER) {
		printf(" %ld", obj.number);
	}

	else if (obj.type == T_DECIMAL) {
		printf(" %f", obj.decimal);
	}

	else if (obj.type == T_STR) {
		printf(" %s", obj.str);
	}

	else if (obj.type == T_LIST) {
		printf("\nLIST:\n\t");
		for (int i = 0; i < obj.index; i++) {
			if (obj.list[i].type == T_UNDEFINED)
				break;
			print_obj_(obj.list[i]);
		}
		printf("\nEND OF LIST");
	}
}

void print_obj(OBJ obj)
{
	print_obj_(obj);

	printf("\n");
}

void print_obj_type(OBJ obj)
{
	printf("%s: %s\n", obj.name, type_name(obj.type));
}

void add_obj_to_obj(OBJ* dest, const OBJ src)
{
	dest->list[dest->index++] = src;
	if (dest->index%5 == 0) {
		dest->list = realloc(dest->list, dest->index*5*sizeof(OBJ));
	}
}

OBJ* reserve(SCOPE* scope, OBJ value)
{
	int num = hash(value.name) % scope->size;
	scope->values[num] = value;
	scope->occupied++;

	if (scope->occupied >= scope->size) {
		scope->size += 20;
		scope->values = realloc(scope->values, scope->size*sizeof(OBJ));
	}

	return &scope->values[num];
}


OBJ* add_to_scope(SCOPE* scope, OBJ value)
{
	// printf("here22 %s %d %d %d\n", value.name, hash(value.name), scope->size, hash(value.name) % scope->size);
	int num = hash(value.name) % scope->size;
	// printf("adding object to scope at index |%s| %d %d\n", value.name, num, value.type);
	if (scope->values[num].type == T_UNDEFINED) {
		scope->values[num] = value;
	}

	else if (strcmp(value.name, scope->values[num].name) == 0) {
		scope->values[num] = value;
	}

	else {
		for (int i = num; i < scope->size; i++) {
			if (scope->values[i].type == T_UNDEFINED) {
				scope->values[i] = value;
				break;
			}
		}
	}

	scope->occupied++;

	if (scope->occupied >= scope->size) {
		scope->size += 20;
		scope->values = realloc(scope->values, scope->size*sizeof(OBJ));
	}

	return &scope->values[num];

	// printf("added object to scope %d\n", scope->values[num].type);
}


OBJ* find_ptr_in_scope(SCOPE* scope, char* key) {
	if (key == NULL)
		return NULL;

	int num = hash(key) % scope->size;
	
	// printf("finding: |%s| %d\n", key, num);

	// printf("fucker: |%s| |%s| %d %d\n", scope.values[num].name, key, hash(scope.values[num].name), hash(key));

	// printf("num: %d\n", num);
	if (scope->values[num].type != 0 && !strcmp(scope->values[num].name, key)) {
		return &scope->values[num];
	}


	else {
		for (int i = 0; i < scope->size; i++) {
			if (scope->values[i].type != 0 && !strcmp(scope->values[i].name, key)) {
				return &scope->values[i];
				// return undefined();
			}
		}
	}

	// lobotomy_error("undefined variable %s\n", key);
	// lobotomy_warning("undefined variable %s\n", key);
	return NULL;
	
}

OBJ find_in_scope(SCOPE scope, char* key)
{
	if (key == NULL)
		return undefined();

	int num = hash(key) % scope.size;
	
	// printf("finding: |%s| %d %d\n", key, num, scope.size);

	// printf("fucker: |%s| |%s| %d %d\n", scope.values[num].name, key, hash(scope.values[num].name), hash(key));

	// printf("num: %d\n", num);
	if (scope.values[num].type != 0 && !strcmp(scope.values[num].name, key)) {
		return scope.values[num];
	}


	else {
		for (int i = 0; i < scope.size; i++) {
			if (scope.values[i].type != 0 && !strcmp(scope.values[i].name, key)) {
				return scope.values[i];
				// return undefined();
			}
		}
	}

	// lobotomy_error("undefined variable %s\n", key);
	// lobotomy_warning("undefined variable %s\n", key);
	return undefined();
}


void scope_init(SCOPE* scope, int size)
{
	scope->occupied = 0;
	scope->size = size;
	scope->values = malloc(scope->size*sizeof(OBJ));
}


// void create_func_scope(SCOPE* scope)
// {
	// scope->func_scope = malloc(5*sizeof(scope));
// }

void add_func_scope(SCOPE* scope, char* name, int argc)
{
	if (scope->func_index % 5 == 0) {
		scope->func_scope = realloc(scope->func_scope, (scope->func_index+5)*sizeof(SCOPE));
	}

	scope->func_scope[scope->func_index].name = name;
	scope_init(&scope->func_scope[scope->func_index], argc);
	scope->func_index++;
}

OBJ create_cfunc(char* name, OBJ(*func)(OBJ*), int min, int max)
{
	OBJ obj;
	obj.type = T_CFUNC;
	obj.name = name;
	obj.func = func;
	// printf("creating object: %d\n", obj.type);
	return obj;
}


OBJ create_func(char* name, OBJ* expr)
{
	
	OBJ obj;
	obj.type = T_FUNC;
	obj.name = name;

	printf("index: %d\n", expr->index);
	obj.index = expr->index;
	obj.list = malloc(obj.index*sizeof(OBJ));

	obj.list[0] = expr->list[2];
	obj.list[0].name = "args";
	obj.list[0].index = expr->list[2].index;

	obj.list[1] = expr->list[3];
	obj.list[1].name = "body";
	obj.list[1].index = expr->list[3].index;

	// printf("f: %d\n", obj.list[0].index);
	// printf("f: %d\n", obj.list[1].index);

	// add_func_scope(&global, obj.name, obj.list[0].index);

	// print_obj_type(obj.list[0]);
	// print_obj_type(obj.list[0].list[0]);
	// print_obj_type(obj.list[1]);
	// print_obj_type(obj.list[1].list[0]);

	// for (int i = 0; i < obj.list[0].index; i++) {
		// for (int j = 0; j < obj.list[1].index; j++) {
			// if (obj.list[1].list[j].name != NULL && strcmp(obj.list[0].list[i].name, obj.list[1].list[j].name) == 0) {
				// printf("got in: %s\n", obj.list[1].list[j].name);
				// if (find_ptr_in_scope(&global.func_scope[0], obj.list[0].list[i].name) == NULL) {
					// obj.list[1].list[j].ref = add_to_scope(&global.func_scope[global.func_index-1], obj.list[0].list[i]);
				// }

				// else {
					// obj.list[1].list[j].ref = find_ptr_in_scope(&global.func_scope[global.func_index-1], obj.list[0].list[i].name);
				// }

				// printf("ptr: %s %p\n", obj.list[0].list[i].name, obj.list[1].list[j].ref);
				// obj.list[1].list[j].type = T_REF;
			// }
		// }
	// }
	// printf("dwafsdd\n");
	return obj;
	// return undefined();
}

OBJ create_var(char* name, OBJ obj)
{
	obj.name = name;
	if (obj.type == T_UNDEFINED)
		obj.type = T_NULL;
	return obj;
}




