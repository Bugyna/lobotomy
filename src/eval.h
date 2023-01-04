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
#define arithmetic_shorthand(a, b, op) a op##= b


enum
{
	ARG_NONE=-1,
	ARG_INF=-2,
};

OBJ* find_var(OBJ*, int);
void eval_args(OBJ*);
void eval_args_new(OBJ*);
OBJ eval(OBJ*);
OBJ eval_c(OBJ);


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
	// eval_args(obj);
	OBJ res;
	res.type = T_STR;
	res.str = type_name(obj->list[1].type);
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


// void arithmetic_shorthand_int(int64_t* a, int64_t* b, char op)
// {
	// switch (op)
	// {
		// case '+':
			// *a += *b;
		// break;

		// case '-':
			// *a -= *b;
		// break;

		// case '*':
			// *a *= *b;
		// break;

		// case '/':
			// *a /= *b;
		// break;

		// case '%':
			// *a %= *b;
		// break;

		// case '&':
			// *a &= *b;
		// break;

		// case '|':
			// *a |= *b;
		// break;

		// case '^':
			// *a ^= *b;
		// break;
	// }
// }



OBJ arithmetic_operation(OBJ* expr)
{
	// printf("Abwedd O: %s %s %ld %ld\n", expr->list[1].name, type_name(expr->list[1].type), expr->list[1].number, find_in_scope(global, "a").number);
	// eval_args(expr);
	// printf("AO: %s %s %ld %ld\n", expr->list[1].name, type_name(expr->list[1].type), expr->list[1].number, find_in_scope(global, "a").number);
	char op = expr->list[0].name[0];
	// printf("AO: %ld %ld | op: %c\n", expr->list[1].number, expr->list[2].number, op);
	OBJ res;
	int i = 1;

	if (expr->index > 2) {
		res = expr->list[1];
		if (res.type == T_EXPR || res.type == T_IDENTIFIER)
			res = eval(&res);
		else if (res.type == T_REF)
			res = *res.ref;
		i = 2;
	}
	else {
		res.type = T_NUMBER;
		res.number = 0;
	}

	for (; i < expr->index; i++) {
		if (expr->list[i].type == T_UNDEFINED)
			break;
		OBJ tmp = expr->list[i];
		switch (tmp.type)
		{
			case T_EXPR: case T_IDENTIFIER:
				tmp = eval(&tmp);
			break;
			// case T_NUMBER:
				// break;

			// case T_DECIMAL:
				// if (tmp.type == T_NUMBER)
					// arithmetic_shorthand_int((int64_t*)&res.decimal, &tmp.number, op);
				// else if (tmp.type == T_DECIMAL)
					// arithmetic_shorthand_int((int64_t*)&res.decimal, (int64_t*)&tmp.decimal, op);
				// break;
		}
		switch (op)
		{
			case '+':
				arithmetic_shorthand(res.number, tmp.number, +);
				break;
			case '-':
				arithmetic_shorthand(res.number, tmp.number, -);
				break;
			case '*':
				arithmetic_shorthand(res.number, tmp.number, *);
				break;
			case '/':
				arithmetic_shorthand(res.number, tmp.number, /);
				break;
			case '%':
				arithmetic_shorthand(res.number, tmp.number, %);
				break;
			case '&':
				arithmetic_shorthand(res.number, tmp.number, &);
				break;
			case '|':
				arithmetic_shorthand(res.number, tmp.number, |);
				break;
			case '^':
				arithmetic_shorthand(res.number, tmp.number, ^);
				break;
		}
	}
	return res;
}


OBJ lobotomy_if(OBJ* expr)
{
	OBJ ret;
	OBJ res = eval(&expr->list[1]);
	if (res.number == 1) {
		ret = eval(&expr->list[2]);
	}

	else {
		ret = eval(&expr->list[3]);
	}


	return ret;
}


