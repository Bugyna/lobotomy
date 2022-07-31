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

void eval_args(OBJ* expr);

// void lobotomy_error(const char* msg)
// {
	// printf("ERROR: %s\n", msg);
	// exit(-1);
// }

// void lobotomy_warning(const char* msg)
// {
	// printf("WARNING: ");
	// printf("%s\n", msg);
	// S(warning, 
	
// }



OBJ eval(OBJ);



OBJ lobotomy_let(OBJ expr)
{
	// eval_args(&expr);
	OBJ id = expr.list[0];
	OBJ val = expr.list[1];
	// printf("creating var: %s %s -> %s\n", id.name, type_name(id.type), type_name(val.type));
	// if (val.type = T_IDENTIFIER) {
		// val = find_in_scope(global, val.name);
	// }
	// id.type = val.type;
	// if (id.type != T_IDENTIFIER) {
		// return undefined();
	// }

	if (id.type == T_IDENTIFIER) {
		id.ref = add_to_scope(&global, create_var(id.name, val));
		return *id.ref;
	}

	else {
		id = *add_to_scope(&global, create_var(id.name, val));
		return id;
	}

}


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

OBJ lobotomy_type(OBJ expr)
{
	// eval_args(&expr);
	OBJ obj;
	obj.type = T_STR;
	obj.str = type_name(expr.list[0].type);
	return obj;
}


void cast_obj(OBJ* obj, int type)
{
	switch (obj->type) {
		case T_EXPR:
			if (type == T_EXPR) {
				return;
			}
			else {
				lobotomy_error("trying to cast expr to: whatever");
			}
		break;

		case T_FUNC:
			if (type == T_EXPR) {
				lobotomy_warning("trying to cast function to expr");
			}

			else {
				lobotomy_warning("trying to cast function to whatever");
			}
		break;


		case T_NUMBER:
			if (type == T_DECIMAL) {
				obj->type = T_DECIMAL;
				obj->decimal = (double)obj->number;
			}

			else {
				lobotomy_error("illegal cast of integer");
			}
		break;

		case T_DECIMAL:
			if (type == T_NUMBER) {
				obj->type = T_NUMBER;
				obj->number = (int64_t)obj->decimal;
			}

			else {
				lobotomy_error("illegal cast of float");
			}
		break;

		default:
			lobotomy_warning("dude i am not even gonna do anything");
		break;
	}
	
	// obj.type = type;
}

OBJ cast_to_int(OBJ obj)
{
	cast_obj(&obj, T_NUMBER);
	return obj;
}

OBJ cast_to_float(OBJ obj)
{
	cast_obj(&obj, T_DECIMAL);
	return obj;
}


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



OBJ arithmetic_operation(OBJ expr, char op)
{
	// eval_args(&expr);
	OBJ res;
	int i = 0;

	if (expr.list_i > 1) {
		res = expr.list[0];
		i = 1;
	}
	else {
		res.type = T_NUMBER;
		res.number = 0;
	}

	for (; i < expr.list_i; i++) {
		if (expr.list[i].type == 0)
			break;
		OBJ tmp = expr.list[i];
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

			case T_EXPR:
				expr.list[i] = eval(tmp);
				i--;
				break;
		}
	}
	return res;
}


OBJ lobotomy_if(OBJ expr)
{
	OBJ ret;
	OBJ res = eval(expr.list[0]);
	if (res.number == 1) {
		ret = eval(expr.list[1]);
	}

	else {
		ret = eval(expr.list[2]);
	}


	return ret;
	
}

OBJ lobotomy_loop(OBJ expr)
{
	return undefined();
}

OBJ lobotomy_equals(OBJ expr)
{
	OBJ ret;
	ret.type = T_NUMBER;
	// eval_args(&expr);

	if (expr.list[0].type == expr.list[1].type) {
		switch (expr.list[0].type) {
			case T_NUMBER:
				if (expr.list[0].number == expr.list[1].number) {
					ret.number = 1;
				}
			break;
		}
	}

	else {
		ret.number = 0;
	}
	return ret;
}

OBJ minus(OBJ expr)
{
	return arithmetic_operation(expr, '-');
}

OBJ plus(OBJ expr)
{
	return arithmetic_operation(expr, '+');
}

OBJ multiply(OBJ expr)
{
	return arithmetic_operation(expr, '*');
}

