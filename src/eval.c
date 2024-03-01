#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "util.h"
#include "lexer.h"
#include "parse.h"
#include "l_std.h"
#include "obj.c"
#include "gc.h"

#include "eval.h"



OBJ* eval_into(OBJ* base, OBJ* res)
{
	ITERATE_OBJECT(base, curr)
	{
		OBJ* tmp = res->cdr;
		*res = *__eval(curr, curr->len);
		res = tmp;
	}
	return res;
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
					ret->cdr = empty_obj();
					*ret->cdr = *tmp;
					// ret->cdr = curr;
					// ret->cdr->type = T_REF;
					// ret->cdr->car = tmp;
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
	GCL_free(tmp);
	return head;
}

OBJ* preeval(OBJ* o)
{
	// OBJ* ooo = o;
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
				ret->cdr = __eval(curr->car, curr->len);
			break;

			case T_LIST:
				ret->cdr= curr;
				// ret->cdr = empty_obj();
				// *ret->cdr = *curr;
				// *ret->cdr = *curr->cdr;
				// ret->cdr->car = preeval(curr->car);
			break;

			case T_REF:
				ret->cdr = curr->car;
			break;


			default:
				ret->cdr = empty_obj();
				*ret->cdr = *curr;
				// if (ret->cdr != NULL || ret->cdr->type != T_NIL) {
					// tmp = ret->cdr->cdr;
				// }
				// ret->cdr = curr;
				// if (ret->cdr != NULL || ret->cdr->type != T_NIL) {
					// ret->cdr->cdr = tmp;
				// }
		}
		ret = ret->cdr;
	}

	tmp = head;
	head = NT(head);
	GCL_free(tmp);
	// printf("gc: %d\n", gcl->occupied);
	return head;
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
			env_add(e,  curr1);
		}
	)
	// print_obj_simple(fn->body);
	set_env(fn->body, e);

	// printf("\ngot here\n");
	// print_objf("::: ", fn->body->car->cdr);

	return L_progn(fn->body->len, fn->body);
	// if (fn->body->car->type == T_EXPR)
		// return L_progn(fn->body->len, fn->body->car);
	// else return __eval(fn->body, fn->body->len);
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


OBJ* __eval(OBJ* head, int argc)
{
	// printf("allocated: %d\n", gc.allocated);
	OBJ* o;
	switch (head->type)
	{

		case T_EXPR:
			return __eval(head->car, head->len);
		break;

		case T_LIST:
			return head;
		break;

		case T_MAP:
			return head;
		break;

		case T_NUM: case T_STR: case T_DECIMAL:
			return head;
		break;

		case T_C_FN:
			return head->c_fn(argc-1, head);
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
					return o->c_fn(argc-1, NT(head));
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
					return o->c_fn(argc-1, NT(head));
				break;

				case T_FN:
					return run_func(o, NT(head));
				break;

				default:
					return o;
			}
		break;

		default:
			LOBOTOMY_WARNING(
				"[%s] not implemented yet",
				type_name(head->type)
			);
	}

	return head;
}


void eval_program_file(const char* filename)
{
	const char* text = read_file(filename);
	eval_program(filename, text);
}


void eval_program(const char filename[], const char text[])
{
	// OBJ_LIST* parsed = parse(text);
	// ITERATE_LINKED_LIST_VN(OBJ_LIST, parsed, OBJ, head)
	// {
		// print_obj_simple(__eval(head));
	// }

	LEXER lexer = tokenize(filename, text);
	// printf("text: %s\n", text);
	for (;;) {
		// head = parse_expr(&lexer, 0).head;
		gcl->curr = gcl->top;
		OBJ* pair = parse_object(&lexer, 0, 0);
		print_obj_simple(__eval(pair, pair->len));
		
		// OBJ_PAIR pair = parse_expr(&lexer, 0, 0);
		// print_obj_simple(__eval(pair.head, pair.size));
		
		// printd("gc: %d\n", gcl->occupied);
		// lexer.peek++;
		// if (pair.head->type == T_NIL) return;
		if (lexer.tokens[lexer.peek].text == NULL || lexer.tokens[lexer.peek].type == TT_)
			break;
	}
}


