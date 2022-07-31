#pragma once

#define DEBUG(s, ...) printf(s __VA_ARGS__)

#define lobotomy_warning(...) DEBUG("warning: ", __VA_ARGS__)
#define lobotomy_error(...) DEBUG("ERROR: ", __VA_ARGS__); exit(-1)


typedef struct SCOPE SCOPE;
typedef struct OBJ OBJ;
typedef struct TREE TREE;
typedef struct FUNC FUNC;



enum
{
	T_UNDEFINED,
	T_EXPR,
	T_NULL,
	T_CFUNC,
	T_FUNC,
	T_IDENTIFIER,
	T_NUMBER,
	T_DECIMAL,
	T_STR,
	T_LIST,
};

struct SCOPE
{
	int size, occupied;
	OBJ* values;
};


struct FUNC
{
	union
	{
		OBJ(*func_ptr)(OBJ);
		struct
		{
			uint8_t n;	// ion know why but it won't work properly without this
			SCOPE scope;
		};
	};
	int min, max;
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
		FUNC func;
	};

};


OBJ undefined()
{
	OBJ obj;
	obj.type = 0;
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
		case T_EXPR:
			return "EXPR";
		case T_NULL:
			return "NULL";
		case T_CFUNC:
			return "C FUNCTION";
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
		case T_LIST:
			return "LIST";
	}

	// printf("type_name: %d\n", type);
	return "FUCK";
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
	printf("\n\n%s----- OBJECT [%s] -------\n\n%s%s", indent, type_name(obj.type), indent, indent);
	
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
	if (obj.type == T_UNDEFINED)
		lobotomy_warning("UNDEFINED");

	if ((obj.type == T_IDENTIFIER || obj.type == T_EXPR || obj.type == T_CFUNC) && obj.name != NULL) {
		printf("%s [%s]\n", obj.name, type_name(obj.type));
	}

	if (obj.type == T_NULL) {
		printf("NULL");
	}

	else if (obj.type == T_FUNC) {
		printf("%s [%s]", obj.name, type_name(obj.type));
		// print_scope(obj.func.scope);
		printf("\n\t");
		for (int i = 0; i < obj.list_i; i++) {
			print_obj(obj.list[i]);
		}
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

	else if (obj.type == T_LIST) {
		for (int i = 0; i < obj.list_i; i++) {
			print_obj(obj.list[i]);
		}
	}

	printf("\n");
}

void add_obj_to_obj(OBJ* dest, const OBJ src)
{
	dest->list[dest->list_i++] = src;
	if (dest->list_i >= dest->list_max) {
		dest->list_max += 10;
		dest->list = realloc(dest->list, dest->list_max*sizeof(OBJ));
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


void scope_init(SCOPE* scope, int size)
{
	scope->occupied = 0;
	scope->size = size;
	scope->values = malloc(scope->size*sizeof(OBJ));
}

OBJ create_cfunc(char* name, OBJ(*func)(OBJ), int min, int max)
{
	OBJ obj;
	obj.type = T_CFUNC;
	obj.name = name;
	FUNC func_s;
	func_s.func_ptr = func;
	func_s.min = min;
	func_s.max = max;
	obj.func = func_s;
	// printf("creating object: %d\n", obj.type);
	return obj;
}


OBJ create_func(char* name, OBJ expr)
{
	OBJ obj;
	obj.type = T_FUNC;
	obj.name = name;
	OBJ args = expr.list[1];
	OBJ el = expr.list[2];

	scope_init(&obj.func.scope, args.list_i);
	
	if (args.type != T_NULL) {
		for (int i = 0; i < args.list_i; i++) {
			add_to_scope(&obj.func.scope, args.list[i]);
		}
	}

	obj.list_i = expr.list[2].list_i;
	obj.list_max = expr.list[2].list_max;
	obj.list = malloc(obj.list_i*sizeof(OBJ));
	obj.list = el.list;	
	
	// printf("creating object: %d\n", obj.type);
	return obj;
}

OBJ create_var(char* name, OBJ obj)
{
	obj.name = name;
	if (obj.type == T_UNDEFINED)
		obj.type = T_NULL;
	return obj;
}




