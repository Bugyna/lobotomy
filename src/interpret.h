#include "parse.h"
#include "GC.h"


typedef struct VAR VAR;

struct VAR
{
	char* name;
	OBJ value;
};

// void let(VAR* vars, static const EXPR expr)
// {
	
// }



int hash(char* key)
{

	return 0;
}


OBJ* minus(const EXPR expr)
{
	OBJ res;

	for (int i = 0; i < expr.args_i; i++) {
		if (expr.args[i].type == NULL) {
			break;
		}

		
		switch (expr.args[i].type)
		{
			case T_NUMBER:
				res.number -= expr.args[i].number;
		}
	}

	// printf("res: %d\n", res);
	print_obj(res);
	return res;
}



OBJ exec_builtin(EXPR expr)
{
	if (strcmp(expr.keyword, "minus")) {
		return minus(expr);
	}
}

void eval(const EXPR expr)
{
	
}

void interpret(const char text[])
{
	VAR* var = malloc(20*sizeof(VAR));
	TREE tree = parse(text);

	for (int i = 0; i < tree.index; i++) {
		eval(tree.expr[i]);
	}
}



