#include "parse.h"



/* #define arithmetic_shorthand(a, b, op) switch (op) {case '+': *a += *b; break; case '-': *a -= *b; break; \
// case '*': \
			// *a *= *b; \
		// break; \
// \
		// case '/': \
// \
			// *a /= *b; \
// \
		// break; \
		// case '%': \
			// *a %= *b; \
		// break; \
// }

*/


enum
{
	ARG_NONE=-1,
	ARG_INF=-2,
};

void eval_args(OBJ*);
OBJ eval(OBJ*);


// OBJ lobotomy_set(OBJ expr)
// {
	// OBJ id = expr.list[0];
	// OBJ val = expr.list[1];
	// OBJ* tmp = find_ptr_in_scope(&global, id.name);
	// printf("setting var: %s %s -> %s\n", id.name, type_name(id.type), type_name(val.type));
	// if (val.type = T_IDENTIFIER) {
		// val = find_in_scope(global, val.name);
	// }
	// id.type = val.type;

	// id = *add_to_scope(&global, create_var(id.name, val));
	// return id;
// }


OBJ lobotomy_type(OBJ* obj)
{
	eval_args(obj);
	OBJ res;
	res.type = T_STR;
	res.str = type_name(obj->list[0].type);
	return res;
}


// OBJ cast_to_int(OBJ* obj)
// {
	// cast_obj(&obj, T_NUMBER);
	// return obj;
// }

// OBJ cast_to_float(OBJ obj)
// {
	// cast_obj(&obj, T_DECIMAL);
	// return obj;
// }


void arithmetic_shorthand_int(int64_t* a, int64_t* b, char op)
{
	switch (op)
	{
		case '+':
			*a += *b;
		break;

		case '-':
			*a -= *b;
		break;

		case '*':
			*a *= *b;
		break;

		case '/':
			*a /= *b;
		break;

		case '%':
			*a %= *b;
		break;

		case '&':
			*a &= *b;
		break;

		case '|':
			*a |= *b;
		break;

		case '^':
			*a ^= *b;
		break;
	}
}



OBJ arithmetic_operation(OBJ* expr)
{
	eval_args(expr);

	char op = expr->list[0].name[0];
	// printf("AO: %ld %ld | op: %c\n", expr->list[1].number, expr->list[2].number, op);
	OBJ res;
	int i = 1;

	if (expr->index > 2) {
		res = expr->list[1];
		i = 2;
	}
	else {
		res.type = T_NUMBER;
		res.number = 0;
	}

	for (; i < expr->index; i++) {
		if (expr->list[i].type == 0)
			break;
		OBJ tmp = expr->list[i];
		switch (tmp.type)
		{
			case T_NUMBER:
				// if (tmp.type == T_NUMBER)
					arithmetic_shorthand_int(&res.number, &tmp.number, op);
				// else if (tmp.type == T_DECIMAL)
					// arithmetic_shorthand_int(&res.number, (int64_t*)&tmp.decimal, op);
				
				break;

			// case T_DECIMAL:
				// if (tmp.type == T_NUMBER)
					// arithmetic_shorthand_int((int64_t*)&res.decimal, &tmp.number, op);
				// else if (tmp.type == T_DECIMAL)
					// arithmetic_shorthand_int((int64_t*)&res.decimal, (int64_t*)&tmp.decimal, op);
				// break;
		}
	}
	return res;
}


OBJ lobotomy_if(OBJ* expr)
{
	OBJ ret;
	OBJ res = eval(&expr->list[0]);
	if (res.number == 1) {
		ret = eval(&expr->list[1]);
	}

	else {
		ret = eval(&expr->list[2]);
	}


	return ret;
	
}

OBJ lobotomy_loop(OBJ* expr)
{
	return undefined();
}

OBJ lobotomy_equals(OBJ* expr)
{
	printf("in eq\n");
	OBJ ret;
	ret.type = T_NUMBER;
	eval_args(expr);

	printf("eq: %s %s\n", type_name(expr->list[1].type), type_name(expr->list[2].type));

	if (expr->list[1].type == expr->list[2].type) {
		switch (expr->list[1].type) {
			case T_NUMBER:
				if (expr->list[1].number == expr->list[2].number) {
					printf("heredd\n");
					ret.number = 1;
				}
			break;

			default:
				ret.number = 0;
			break;
		}
	}

	else {
		ret.number = 0;
		printf("heredd\n");
	}

	printf("ret in eq: %ld\n", ret.number);
	return ret;
}