OBJ divide(OBJ expr)
{
	expr.list[0] = cast_to_float(expr.list[0]);
	return arithmetic_operation(expr, '/');
}

OBJ modulo(OBJ expr)
{
	return arithmetic_operation(expr, '%');
}

OBJ lobotomy_exit(OBJ expr)
{
	exit(0);
	return undefined();
}

OBJ print_scope_obj(OBJ expr)
{
	print_scope(global);
	return undefined();
}


OBJ lobotomy_print(OBJ expr)
{
	// eval_args(&expr);
	expr.type = T_LIST;
	// for (int i = 0; i < expr.list_i; i++) {
		// print_obj(expr.list[i]);
	// }
	return expr;
	
}

OBJ lobotomy_func(OBJ expr)
{
	OBJ obj = create_func(expr.list[0].name, expr);
	obj.type = T_FUNC;
	add_to_scope(&global, obj);
	return obj;
	// return undefined();
}

OBJ nothing(OBJ expr)
{
	return undefined();
}

void init()
{
	add_to_scope(&global, create_cfunc("minus", minus, 1, ARG_INF));
	add_to_scope(&global, create_cfunc("plus", plus, 1, ARG_INF));
	add_to_scope(&global, create_cfunc("+", plus, 1, ARG_INF));
	add_to_scope(&global, create_cfunc("mul", multiply, 2, ARG_INF));
	add_to_scope(&global, create_cfunc("div", divide, 2, ARG_INF));
	add_to_scope(&global, create_cfunc("mod", modulo, 2, ARG_INF));
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

void eval_args(OBJ* expr)
{
	for (int i = 0; i <= expr->list_i; i++) {
		expr->list[i] = eval(expr->list[i]);
	}
}


OBJ eval(OBJ expr)
{
	// print_obj_full(*expr);
	if (expr.type != T_EXPR && expr.type != T_FUNC) {
		return expr;
	}

	// printf("gonna try and find: |%s|\n", expr.name);
	// printf("fuck: %s<%s> %d\n", expr.list[0].name, type_name(expr.list[0].type), expr.list[0].type);
	OBJ obj = find_in_scope(global, expr.name);
	if (obj.type == T_EXPR || obj.type == T_FUNC) {
		eval_args(&expr);
	}
	// printf("name: %s<%s> %d\n", expr.name, type_name(obj.type), obj.type);
	// if (obj.type = T_NULL)
		// lobotomy_warning("function undefined");
	// eval_args(&expr);


	// printf("found obj: %d |%s| %ld\n", obj.type, obj.name, obj.number);
	// print_scope(global);
	// printf("found |%s| in global\n", obj.name);
	switch (obj.type)
	{
		case 0:
			lobotomy_warning("uninitialized variable|expr");
			return undefined();
		break;

		case T_EXPR:
			return eval(obj);
		break;

		case T_NULL:
			lobotomy_warning("variable: %s <%s> is NULL", obj.name, type_name(obj.type));
			return obj;
		break;

		case T_IDENTIFIER:
			printf("huih\n");
			lobotomy_warning("variable: %s <%s> is uninitialized", obj.name, type_name(obj.type));
			return undefined();
		break;


		case T_FUNC:
			eval_args(&obj);
			return obj;
			// return obj;
			// return undefined();
		break;

		case T_CFUNC:
			// printf("function\n");
			return obj.func.func_ptr(expr);
		break;

		case T_LIST:
			return obj;
		break;

		default:
			return obj;
		break;
	}
	return undefined();
}


void interpret(const char text[])
{
	TREE tree = parse(text);
	// printf("\n\n---------------EVAL--------------------\n\n");
	for (int i = 0; i < tree.index; i++) {
		if (tree.expr[i].type == 0)
			break;
		// print_obj_full(tree.expr[i]);
		print_obj(eval(tree.expr[i]));
		printf("\n");
		// print_obj_full(eval(tree.expr[i]));
		// print_scope(global);
		// eval(tree.expr[i]);
	}
	// printf("\n\n---------------------------------------\n\n");
}

void interpret_(const char text[])
{
	TREE tree = parse(text);
	// printf("\n\n---------------EVAL--------------------\n\n");
	for (int i = 0; i < tree.index; i++) {
		if (tree.expr[i].type == 0)
			break;
		eval(tree.expr[i]);
	}
	// printf("\n\n---------------------------------------\n\n");

}


