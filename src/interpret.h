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

void do_bs(OBJ* expr);

void lobotomy_error(const char* msg)
{
	printf("ERROR: %s\n", msg);
	exit(-1);
}

void lobotomy_warning(const char* msg)
{
	printf("WARNING: %s\n", msg);
}


OBJ eval(OBJ);



OBJ lobotomy_let(OBJ expr)
{
	do_bs(&expr);
	OBJ id = expr.list[0];
	OBJ val = expr.list[1];
	printf("creating var: %s %s -> %s\n", id.name, type_name(id.type), type_name(val.type));
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
	do_bs(&expr);
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
			if (type == T_EXPR)
				lobotomy_warning("trying to cast function to expr");

			else
				lobotomy_warning("trying to cast function to whatever");
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
	do_bs(&expr);
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
	
}

OBJ lobotomy_equals(OBJ expr)
{
	OBJ ret;
	ret.type = T_NUMBER;
	do_bs(&expr);

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
	do_bs(&expr);
	expr.type = T_LIST;
	// for (int i = 0; i < expr.list_i; i++) {
		// print_obj(expr.list[i]);
	// }
	return expr;
	
}

OBJ lobotomy_fn(OBJ expr)
{
	
}

void init()
{
	add_to_scope(&global, create_fn("minus", minus));
	add_to_scope(&global, create_fn("plus", plus));
	add_to_scope(&global, create_fn("+", plus));
	add_to_scope(&global, create_fn("mul", multiply));
	add_to_scope(&global, create_fn("div", divide));
	add_to_scope(&global, create_fn("mod", modulo));
	add_to_scope(&global, create_fn("exit", lobotomy_exit));
	add_to_scope(&global, create_fn("pso", print_scope_obj));
	add_to_scope(&global, create_fn("let", lobotomy_let));
	add_to_scope(&global, create_fn("eq", lobotomy_equals));
	add_to_scope(&global, create_fn("=", lobotomy_equals));
	add_to_scope(&global, create_fn("if", lobotomy_if));
	add_to_scope(&global, create_fn("?", lobotomy_if));

	add_to_scope(&global, create_fn("fn", lobotomy_fn));
	
	add_to_scope(&global, create_fn("print", lobotomy_print));
	// add_to_scope(&global, create_fn("set", lobotomy_set));
	add_to_scope(&global, create_fn("type", lobotomy_type));
	

	// OBJ test;
	// test.type = T_NUMBER;
	// test.number = 5;

	// add_to_scope(&global, create_var("a", test));
	print_scope(global);

}


void do_bs(OBJ* expr)
{
	for (int i = 0; i <= expr->list_i; i++) {
		// printf("motherfucker: %d: %d\n", i, expr->list[i].type);
		if (expr->list[i].type == T_EXPR) {
			expr->list[i] = eval(expr->list[i]);
		}


		else if (expr->list[i].type == T_IDENTIFIER) {
			// printf("fuckery %d\n", i);
			// expr->list[i] = find_in_scope(global, expr->list[i].name);
			expr->list[i] = *expr->list[i].ref;
			// printf("after: %d: %d\n", i, expr->list[i].type);
		}

		// else
			// printf("what %d\n", i);
	}
}

OBJ eval(OBJ expr)
{
	// print_obj_full(*expr);
	if (expr.type != T_EXPR) {
		return expr;
	}

	// printf("gonna try and find: |%s|\n", expr.name);
	OBJ obj = find_in_scope(global, expr.name);
	// do_bs(&expr);


	// printf("found obj: %d |%s| %ld\n", obj.type, obj.name, obj.number);
	// print_scope(global);
	// printf("found |%s| in global\n", obj.name);
	switch (obj.type)
	{
		case 0:
			return undefined();
		break;

		case T_EXPR:
			return eval(obj);
		break;

		case T_IDENTIFIER:
			return *obj.ref;
		break;

		case T_FUNC:
			// printf("function\n");
			return obj.fn(expr);
		break;

		case T_LIST:
			return obj;
		break;

		default:
			return obj;
		break;
	}

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