// OBJ minus(OBJ expr)
// {
	// return arithmetic_operation(expr, '-');
// }

// OBJ plus(OBJ expr)
// {
	// return arithmetic_operation(expr, '+');
// }

// OBJ multiply(OBJ expr)
// {
	// return arithmetic_operation(expr, '*');
// }

// OBJ divide(OBJ expr)
// {
	// expr.list[0] = cast_to_float(expr.list[0]);
	// return arithmetic_operation(expr, '/');
// }

// OBJ modulo(OBJ expr)
// {
	// return arithmetic_operation(expr, '%');
// }

OBJ lobotomy_exit(OBJ* expr)
{
	exit(0);
	return undefined();
}

OBJ print_scope_obj(OBJ* expr)
{
	print_scope(global);
	return undefined();
}


OBJ lobotomy_print(OBJ* expr)
{
	eval_args(expr);
	// expr->type = T_LIST;
	for (int i = 1; i < expr->index; i++) {
		print_obj_(expr->list[i]);
	}
	return *expr;
}

OBJ lobotomy_let(OBJ* expr)
{
	expr->list[1] = eval(&expr->list[1]);
	OBJ id = expr->list[1];
	reserve(&global, id);
	eval_args(expr);
	OBJ val = expr->list[2];
	// printf("creating var: %s %s -> %s\n", id.name, type_name(id.type), type_name(val.type));
	// if (val.type = T_IDENTIFIER) {
		// val = find_in_scope(global, val.name);
	// }
	// id.type = val.type;
	// if (id.type != T_IDENTIFIER) {
		// return undefined();
	// }

	// if (id.type == T_IDENTIFIER) {
		// id.ref = add_to_scope(&global, create_var(id.name, val));
		// return *id.ref;
	// }

	// else {
	id = *add_to_scope(&global, create_var(id.name, val));
	return id;
	// }
}

OBJ lobotomy_func(OBJ* expr)
{
	// printf("creating lob func\n");
	OBJ obj = create_func(expr->list[1].name, expr);
	// printf("creating lob func %s %s\n", expr->list[1].name, obj.name);
	add_to_scope(&global, obj);
	return obj;
	// return undefined();
}

OBJ nothing(OBJ* expr)
{
	return undefined();
}

void init()
{
	add_to_scope(&global, create_cfunc("-", arithmetic_operation, 1, ARG_INF));
	add_to_scope(&global, create_cfunc("+", arithmetic_operation, 1, ARG_INF));
	// add_to_scope(&global, create_cfunc("mul", multiply, 2, ARG_INF));
	// add_to_scope(&global, create_cfunc("div", divide, 2, ARG_INF));
	// add_to_scope(&global, create_cfunc("mod", modulo, 2, ARG_INF));
	add_to_scope(&global, create_cfunc("exit", lobotomy_exit, ARG_NONE, ARG_INF));
	add_to_scope(&global, create_cfunc("pso", print_scope_obj, ARG_NONE, ARG_INF));
	add_to_scope(&global, create_cfunc("let", lobotomy_let, 2, 2));
	add_to_scope(&global, create_cfunc("eq", lobotomy_equals, ARG_INF, ARG_INF));
	add_to_scope(&global, create_cfunc("=", lobotomy_equals, ARG_INF, ARG_INF));
	add_to_scope(&global, create_cfunc("if", lobotomy_if, 3, ARG_INF));
	add_to_scope(&global, create_cfunc("?", lobotomy_if, 3, ARG_INF));

	add_to_scope(&global, create_cfunc("func", lobotomy_func, 3, 3));
	
	add_to_scope(&global, create_cfunc("print", lobotomy_print, 1, ARG_INF));
	add_to_scope(&global, create_cfunc("type", lobotomy_type, 1, 1));
	add_to_scope(&global, create_cfunc("nothing", nothing, 0, ARG_INF));
	

	// OBJ test;
	// test.type = T_NUMBER;
	// test.number = 5;

	// add_to_scope(&global, create_var("a", test));
	// print_scope(global);

}