bool obj_eq(OBJ a, OBJ b)
{
	int ret = 0;
	// eval_args(expr);

	// printf("eq: %s %s\n", type_name(expr->list[1].type), type_name(expr->list[2].type));

	if (a.type == b.type) {
		switch (a.type) {
			case T_NUMBER:
				if (a.number == b.number) {
					// printf("heredd\n");
					ret = 1;
				}
			break;

			default:
				ret = 0;
			break;
		}
	}

	return ret;
}

bool obj_less_than(OBJ a, OBJ b)
{
	int ret = 0;
	if (a.type == T_REF)
		a = *a.ref;
	if (a.type == b.type) {
		switch (a.type) {
			case T_NUMBER:
				if (a.number < b.number) {
					ret = 1;
				}
			break;

			default:
				ret = 0;
			break;
		}
	}

	return ret;
}

bool obj_less_than_or_eq(OBJ a, OBJ b)
{
	int ret = 0;
	if (a.type == b.type) {
		switch (a.type) {
			case T_NUMBER:
				if (a.number <= b.number) {
					ret = 1;
				}
			break;

			default:
				ret = 0;
			break;
		}
	}


	return ret;
}

OBJ lobotomy_equals(OBJ* expr)
{
	eval_args(expr);
	OBJ ret;
	ret.type = T_NUMBER;
	ret.number = obj_eq(expr->list[1], expr->list[2]);
	return ret;
}

OBJ lobotomy_not_equals(OBJ* expr)
{
	eval_args(expr);
	OBJ ret;
	ret.type = T_NUMBER;
	ret.number = !obj_eq(expr->list[1], expr->list[2]);
	return ret;
}

OBJ lobotomy_less_than(OBJ* expr)
{
	// TODO: remove eval_args from everywhere and replace with something more sensible
	eval_args_new(expr);
	// eval_args(expr);
	OBJ ret;
	ret.type = T_NUMBER;
	ret.number = obj_less_than(expr->list[1], expr->list[2]);
	return ret;
}

OBJ lobotomy_more_than(OBJ* expr)
{
	eval_args(expr);
	OBJ ret;
	ret.type = T_NUMBER;
	ret.number = !obj_less_than_or_eq(expr->list[1], expr->list[2]);
	return ret;
}

OBJ lobotomy_less_than_or_eq(OBJ* expr)
{
	eval_args(expr);
	OBJ ret;
	ret.type = T_NUMBER;
	ret.number = obj_less_than_or_eq(expr->list[1], expr->list[2]);
	printf("ltoq: %ld\n", ret.number);
	return ret;
}

OBJ lobotomy_more_than_or_eq(OBJ* expr)
{
	eval_args(expr);
	OBJ ret;
	ret.type = T_NUMBER;
	ret.number = !obj_less_than(expr->list[1], expr->list[2]);
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
	// eval_args(expr);
	// expr->type = T_LIST;
	for (int i = 1; i < expr->index; i++) {
		// print_obj_type(expr->list[i]);
		if (expr->list[i].type == T_UNDEFINED)
			break;

		else if (expr->list[i].type == T_EXPR || expr->list[i].type == T_LIST || expr->list[i].type == T_IDENTIFIER)
			print_obj_(eval(&expr->list[i]));

		else
			print_obj_(expr->list[i]);
	}
	return *expr;
}

OBJ lobotomy_let(OBJ* expr)
{
	// expr->list[1] = eval(&expr->list[1]);
	OBJ id = expr->list[1];
	// eval_args(expr);
	// printf("let type b: %s\n", type_name(expr->list[2].type));
	// print_obj(expr->list[2]);
	// print_obj(*expr);
	OBJ val = eval(&expr->list[2]);
	// printf("let type: %s\n", type_name(val.type));
	// print_obj(val);
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
	// printf("let: %s\n", expr->scope->name);
	if (id.type == T_REF)
	{;
		*id.ref = val;
	}
	else 
		id = *add_to_scope(expr->scope, create_var(id.name, val));
	// printf("id: %ld\n", find_in_scope(global, id.name).number);
	return id;
	// }
}

