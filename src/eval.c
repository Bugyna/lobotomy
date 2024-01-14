#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <dlfcn.h>

#include "util.h"
#include "lexer.h"
#include "parse.h"
#include "l_std.h"
#include "obj.c"


#include "eval.h"




OBJ* L_less_than(OBJ* o)
{
	/*!
			 DOC
	*/
	OBJ* ret = empty_obj();
	ret->type = T_NUM;
	o = preeval(o);

	// if (o->type == T_IDENTIFIER)
		// ret->num = (o->car->num < NT(o)->num);
	// else
	ret->num = (o->num < NT(o)->num);

	return ret;
}

OBJ* L_more_than(OBJ* o)
{
	OBJ* ret = empty_obj();
	ret->type = T_NUM;
	o = preeval(o);
	ret->num = (o->num > NT(o)->num);
	return ret;
}


OBJ* L_less_or_eq_than(OBJ* o)
{
	OBJ* ret = empty_obj();
	ret->type = T_NUM;
	o = preeval(o);
	ret->num = (o->num <= NT(o)->num);
	return ret;
}

OBJ* L_more_or_eq_than(OBJ* o)
{
	OBJ* ret = empty_obj();
	ret->type = T_NUM;
	o = preeval(o);
	ret->num = (o->num >= NT(o)->num);
	return ret;
}

OBJ* L_eq(OBJ* o)
{
	OBJ* ret = empty_obj();
	ret->type = T_NUM;
	o = preeval(o);
	switch (o->type)
	{
		case T_NUM: T_DECIMAL:
			if (NT(o)->type == T_NUM || NT(o)->type == T_DECIMAL) {
				ret->num = (o->num == NT(o)->num);
			}
			else {
				lobotomy_error("Invalid types for comparison: trying to compare '%s' and '%s'\n", type_name(o->type), type_name(NT(o)->type));
			}
		break;

		case T_STR:
			if (NT(o)->type == T_STR) {
				ret->num = !strcmp(o->str, NT(o)->str);
			}
			else {
				lobotomy_error("Invalid types for comparison: trying to compare '%s' and '%s'\n", type_name(o->type), type_name(NT(o)->type));
			}
		break;
	}
	
	return ret;
}


OBJ* L_car(OBJ* o)
{
	o = preeval(o);
	print_obj_simple(o->car->cdr);
	return o->car;
	// return NIL;
}



OBJ* L_get_input(OBJ* o)
{
	if (o != NULL && o->type != T_NIL)
		__print_obj_simple(o);
	char x[200] = "";
	fgets(x, 200, stdin);
	size_t l = strlen(x);

	OBJ* ret = empty_obj();
	ret->type = T_STR;
	ret->str = malloc(l);
	x[l-1] = '\0';
	strcpy(ret->str, x);
	return ret;
}


OBJ* L_cdr(OBJ* o)
{
	o = preeval(o);
	OBJ* tmp = empty_obj();
	tmp->type = T_LIST;
	tmp->car = o->cdr;
	return tmp;
	// print_objf("obj: ", o->cdr);
	return o->cdr;
}

OBJ* L_list(OBJ* o)
{
	OBJ* tmp = empty_obj();
	tmp->type = T_LIST;
	tmp->car = o;
	return tmp;
	// return o->cdr;
}

OBJ* L_nth(OBJ* o)
{
	o = preeval(o);
	OBJ* list = NT(o);
	OBJ* tmp = list->car;
	for (int i = 0; i < o->num; i++) {
		tmp = NT(tmp);
	}

	return L_copy(tmp);
}


OBJ* L_exit(OBJ* o)
{
	if (o == NULL)
		exit(0);
	else
		exit(o->num);
}


OBJ* L_progn(OBJ* o)
{
	// ITERATE_OBJ(o, curr)
	// {
		
	// }
	return NIL;
}


OBJ* L_test(OBJ* o)
{
	// printf("pp: %p\n", o->cdr->cdr);
	o = preeval(o);
	return NIL;
}