OBJ rr(OBJ* o) { OBJ p; p = *o; return p; }

OBJ run_func(OBJ func, OBJ* expr)
{
	// OBJ* tmp;
	// OBJ tmp_tmp;

	// for (int i = 0; i < func.list[0].index; i++) {
		// tmp = find_ptr_in_scope(&global.func_scope[global.func_index-1], func.list[0].list[i].name);
		// tmp_tmp = *tmp;
		// printf("tmp name: %s %s %ld\n", tmp->name, type_name(tmp->type), tmp->number);
		// expr->list[1+i].name = tmp->name;
		// *tmp = expr->list[1+i];
		
		// printf("tmp name: %s %s %ld\n", tmp->name, type_name(tmp->type), tmp->number);
		// *tmp = func.list[0].list[i];
		// tmp->type = T_REF;
		// tmp->ref = &expr->list[1+i];
		// print_obj(*tmp);
		// for (int j = 0; j < func.list[1].index; j++) {
			// if (strcmp(func.list[0].list[i].name, func.list[1].list[j].name) == 0) {
				// func.list[1].list[j] = tmp;
				// printf("ptr c: %s: %p | %s: %p\n", func.list[1].list[j].name, func.list[1].list[j].ref, tmp->name, tmp);
			// }
		// }
	// }


	// add_func_scope(&global, func.name, func.list[0].index);
	// printf("running func: %p %d\n", expr, global.func_index);
	// printf("size: %d %d\n", func.list[0].index, global.func_scope[global.func_index-1].size);
	
	// for (int i = 0; i < func.list[0].index; i++) {
		// expr->list[i+1].name = func.list[0].list[i].name;
		// printf("name: %s\n", expr->list[i+1].name);
		// printf("what: %ld\n", expr->list[i+1].number);
		// add_to_scope(&global.func_scope[global.func_index-1], expr->list[i+1]);
		// printf("fc: %ld\n", find_in_scope(global.func_scope[global.func_index-1], func.list[0].list[i].name).number);
		// func.list[1].scope = &global.func_scope[global.func_index-1];
	// }
	

	// func.list[1].list = rr(func.list[1].list);
	// printf("here run func %d %d\n", func.list[1].index, expr->list[0].index);
	// printf("fc: %ld\n", find_in_scope(global.func_scope[global.func_index-1], func.list[0].list[0].name).number);
	for (int i = 0; i < func.list[1].index; i++) {
		for (int j = 0; j < func.list[0].index; j++) {
		// OBJ tmp = find_in_scope(global.func_scope[global.func_index-1], func.list[1].list[i].name);
		// print_obj(tmp);
		// if (tmp.type != T_UNDEFINED)
		if (func.list[1].list[i].name != NULL && func.list[0].list[j].name != NULL && strcmp(func.list[1].list[i].name, func.list[0].list[j].name) == 0) {
			// printf("ok %s %s\n", func.list[1].list[i].name, expr->list[j].name);
			expr->list[j+1].name = func.list[1].list[i].name;
			func.list[1].list[i] = expr->list[j+1];
		}
		// printf("fc: %s: %ld\n", func.list[1].list[i].name, find_in_scope(global.func_scope[global.func_index-1], func.list[1].list[1].name).number);
		}
	}
	// printf("fc: %s: %ld\n", func.list[1].list[1].name, find_in_scope(global.func_scope[global.func_index-1], func.list[1].list[1].name).number);
	// scope_init(&global.func_scope[global.func_index], 5);
	OBJ res = eval(&func.list[1]);
	return res;

	// return undefined();
}

