#pragma once

typedef struct SCOPE SCOPE;
typedef struct OBJ OBJ;
typedef struct TREE TREE;


enum
{
	T_EXPR=1,
	T_NULL,
	T_FUNC,
	T_IDENTIFIER,
	T_NUMBER,
	T_DECIMAL,
	T_STR,
	T_LIST,
};

struct OBJ
{
	int type;
	int list_i, list_max;
	char* name; // NULL if literal
	
	union
	{
		char* str;
		int64_t number;
		double decimal;
		OBJ* list;
		OBJ* ref;
		OBJ(*fn)(OBJ);
	};

};

OBJ undefined()
{
	OBJ obj;
	obj.type = T_NULL;
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


const char* type_name(int type)
{
	switch (type)
	{
		case T_EXPR:
			return "EXPR";
		case T_NULL:
			return "NULL";
		case T_FUNC:
			return "FUNCTION";
		case T_IDENTIFIER:
			return "IDENTIFIER";
		case T_NUMBER:
			return "NUMBER";
		case T_DECIMAL:
			return "DECIMAL";	
		case T_STR:
			return "STR";
	}

	return "FUCK";
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
	printf("\n\n%s----- OBJECT [%s] -------\n\n%s%s", indent, type_name(obj.type), indent, indent);
	
	// printf("\n\n-----OBJECT-------\n\n");
	// printf("debg: %d\n", obj.type);
	// if (obj.type == 0)
		// return;

	if ((obj.type == T_IDENTIFIER || obj.type == T_EXPR || obj.type == T_FUNC) && obj.name != NULL) {
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
		for (int i = 0; i < obj.list_i; i++) {
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

void print_obj(OBJ obj)
{
	if (obj.type == 0)
		return;

	if ((obj.type == T_IDENTIFIER || obj.type == T_EXPR || obj.type == T_FUNC) && obj.name != NULL) {
		printf("%s [%s]\n", obj.name, type_name(obj.type));
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
		for (int i = 0; i < obj.list_i; i++) {
			print_obj(obj.list[i]);
		}
	}

	// printf("\n");
}

void add_obj_to_obj(OBJ* dest, const OBJ src)
{
	dest->list[dest->list_i++] = src;
	if (dest->list_i >= dest->list_max) {
		dest->list_max += 10;
		dest->list = realloc(dest->list, dest->list_max*sizeof(OBJ));
	}
}

struct SCOPE
{
	int size, occupied;
	OBJ* values;
};


void print_scope(SCOPE scope)
{
	for (int i = 0; i < scope.size; i++)
	{
		if (scope.values[i].type == 0 || scope.values[i].name == NULL)
			continue;
		printf("init[%d]: %d |%s|\n", i, scope.values[i].type, scope.values[i].name);
	}
}


OBJ* add_to_scope(SCOPE* scope, OBJ value)
{
	int num = hash(value.name) % scope->size;
	// printf("adding object to scope at index |%s| %d %d\n", value.name, num, value.type);
	scope->values[num] = value;
	scope->occupied++;

	if (scope->occupied >= scope->size) {
		scope->size += 20;
		scope->values = realloc(scope->values, scope->size*sizeof(OBJ));
	}

	return &scope->values[num];

	// printf("added object to scope %d\n", scope->values[num].type);
}


OBJ* find_ptr_in_scope(SCOPE* scope, char* key) {
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

	return NULL;
	
}

OBJ find_in_scope(SCOPE scope, char* key)
{
	int num = hash(key) % scope.size;
	
	// printf("finding: |%s| %d\n", key, num);

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

	return undefined();
}



OBJ create_fn(char* name, OBJ(*fn)(OBJ))
{
	OBJ obj;
	obj.type = T_FUNC;
	obj.name = name;
	obj.fn = fn;
	// printf("creating object: %d\n", obj.type);
	return obj;
}

OBJ create_var(char* name, OBJ obj)
{
	obj.name = name;
	return obj;
}