OBJ* L_print(OBJ* o)
{
	// o = NT(o);
	// o = preeval(o);
	// printf("type: %s\n", type_name(o->cdr->type));
	OBJ* tmp = preeval(o);
	printf("print>>");
	print_obj_full(tmp);
	// print_obj_simple(L_list(tmp));

	// ITERATE_OBJECT(NT(tmp), curr)
	// {
		// __print_obj_simple(__eval(curr));
	// }
	// printf("\n");
	// print_objf("print>>", NT(tmp));
	return NIL;
}

OBJ* L_type(OBJ* o)
{
	preeval(o);
	printf("%s\n", type_name(o->type));
	return NIL;
}


OBJ* L_help(OBJ* o)
{
	for (int i = 0; i < o->env->size; i++) {
		if (o->env->list[i].key != NULL)
			printf("::> '%s'\n", o->env->list[i].key);
	}
	return NIL;
}

OBJ* L_create_fn(OBJ* o)
{
	OBJ* fn = empty_obj();
	OBJ* name = o;
	OBJ* args = NT(name);
	OBJ* body = NT(args);

	// print_objf("fn: ", body);

	fn->name = name->name;
	fn->type = T_FN;
	fn->args=args;
	fn->body=body;
	env_add(global_env, fn);
	return fn;
}

OBJ* L_obj_name(OBJ* o)
{
	OBJ* ret = NEW();
	ret->type = T_STR;
	ret->str = o->name;
	return ret;
}


OBJ* L_let(OBJ* o)
{
	ENV* e = o->env;
	OBJ* var = o;
	if (var->type != T_IDENTIFIER) {
		if (var->name != NULL && var->name != "") var->type = T_IDENTIFIER;
		else var = preeval(var);
	}
	OBJ* val = NT(var);
	val = preeval(val);
	printd("let: %s %s\n", var->name, var->env->name);
	OBJ* ret = ENV_GET(var->env, var->name); // can't use env_get because we have to check only the scope we're currently in

	if (ret == NULL)
	{

		if (val->type == T_EXPR){printf("xxx\n"); ret = __eval(val->car);}
		else ret = L_copy(val);
		ret->name = var->name;

		// printf("\npp: %s\n", ret->name);
		// printf("blabla: %s :", ret->name); print_obj_simple(ret);
		env_add(e, ret);
		// print_obj_simple(ENV_GET(&global_env, var->name));
		return ret;
	}

	if (val->type == T_EXPR)
		*ret = *__eval(val->car);
	else
		*ret = *val;

	ret->name = var->name;
	// printf("blabla: "); print_obj_simple(ret);


	return L_copy(ret);


	// if (var->car == NULL)
	// {
		// printf("\neee\n");
		// if (val->type == T_EXPR)
		// {
			// printf("xxx\n");
			// OBJ* tmp = __eval(val->car);
			// var->car = malloc(tmp);
			// *var->car = *tmp;
		// }
		// else
		// {
			// var->car = malloc(sizeof(val));
			// *var->car = *L_copy(val);
		// }
		// // var->car->name = var->name;

		// // printf("\npp: %s\n", ret->name);
		// printf("blabla: "); print_obj_simple(var->car);
		// var->car = ENV_ADD(&global_env, var->name, var->car);
		
		// return var;
	// }

	// if (val->type == T_EXPR){printf("xxx\n"); *var->car = *__eval(val->car);}
	// else
	// {
		// *var->car = *val;
	// }
	
	// return var;
}


OBJ* L_loop(OBJ* o)
{
	OBJ* cond_expr = o;
	// cond_expr = preeval(cond_expr);
	// cond_expr->car = preeval_symbols(cond_expr->car);
	// printf("cond_exprp: %s", type_name(cond_expr->cdr->car->type));
	// print_objf("cond_expr: ", cond_expr);

	OBJ* exec_expr = NT(cond_expr);
	// exec_expr = preeval(exec_expr);
	// exec_expr->car = preeval_symbols(exec_expr->car);
	// print_obj_simple(cond_expr);

	// print_objf("exec_expr: ", exec_expr);

	// preeval(cond_expr);
	// preeval(exec_expr);
	OBJ* ret = NIL;

	printd("loop starting now\n");

	while (1)
	{
		OBJ* tmp = __eval(cond_expr->car);
		// printf("tmp: "); print_obj_simple(tmp);
		// print_objf("cond_expr: ", cond_expr);
		// print_objf("exec_expr: ", exec_expr);

		// (loop (< a 10) (let a (+ a 2)))
		if (!tmp->num)
		{
			// printf("aaaaa: "); print_obj_simple(tmp);
			break;
			goto ret;
		}
		// (loop (a) (let a (+ a 2)))
			
		// printf("aaaaa");
		ret = __eval(exec_expr->car);
		// __print_obj_full(__eval(exec_expr->car));
		// usleep(200);
		// sleep(1);
	}

	ret:
	return ret;
	// return NT(o);
}