void eval_args(OBJ* expr)
{
	// if (scope == NULL)
		// scope = &global;

	for (int i = 0; i <= expr->index; i++) {
		printf("expr naem: %s %s %ld\n", expr->name, type_name(expr->list[i].type), expr->list[i].number);
		if (expr->list[i].type == T_CFUNC)
			continue;
		
		else if (expr->list[i].type == T_IDENTIFIER) {
			if (expr->type != T_FUNC) {
				// OBJ tmp = find_in_scope(*scope, expr->list[i].name);
				// printf("here in eval %s %s %ld %s\n", tmp.name, expr->list[i].name, tmp.number, type_name(tmp.type));
				// if (tmp.type == T_UNDEFINED) {
					// printf("fuckery\n");
					OBJ tmp = find_in_scope(global, expr->list[i].name);

					if (tmp.type == T_UNDEFINED) {
						lobotomy_error("%s is undefined", expr->list[i].name);
					}
				// }

				expr->list[i] = tmp;
				
			}
			// if (expr->type != T_FUNC && find_in_scope(global, expr->list[i].name).type != T_UNDEFINED)
				// expr->list[i] = find_in_scope(global, expr->list[i].name);
			
			// else if (find_in_scope(global.func_scope[global.func_index-1], expr->list[i].name).type != T_UNDEFINED) {
				// expr->list[i] = find_in_scope(global.func_scope[global.func_index-1], expr->list[i].name);
			// }
		}

		else if (expr->list[i].type == T_REF) {
			printf("dda\n");
			expr->list[i] = *expr->list[i].ref;
			// print_obj_type(expr->list[i]);
			print_obj(expr->list[i]);
		}
		
		else if (expr->list[i].type == T_LIST)
			expr->list[i] = eval(&expr->list[i]);
		// printf("eval_args: %s\n", type_name(expr->list[i].type));
	}
}


OBJ eval(OBJ* expr)
{
	// if (scope == NULL)
		// scope = &global;
	// if (expr.type == T_LIST) {
		// eval_args(&expr);
		// return expr;
	// }

	if (expr->type != T_LIST && expr->type != T_CFUNC && expr->type != T_FUNC) {
		return *expr;
	}

	// printf("gonna try and find: |%s|\n", expr.name);
	// printf("fuck: %s<%s> %d\n", expr.list[0].name, type_name(expr.list[0].type), expr.list[0].type);


	// eval_args(&expr);
	// OBJ obj = expr.list[0];
	OBJ obj = find_in_scope(global, expr->list[0].name);
	expr->list[0] = obj;
	// OBJ obj = undefined();

	// printf("name: %s<%s> %d\n", obj.name, type_name(obj.type), obj.type);


	// printf("found obj: %d |%s| %ld\n", obj.type, obj.name, obj.number);
	// print_scope(global);
	// printf("found |%s| in global\n", obj.name);

	switch (obj.type)
	{
		case T_UNDEFINED:
			lobotomy_warning("%s is undefined", expr->name);
			return undefined();
		break;

		case T_EXPR:
			return eval(&obj);
		break;

		case T_NULL:
			lobotomy_warning("variable: %s <%s> is NULL", obj.name, type_name(obj.type));
			return obj;
		break;

		case T_IDENTIFIER:
			lobotomy_warning("variable: %s <%s> is uninitialized", obj.name, type_name(obj.type));
			return obj;
			// return undefined();
			// return expr;
		break;


		case T_FUNC:
			return run_func(obj, expr);
			// obj.list[0].ref = &expr.list[1];
			// return undefined();
			// eval_args(&obj);
			// return obj;
		break;

		case T_CFUNC:
			// printf("cfunc\n");
			return obj.func(expr);
		break;

		case T_LIST:
			return obj;
		break;

		default:
			return obj;
		break;
	}

	return lobotomy_exception("\nSHIT HAPPENED\n");
}


void interpret(const char text[])
{
	TREE tree = parse(text);
	// printf("\n\n---------------EVAL--------------------\n\n");
	OBJ res;
	for (int i = 0; i < tree.index; i++) {
		if (tree.expr[i].type == 0)
			break;
		
		// print_obj_full(tree.expr[i]);
		// printf("eval: %s\n", tree.expr[i].list[0].name);
		// eval(tree.expr[i]);

		res = eval(&tree.expr[i]);
		printf("\nres: ");
		print_obj(res);
		printf("\n");
	}
	// printf("\n\n---------------------------------------\n\n");
}

void interpret_(const char text[])
{
	TREE tree = parse(text);
	// printf("\n\n---------------EVAL--------------------\n\n");
	for (int i = 0; i < tree.index; i++) {
		if (tree.expr[i].type == T_UNDEFINED)
			break;
		// printf("eval: %s\n", type_name(tree.expr[i].type));
		eval(&tree.expr[i]);
	}
	// printf("\n\n---------------------------------------\n\n");

}