OBJ lobotomy_make_ref(OBJ* expr)
{
	eval_args_new(&expr->list[1]);
	return eval(&expr->list[1]);
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

OBJ lobotomy_macro(OBJ* expr)
{
	// printf("creating lob func\n");
	// OBJ obj = create_func(expr->list[1].name, expr);
	OBJ obj = find_in_scope(*expr->scope, expr->list[2].name);
	obj.name = expr->list[1].name;
	// printf("creating lob func %s %s\n", expr->list[1].name, obj.name);
	add_to_scope(&global, obj);
	return obj;
	// return undefined();
}

OBJ lobotomy_loop(OBJ* expr)
{
	// printf("start loop %s\n", expr->list[2].scope->name);
	bool f = 1;
	int i = 0;
	OBJ ret = undefined();
	eval_args_new(&expr->list[2]);
	// for (int i = 0; i < expr->list[2].index; i++)
	// {
		// if (expr->list[2].list[i].type == T_IDENTIFIER) {
			// expr->list[2].list[i].ref = find_var(expr, i);
			// expr->list[2].list[i].type = T_REF;
		// }
	// }
	// const OBJ tmp = expr->list[2];
	// OBJ tmp = create_copy(&expr->list[1]);
	// OBJ tmp1 = create_copy(&expr->list[2]);
	// OBJ cond = expr->list[1];
	// *expr = create_copy(&tmp1);
	// printf("loop\n");
	if (expr->list[1].type == T_NUMBER)
	{
		lobotomy_warning("infinite loop");
		while (expr->list[1].number)
		{
			ret = eval(&expr->list[2]);
		}
		return ret;
	}

	while (f) {
		// for (int i = 1; i < tmp1.index; i++) {
		// if (tmp1.list[i].type == T_IDENTIFIER)
			// tmp1.list[i] = find_var(&tmp1, i);
		// }
		// *expr = create_copy(&tmp1);
		// cond = create_copy(&tmp);
		if (!eval(&expr->list[1]).number) {
		// if (i++ > 10000) {
			f = 0;
			break;
		}
		// printf("aaaaaa: %d\n", i);
		// print_obj(expr->list[2]);
		// printf("------\n");
		// printf("loop: %d %s %s %ld %ld\n", i++, expr->list[2].list[1].name, type_name(expr->list[2].list[1].type), expr->list[2].list[1].number, find_in_scope(global, "a").number);
		ret = eval(&expr->list[2]);
		// print_obj(ret);
		// printf("id: %ld\n", find_in_scope(global, "a").number);
		// print_obj(ret);
	}


	return ret;
	// return undefined();
}


OBJ obj_nth(OBJ obj, int n)
{
	printf("here %d %s\n", n, type_name(obj.type));
	OBJ res;
	if (obj.type != T_LIST && obj.type != T_FUNC && obj.type != T_STR) {
		printf("aaaaa\n");
		return undefined();
	}


	switch (obj.type)
	{
		case T_LIST: case T_FUNC:
			res = obj.list[n];
		break;

		case T_STR:
			res.type = T_STR;
			res.str = malloc(2);
			res.str[0] = obj.str[n];
			res.index = 2;
		break;
			
	}


	return res;
}

OBJ lobotomy_first(OBJ* expr)
{
	eval_args(expr);
	if (expr->list[1].type == T_LIST || expr->list[1].type == T_STR) {
		return obj_nth(expr->list[1], 0);
	}

	else {
		lobotomy_error("invalid type in function first expected LIST or STR, but got %s\n", type_name(expr->list[1].type));
	}
}

OBJ lobotomy_nth(OBJ* expr)
{
	eval_args(expr);
	if (expr->list[2].type == T_LIST || expr->list[2].type == T_STR) {
		return obj_nth(expr->list[2], expr->list[1].number);
	}

	else {
		lobotomy_error("invalid type in function first expected LIST or STR, but got %s\n", type_name(expr->list[2].type));
	}
}

OBJ lobotomy_last(OBJ* expr)
{
	eval_args(expr);
	if (expr->list[1].type == T_LIST || expr->list[1].type == T_STR) {
		return obj_nth(expr->list[1], expr->list[1].index-1);
	}

	else {
		lobotomy_error("invalid type in function first expected LIST or STR, but got %s\n", type_name(expr->list[1].type));
	}
}


void init()
{

	// add_to_scope(&global, create_cfunc("", lobotomy_, 1, 1));
	
	add_to_scope(&global, create_cfunc("-", arithmetic_operation, 1, ARG_INF));
	add_to_scope(&global, create_cfunc("+", arithmetic_operation, 1, ARG_INF));
	add_to_scope(&global, create_cfunc("*", arithmetic_operation, 2, ARG_INF));
	add_to_scope(&global, create_cfunc("/", arithmetic_operation, 2, ARG_INF));
	add_to_scope(&global, create_cfunc("%", arithmetic_operation, 2, ARG_INF));
	add_to_scope(&global, create_cfunc("pso", print_scope_obj, ARG_NONE, ARG_INF));
	add_to_scope(&global, create_cfunc("let", lobotomy_let, 2, 2));
	add_to_scope(&global, create_cfunc("eq", lobotomy_equals, ARG_INF, ARG_INF));
	
	add_to_scope(&global, create_cfunc("=", lobotomy_equals, 2, 2));
	add_to_scope(&global, create_cfunc("!", lobotomy_not_equals, 2, 2));
	add_to_scope(&global, create_cfunc("<", lobotomy_less_than, 2, 2));
	add_to_scope(&global, create_cfunc(">", lobotomy_more_than, 2, 2));
	add_to_scope(&global, create_cfunc("<=", lobotomy_less_than_or_eq, 2, 2));
	add_to_scope(&global, create_cfunc(">=", lobotomy_more_than_or_eq, 2, 2));

	add_to_scope(&global, create_cfunc("if", lobotomy_if, 3, ARG_INF));
	add_to_scope(&global, create_cfunc("?", lobotomy_if, 3, ARG_INF));
	add_to_scope(&global, create_cfunc("loop", lobotomy_loop, 3, 3));

	add_to_scope(&global, create_cfunc("func", lobotomy_func, 3, 3));
	add_to_scope(&global, create_cfunc("macro", lobotomy_macro, 3, 3));

	add_to_scope(&global, create_cfunc("first", lobotomy_first, 1, 1));
	add_to_scope(&global, create_cfunc("nth", lobotomy_nth, 2, 2));
	add_to_scope(&global, create_cfunc("last", lobotomy_last, 1, 1));
	
	
	add_to_scope(&global, create_cfunc("print", lobotomy_print, 1, ARG_INF));
	add_to_scope(&global, create_cfunc("type", lobotomy_type, 1, 1));
	add_to_scope(&global, create_cfunc("undefined", undefined, 0, ARG_INF));
	add_to_scope(&global, create_cfunc("make_ref", lobotomy_make_ref, 0, ARG_INF));
	add_to_scope(&global, create_cfunc("exit", lobotomy_exit, ARG_NONE, ARG_INF));
	// add_to_scope(&global, (OBJ){.type=T_NUMBER, .name="a", .number = 2});
	

	// OBJ test;
	// test.type = T_NUMBER;
	// test.number = 5;

	// add_to_scope(&global, create_var("a", test));
	// print_scope(global);

}

OBJ rr(OBJ* o) { OBJ p; p = *o; return p; }

void apply_scope(OBJ* expr, SCOPE* scope)
{
	expr->scope = scope;
	// printf("here %d %s\n", expr->index, expr->list[0].name);
	for (int i = 0; i < expr->index; i++) {
		// printf("i: %d\n", i);
		if (expr->list[i].type == T_UNDEFINED)
			break;

		else if (expr->list[i].type == T_LIST) {
			// printf("a\n");
			apply_scope(&expr->list[i], scope);
		}

	}
}

OBJ run_func(OBJ func, OBJ* expr)
{
	add_func_scope(&global, func.name, func.list[0].index);
	func.scope = &global.func_scope[global.func_index-1];
	func.list[1].scope = &global.func_scope[global.func_index-1];
	func = create_copy(&func);
	eval_args(expr);
	// print_obj_type(func.list[1]);+

	for (int i = 0; i < func.list[0].index; i++) {
		if (func.list[0].list[i].type != T_UNDEFINED && func.list[0].list[i].name != NULL) {
			expr->list[i+1].name = func.list[0].list[i].name;
			// printf("adding: %s %ld to scope\n", expr->list[i+1].name, expr->list[i+1].number);
			add_to_scope(&global.func_scope[global.func_index-1], expr->list[i+1]);
		}
	}

	// for (int i = 0; i < func.list[1].index; i++) {
		// for (int j = 0; j < func.list[0].index; j++) {
		// if (func.list[1].list[i].name != NULL && func.list[0].list[j].name != NULL && strcmp(func.list[1].list[i].name, func.list[0].list[j].name) == 0) {
			// printf("ok %s %s\n", func.list[1].list[i].name, expr->list[j].name);
			// expr->list[j+1].name = func.list[1].list[i].name;
			// func.list[1].list[i] = expr->list[j+1];
		// }
		// printf("fc: %s: %ld\n", func.list[1].list[i].name, find_in_scope(global.func_scope[global.func_index-1], func.list[1].list[1].name).number);
		// }
	// }
	// apply_scope(&func.list[1], &global.func_scope[global.func_index-1]);
	// printf("fp: %p\n", &func.list[1]);
	// func.list[1].scope = &global.func_scope[global.func_index-1];
	// printf("fc: %s: %ld\n", func.list[1].list[1].name, find_in_scope(global.func_scope[global.func_index-1], func.list[1].list[1].name).number);
	// scope_init(&global.func_scope[global.func_index], 5);
	// printf("ffffuck: %s\n", func.list[1].scope->name);
	OBJ res = eval(&func.list[1]);
	return res;

	// return undefined();
}


OBJ* find_var(OBJ* expr, int i)
{
	OBJ* tmp = NULL;
	if (expr->type != T_FUNC) {
		tmp = find_ptr_in_scope(expr->scope, expr->list[i].name);
		// printf("here in eval %s %s\n", tmp.name, expr->list[i].name);
		if (tmp->type == T_UNDEFINED) {
			// printf("fuckery\n");
			tmp = find_ptr_in_scope(&global, expr->list[i].name);
			// printf("found: %ld\n", tmp.number);

			// if (tmp->type == T_UNDEFINED) {
			if (tmp == NULL) {
				lobotomy_error_s(ERR_UNDEFINED, "%s %s is undefined", expr->list[i].name, expr->scope->name);
			}
		}

		// print_obj_type(tmp);
		// expr->list[i] = tmp;
	}
	return tmp;
}

void eval_args_new(OBJ* expr)
{
	for (int i = 1; i < expr->index; i++) {
		if (expr->list[i].type == T_LIST || expr->list[i].type == T_EXPR)
			eval_args_new(&expr->list[i]);
		else if (expr->list[i].type == T_IDENTIFIER) {
				expr->list[i].ref = find_var(expr, i);
				expr->list[i].type = T_REF;
		}
	}
}

void eval_args(OBJ* expr)
{
	OBJ tmp;
	for (int i = 0; i <= expr->index; i++) {
		// printf("expr naem: %s %s\n", type_name(expr->list[i].type), expr->list[i].name);
		// printf("expr: %i %s\n", i, type_name(expr->list[i].type));
		if (expr->list[i].type == T_CFUNC)
			continue;

		else if (expr->list[i].type == T_IDENTIFIER) {
			// printf("looking for: %s in %s\n", expr->list[i].name, expr->scope->name);
				// printf("here %s %s\n", expr->list[i].name, expr->scope->name);
			expr->list[i].ref = find_var(expr, i);
			expr->list[i].type = T_REF;
			// if (expr->type != T_FUNC && find_in_scope(global, expr->list[i].name).type != T_UNDEFINED)
				// expr->list[i] = find_in_scope(global, expr->list[i].name);
			
			// else if (find_in_scope(global.func_scope[global.func_index-1], expr->list[i].name).type != T_UNDEFINED) {
				// expr->list[i] = find_in_scope(global.func_scope[global.func_index-1], expr->list[i].name);
			// }
		}

		// else if (expr->list[i].type == T_REF) {
			// printf("dda\n");
			// expr->list[i] = *expr->list[i].ref;
			// print_obj(expr->list[i]);
		// }
		
		else if (expr->list[i].type == T_LIST)
			expr->list[i] = eval(&expr->list[i]);
		// printf("eval_args: %s\n", type_name(expr->list[i].type));
	}
}

OBJ eval_c(OBJ expr)
{
	// if (scope == NULL)
		// scope = &global;
	// if (expr.type == T_LIST) {
		// eval_args(&expr);
		// return expr;
	// }

	if (expr.type != T_LIST && expr.type != T_CFUNC && expr.type != T_FUNC) {
		return expr;
	}

	// printf("gonna try and find: |%s|\n", expr.name);
	// printf("fuck: %s<%s> %d\n", expr->list[0].name, type_name(expr->list[0].type), expr->list[0].type);


	// eval_args(&expr);
	OBJ obj = expr.list[0];
	if (expr.list[0].type == T_IDENTIFIER) {
		// printf("here\n");
		obj = find_in_scope(global, expr.list[0].name);
		expr.list[0] = obj;
	}
	// OBJ obj = undefined();

	// printf("name: %s<%s> %d\n", obj.name, type_name(obj.type), obj.type);


	// printf("found obj: %d |%s| %ld\n", obj.type, obj.name, obj.number);
	// print_scope(global);
	// printf("found |%s| in global\n", obj.name);

	switch (obj.type)
	{
		case T_UNDEFINED:
			lobotomy_warning("%s is undefined", expr.name);
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
			return run_func(obj, &expr);
			// obj.list[0].ref = &expr.list[1];
			// return undefined();
			// eval_args(&obj);
			// return obj;
		break;

		case T_CFUNC:
			// printf("cfunc\n");
			return obj.func(&expr);
		break;

		case T_REF:
			return *obj.ref;
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

OBJ eval(OBJ* expr)
{
	// if (scope == NULL)
		// scope = &global;
	// if (expr.type == T_LIST) {
		// eval_args(&expr);
		// return expr;
	// }
	// printf("ffffff: %s %s\n", expr->name, expr->scope->name);
	if (expr->type == T_IDENTIFIER) {
		return find_in_scope(global, expr->name);
	}
	if (expr->type != T_LIST && expr->type != T_CFUNC && expr->type != T_FUNC) {
		return *expr;
	}

	// printf("gonna try and find: |%s|\n", expr.name);
	// printf("fuck: %s<%s> %d\n", expr->list[0].name, type_name(expr->list[0].type), expr->list[0].type);


	// eval_args(&expr);
	OBJ obj = expr->list[0];
	// print_obj_type(expr->list[0]);
	if (obj.type == T_IDENTIFIER) {
		obj = find_in_scope(global, expr->list[0].name);
		expr->list[0] = obj;
		// print_obj_type(expr->list[1]);
	}
	// else
		// return *expr;
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
			// lobotomy_warning("variable: %s <%s> is uninitialized", obj.name, type_name(obj.type));
			obj = find_in_scope(*obj.scope, obj.name);
			return obj;
			// return undefined();
			// return expr;
		break;


		case T_FUNC:
			// printf("p: %p\n", &obj.list[1]);
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