OBJ* L_cond(OBJ* o)
{
	// OBJ* cond_expr = NT(o);
	// print_objf("cond_expr: ", cond_expr);

	// OBJ* exec_expr = NT(cond_expr);
	// print_objf("exec_expr: ", exec_expr);

	OBJ* ret = NIL;

	ITERATE_OBJECT_PAIR(o, cond_expr, exec_expr)
	{
		// print_objf("cond_expr: ", cond_expr);
		// print_objf("exec_expr: ", exec_expr);
		OBJ* tmp = __eval(cond_expr->car);
		// print_objf("tmp: ", tmp);


		if (tmp->num || tmp->type == T_TRUE) {
			// printd("aaa: %s\n", exec_expr->car->cdr->env->name);
			ret = __eval(exec_expr);
			goto ret;
		}
	}

	ret:
	return ret;
}

OBJ* L_copy(OBJ* o)
{
	OBJ* ret = empty_obj();
	*ret = *o;
	ret->cdr = NIL;
	return ret;
}

OBJ* L_get_random_num(OBJ* o)
{
	OBJ* ret = empty_obj_t(T_NUM);
	ret->num = time(NULL);
	// printf("t: %d", ret->num);
	if (o != NULL && o->type != T_NIL) {
		ret->cdr = o;
		ret = L_mod(ret);
		// o = preeval(NT(o));
		// ret->num %= o->num;
	}
	return ret;
}

OBJ* L_use(OBJ* o)
{
	eval_program_file(o->str);
	return NIL;
}

OBJ* L_load(OBJ* o)
{
	void* handle = NULL;
	load_lib_func fn = NULL;
	size_t len = strlen(o->str);
	char x[len+3];
	char xx[len+5];
	strcpy(x, o->str);
	strcpy(xx, o->str);

	strcat(x, ".so");
	strcat(xx, "_load");
	printd("strcat: %s\n", x);
	printd("strcat: %s\n", xx);

	handle = dlopen("./sample_module.so", RTLD_NOW | RTLD_GLOBAL);
	if (handle == NULL) {
		lobotomy_error("unable to load library '%s'\n", x);
	}
	fn = dlsym(handle, xx);
	if (fn == NULL) {
		lobotomy_error("unable to load library '%s' because %s couldn't be loaded\n", x, xx);
	}
	printd("load: %p\n", fn);
	fn(global_env);
	
	return NIL;
}

OBJ* preeval_symbols(OBJ* o)
{
	OBJ* ret = empty_obj();
	OBJ* head = ret;
	OBJ* tmp;

	ITERATE_OBJECT(o, curr)
	{
		switch (curr->type)
		{
			case T_IDENTIFIER:
				tmp = env_get(o->env, curr->name);
				if (tmp != NULL) {
					// ret->cdr = empty_obj();
					// *ret->cdr = *tmp;
					ret->cdr = curr;
					ret->cdr->type = T_REF;
					ret->cdr->car = tmp;
				}
			break;

			// case T_EXPR:
				// ret->cdr = preeval_symbols(curr->car);
			// break;
			
			default:
				ret->cdr = curr;
		}
		ret = ret->cdr;
	}

	tmp = head;
	head = NT(head);
	GC_free(tmp);
	return head;
}

