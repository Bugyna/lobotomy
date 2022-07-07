#include "parse.h"


typedef struct SCOPE SCOPE;


struct SCOPE
{
	OBJ* values;
};



int hash(char* key)
{

	return 0;
}


OBJ minus(OBJ expr)
{
	printf("munus\n");
	OBJ res;
	res.type = T_NUMBER;
	res.number = 0;
	for (int i = 0; i < expr.list_i; i++) {
		if (expr.list[i].type == 0)
			break;
		OBJ tmp = expr.list[i];
		switch (tmp.type)
		{
			case T_NUMBER:
				printf("res bef: %ld\n", res.number);
				res.number -= tmp.number;
				printf("res aft: %ld\n", res.number);
				break;
		}
	}
	return res;
}



OBJ exec_builtin(OBJ expr)
{
	OBJ obj;
	obj.type = 0;
	printf("builtin name: %s\n", expr.name);
	if (strcmp(expr.name, "minus ")) {
		// printf("here");
		return minus(expr);
	}

	return obj;
}

void set_val(OBJ* obj)
{
	
}

void eval(OBJ expr)
{
	// print_obj(*expr);
	if (expr.type != T_EXPR) {
		return;
	}
	// exec_builtin(expr);
	print_obj(exec_builtin(expr));
	
}


void interpret(const char text[])
{
	TREE tree = parse(text);
	printf("\n\n---------------EVAL--------------------\n\n");
	for (int i = 0; i < tree.index; i++) {
		if (tree.expr[i].type == 0)
			break;
		// print_obj(tree.expr[i]);
		eval(tree.expr[i]);
	}
	printf("\n\n---------------------------------------\n\n");
}