OBJ* preeval(OBJ* o)
{
	OBJ* ooo = o;
	OBJ* ret = empty_obj();
	// DO_THIS(&o, ret);
	OBJ* head = ret;

	OBJ* tmp;
	ITERATE_OBJECT(o, curr)
	{
		switch (curr->type)
		{
			case T_IDENTIFIER:
				tmp = env_get(o->env, curr->name);
				if (tmp != NULL) {
					// ret->cdr = tmp;
					ret->cdr = empty_obj();
					*ret->cdr = *tmp;
				}
				else {
					lobotomy_error("Variable '%s' '%s' not found", curr->name, o->env->name);
				}
			break;
			
			case T_EXPR:
				// printf("occuring for: %s\n", curr->car->name);
				// ret->cdr = __eval(curr->car);
				ret->cdr = __eval(curr->car);
			break;

			case T_LIST:
				ret->cdr = empty_obj();
				*ret->cdr = *curr->cdr;
				ret->cdr->car = preeval(curr->car);
			break;

			case T_REF:
				ret->cdr = curr->car;
			break;

			default:
				ret->cdr = empty_obj();
				*ret->cdr = *curr;
		}
		ret = ret->cdr;
	}

	tmp = head;
	head = NT(head);
	GC_free(tmp);
	return head;

	ITERATE_OBJECT_PTR(o, curr)
	{
		if ((*curr)->type == T_IDENTIFIER)
		{
			OBJ* tmp = env_get(o->env, (*curr)->name);
			if (tmp != NULL) {
				*ret = *tmp;
				ret->cdr = empty_obj();
				// ret->cdr = (*curr)->cdr;
			}
			else {
				lobotomy_error("Variable '%s' '%s' not found", (*curr)->name, o->env->name);
			}
		}
		
		else if ((*curr)->type == T_EXPR) {
			*ret = *__eval((*curr)->car);
			ret->cdr = empty_obj();
		}

		else if ((*curr)->type == T_LIST) {
			// *ret = **curr;
			*ret = *preeval((*curr)->car);
			ret->cdr = empty_obj();
		}

		else {
			*ret = **curr;
		}
		
		ret = ret->cdr;
		// ret->cdr = empty_obj();
	}
	print_obj_simple(L_list(ooo));
	print_obj_simple(L_list(head));
	return head;
}


OBJ* progn(OBJ* o)
{
	/*!
		evaluate a list of expressions and return the last result
	*/
	OBJ* ooo = o;
	OBJ* ret = empty_obj();
	OBJ* head = ret;

	OBJ* tmp;
	ITERATE_OBJECT(o, curr)
	{
		printd("progn: %s\n", type_name(curr->type));
		ret->cdr = __eval(curr);
		ret = ret->cdr;
	}

	tmp = head;
	head = NT(head);
	GC_free(tmp);
	return ret;
}

void set_env(OBJ* head, ENV* e)
{
	ITERATE_OBJECT(head, curr)
	{
		if (curr->type == T_EXPR || curr->type == T_LIST) set_env(curr->car, e);
		curr->env = e;
	}
}

OBJ* run_func(OBJ* fn, OBJ* args)
{
	args = preeval(args);
	// print_objf("args: ", args);
	// print_objf("args: ", NT(args));
	ENV* e = malloc(sizeof(ENV));
	ENV_INIT(e, 20);
	e->name = "fn";
	e->parent = global_env;
	// OBJ* curr1 = fn;
	// ITERATE_OBJECT(args, curr)
	// {
		// print_objf("saaa: ", curr);
	// }
	// print_obj_simple(fn->fn.args);

	ZIP_ITERATE_OBJECT(fn->args->car, args, curr, curr1,
		{
			curr->env = e;
			curr1->env = e;
			// print_objf("saaa: ", curr);
			// print_objf("saaa1: ", curr1);
			curr1->name = curr->name;
			env_add(e, L_copy(curr1));
		}
	)
	// print_obj_simple(fn->body);
	set_env(fn->body, e);

	// printf("\ngot here\n");
	// print_objf("::: ", fn->body->car->cdr);

	if (fn->body->car->type == T_EXPR)
		return progn(fn->body->car);
	else return __eval(fn->body);
}


// void lobotomy_init()
// {
	// // ENV_ADD(global_env, "+", create_cfn("+", lobotomy_add));
	// ENV_ADD(global_env, "+", create_cfn("+", L_add));
	// ENV_ADD(global_env, "-", create_cfn("-", L_subtract));
	// ENV_ADD(global_env, "*", create_cfn("*", L_multiply));
	// ENV_ADD(global_env, "/", create_cfn("/", L_divide));

	// ENV_ADD(global_env, "&", create_cfn("&", L_and));
	// ENV_ADD(global_env, "|", create_cfn("|", L_or));
	// ENV_ADD(global_env, "xor", create_cfn("xor", L_xor));
	// // ENV_ADD(global_env, "%", create_cfn("%", L_modulo));
	// ENV_ADD(global_env, "loop", create_cfn("loop", L_loop));
	// ENV_ADD(global_env, "<", create_cfn("<", L_less_than));
	// ENV_ADD(global_env, "<=", create_cfn("<=", L_less_or_eq_than));
	// ENV_ADD(global_env, ">", create_cfn(">", L_more_than));
	// ENV_ADD(global_env, ">=", create_cfn(">=", L_more_or_eq_than));
	// ENV_ADD(global_env, "let", create_cfn("let", L_let));
	// ENV_ADD(global_env, "print", create_cfn("print", L_print));
	// ENV_ADD(global_env, "type", create_cfn("type", L_type));
	// ENV_ADD(global_env, "exit", create_cfn("exit", L_exit));


	// ENV_ADD(global_env, "help", create_cfn("help", L_help));


	// env_add(global_env, create_cfn("cdr", L_cdr));
	// env_add(global_env, create_cfn("car", L_car));
	// env_add(global_env, create_cfn("tést", L_test));

	// env_add(global_env, create_cfn("fn", L_create_fn));
	// env_add(global_env, create_cfn("obj-name", L_obj_name));
	// env_add(global_env, create_cfn("test", L_test));
	// env_add(global_env, create_cfn("input", L_get_input));
	// // env_add(global_env, create_cfn("=", L_eq));
	// // env_add(global_env, create_cfn("is?", L_is_same_obj));
// }


OBJ* __eval(OBJ* head)
{
	// printf("allocated: %d\n", gc.allocated);
	OBJ* o;
	switch (head->type)
	{

		case T_EXPR:
			return __eval(head->car);
		break;

		case T_LIST:
			return head;
		break;

		case T_NUM: case T_STR: case T_DECIMAL:
			return head;
		break;

		case T_C_FN:
			return head->c_fn(head);
		break;

		case T_FN:
			return run_func(head, head->cdr);
		break;

		case T_IDENTIFIER:
			if (head->name == NULL) return head;
			o = env_get(head->env, head->name);
			if (o == NULL) {printf("'%s' not found in '%s'\n", head->name, head->env->name); exit(-1); }

			switch (o->type)
			{
				case T_C_FN:
					return o->c_fn(NT(head));
				break;

				case T_FN:
					return run_func(o, NT(head));
				break;

				default:
					return o;
			}

			// print_obj_simple(o);
		break;

		case T_REF:
			// if (head->car == NULL) return head;
			o = head->car;
			if (o == NULL) {printf("'%s' not found in '%s'\n", head->name, head->env->name); exit(-1); }

			switch (o->type)
			{
				case T_C_FN:
					return o->c_fn(NT(head));
				break;

				case T_FN:
					return run_func(o, NT(head));
				break;

				default:
					return o;
			}
		break;

		default:
			LOBOTOMY_WARNING("[%s] not implemented yet", type_name(head->type));
	}

	return head;
}


void eval_program_file(const char* filename)
{
	const char* text = read_file(filename);
	eval_program(text);
}


void eval_program(const char text[])
{
	// OBJ_LIST* parsed = parse(text);
	// ITERATE_LINKED_LIST_VN(OBJ_LIST, parsed, OBJ, head)
	// {
		// print_obj_simple(__eval(head));
	// }

	LEXER lexer = tokenize(text);
	for (;;) {
		if (lexer.tokens[lexer.peek].text == NULL)
			break;
		// head = parse_expr(&lexer, 0).head;
		print_obj_simple(__eval(parse_expr(&lexer, 0).head));
		
		lexer.peek++;
	}
}