#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "obj.h"


OBJ* __eval(OBJ*, int argc);
OBJ* preeval(OBJ*);
OBJ* preeval_symbols(OBJ*);

static OBJ PI = {.marked=true, .type=T_DECIMAL, .name="PI", .decimal=3.141595265};
static OBJ ELSE_ALIAS = {.marked=true, .type=T_TRUE, .name="else", .num=1};

#define DEF_ARITHMETIC_OPERATION(NAME, SIGN)\
OBJ* L_##NAME(OBJ_FN_ARGS)\
{\
	OBJ* ret = empty_obj();\
	o = preeval(o);\
	OBJ* curr = o;\
	ret->type = o->type;\
	if (ret->type == T_NUM)\
		ret->num = o->num;\
	else if (ret->type == T_DECIMAL)\
		ret->decimal = o->decimal;\
\
	o = NT(curr);\
	if (o == NIL) {\
		LOBOTOMY_ERROR("Not enough arguments for arithmetic: '%s'", #SIGN);\
	}\
\
	ITERATE_OBJECT(o, tmp)\
	{\
		\
		if (tmp->type == T_IDENTIFIER)\
			curr = tmp->car;\
		else curr = tmp;\
		\
		if (ret->type == T_DECIMAL && curr->type == T_NUM)\
			ret->decimal SIGN##= curr->num;\
		\
		else if (ret->type == T_DECIMAL && curr->type == T_DECIMAL)\
			ret->decimal SIGN##= curr->decimal;\
		\
		else if (ret->type == T_NUM && curr->type == T_DECIMAL){\
			ret->type = T_DECIMAL; ret->decimal = ret->num;\
			ret->decimal SIGN##= curr->decimal;\
		}\
		else if (ret->type == T_NUM && curr->type == T_NUM)\
			ret->num SIGN##= curr->num;\
	} \
	return ret;\
}


#define DEF_BINARY_OPERATION(NAME, SIGN)\
OBJ* L_##NAME(OBJ_FN_ARGS)\
{\
	OBJ* ret = empty_obj();\
	o = preeval(o);\
	ret->type = o->type;\
	if (ret->type == T_NUM)\
		ret->num = o->num;\
\
	NEXT(o);\
	if (o == NULL) return ret; \
\
	ITERATE_OBJECT(o, curr)\
	{\
		\
		if (ret->type == T_NUM && curr->type == T_NUM)\
			ret->num SIGN##= curr->num;\
	}\
	return ret;\
}

#define DEF_UNARY_OPERATION(NAME, SIGN)\
OBJ* L_##NAME(OBJ_FN_ARGS)\
{\
	OBJ* ret = empty_obj();\
	o = preeval(o);\
	ret->type = o->type;\
	if (ret->type == T_NUM)\
		ret->num = o->num;\
\
	NEXT(o);\
	if (o == NULL) return ret; \
\
	ITERATE_OBJECT(o, curr)\
	{\
		\
		if (ret->type == T_NUM && curr->type == T_NUM)\
			ret->num = SIGN ret->num;\
	}\
	return ret;\
}


// Arithmetic operations
DEF_ARITHMETIC_OPERATION(add, +)
DEF_ARITHMETIC_OPERATION(subtract, -)
DEF_ARITHMETIC_OPERATION(multiply, *)
DEF_ARITHMETIC_OPERATION(divide, /)
OBJ* L_mod(OBJ_FN_ARGS)
{
	OBJ* ret = empty_obj();
	o = preeval(o);
	OBJ* curr = o;
	ret->type = o->type;
	if (ret->type == T_NUM)
		ret->num = o->num;
	else if (ret->type == T_DECIMAL)
		ret->decimal = o->decimal;

	o = NT(curr);
	if (o == NULL) {printf("\nhappend\n"); return ret;} 

	ITERATE_OBJECT(o, tmp)\
	{

		if (tmp->type == T_IDENTIFIER)
			curr = tmp->car;
		else curr = tmp;

		if (ret->type == T_DECIMAL && curr->type == T_NUM)\
			ret->decimal = fmod(ret->decimal, curr->num);

		else if (ret->type == T_DECIMAL && curr->type == T_DECIMAL)
			ret->decimal = fmod(ret->decimal, curr->decimal);

		else if (ret->type == T_NUM && curr->type == T_DECIMAL){
			ret->type = T_DECIMAL; ret->decimal = ret->num;
			ret->decimal = fmod(ret->num, curr->decimal);
		}
		else if (ret->type == T_NUM && curr->type == T_NUM)
			ret->num %= curr->num;
	}
	return ret;
}


// Binary operations
DEF_BINARY_OPERATION(and, &)
DEF_BINARY_OPERATION(or, |)
DEF_BINARY_OPERATION(xor, ^)
DEF_BINARY_OPERATION(bitshift_left, <<)
DEF_BINARY_OPERATION(bitshift_right, >>)


DEF_UNARY_OPERATION(neg, !)
DEF_UNARY_OPERATION(flip, ~)



// numeric comparisons
OBJ* L_less_than(OBJ_FN_ARGS);
OBJ* L_more_than(OBJ_FN_ARGS);
OBJ* L_less_or_eq_than(OBJ_FN_ARGS);
OBJ* L_more_or_eq_than(OBJ_FN_ARGS);
OBJ* L_eq(OBJ_FN_ARGS);


// Math
OBJ* L_pow(OBJ_FN_ARGS);
OBJ* L_sqrt(OBJ_FN_ARGS);
OBJ* L_cbrt(OBJ_FN_ARGS);
OBJ* L_nth_root(OBJ_FN_ARGS);
OBJ* L_log(OBJ_FN_ARGS);
OBJ* L_log10(OBJ_FN_ARGS);
OBJ* L_ceil(OBJ_FN_ARGS);
OBJ* L_floor(OBJ_FN_ARGS);
OBJ* L_round(OBJ_FN_ARGS);
OBJ* L_max(OBJ_FN_ARGS);
OBJ* L_min(OBJ_FN_ARGS);

OBJ* L_cos(OBJ_FN_ARGS);
OBJ* L_sin(OBJ_FN_ARGS);
OBJ* L_tan(OBJ_FN_ARGS);
OBJ* L_acos(OBJ_FN_ARGS);
OBJ* L_asin(OBJ_FN_ARGS);
OBJ* L_atan(OBJ_FN_ARGS);
OBJ* L_atan2(OBJ_FN_ARGS);



// lisp functions
OBJ* L_car(OBJ_FN_ARGS);
OBJ* L_cdr(OBJ_FN_ARGS);
OBJ* L_list(OBJ_FN_ARGS);
OBJ* L_progn(OBJ_FN_ARGS);

// List/vector operatins
OBJ* L_first(OBJ_FN_ARGS);
OBJ* L_last(OBJ_FN_ARGS);
OBJ* L_nth(OBJ_FN_ARGS);
OBJ* L_len(OBJ_FN_ARGS);
OBJ* L_insert(OBJ_FN_ARGS);
OBJ* L_append(OBJ_FN_ARGS);
OBJ* L_pop(OBJ_FN_ARGS);
OBJ* L_pop_at(OBJ_FN_ARGS);

// Map functions
OBJ* L_map(OBJ_FN_ARGS);
OBJ* L_map_get(OBJ_FN_ARGS);
OBJ* L_map_add(OBJ_FN_ARGS);


// helper functions
OBJ* L_copy(OBJ_FN_ARGS);
OBJ* L_type(OBJ_FN_ARGS);
OBJ* L_help(OBJ_FN_ARGS);
OBJ* L_exit(OBJ_FN_ARGS);
OBJ* L_obj_name(OBJ_FN_ARGS);


// meta functions
OBJ* L_expand(OBJ_FN_ARGS);

// GC helpers
OBJ* L_gc_top(OBJ_FN_ARGS);
OBJ* L_gc_size(OBJ_FN_ARGS);
OBJ* L_gc_print(OBJ_FN_ARGS);
OBJ* L_gc_collect(OBJ_FN_ARGS);

// variables and functions
OBJ* L_let(OBJ_FN_ARGS);
OBJ* L_create_fn(OBJ_FN_ARGS);

// flow control
OBJ* L_if(OBJ_FN_ARGS);
OBJ* L_cond(OBJ_FN_ARGS);
OBJ* L_loop(OBJ_FN_ARGS);

// IO
OBJ* L_get_input(OBJ_FN_ARGS);
OBJ* L_print(OBJ_FN_ARGS);
OBJ* L_fprint(OBJ_FN_ARGS);
OBJ* L_file_open(OBJ_FN_ARGS);
OBJ* L_file_write(OBJ_FN_ARGS);
OBJ* L_file_close(OBJ_FN_ARGS);
OBJ* L_file_read(OBJ_FN_ARGS);


// Random
OBJ* L_get_random_num(OBJ_FN_ARGS);

// other
OBJ* L_test(OBJ_FN_ARGS);

OBJ* L_load(OBJ_FN_ARGS);
OBJ* L_use(OBJ_FN_ARGS);

void lobotomy_init(ENV* env)
{
	// ENV_ADD(global_env, "+", create_cfn("+", lobotomy_add));
	ENV_ADD(env, "+", create_cfn("+", L_add));
	ENV_ADD(env, "-", create_cfn("-", L_subtract));
	ENV_ADD(env, "*", create_cfn("*", L_multiply));
	ENV_ADD(env, "/", create_cfn("/", L_divide));

	ENV_ADD(env, "&", create_cfn("&", L_and));
	ENV_ADD(env, "|", create_cfn("|", L_or));
	ENV_ADD(env, "xor", create_cfn("xor", L_xor));
	ENV_ADD(env, "%", create_cfn("%", L_mod));
	ENV_ADD(env, "loop", create_cfn("loop", L_loop));
	env_add(env, create_cfn("?", L_cond));
	
	ENV_ADD(env, "<", create_cfn("<", L_less_than));
	ENV_ADD(env, "<=", create_cfn("<=", L_less_or_eq_than));
	ENV_ADD(env, ">", create_cfn(">", L_more_than));
	ENV_ADD(env, ">=", create_cfn(">=", L_more_or_eq_than));
	env_add(env, create_cfn("=", L_eq));
	ENV_ADD(env, "let", create_cfn("let", L_let));
	ENV_ADD(env, "print", create_cfn("print", L_print));
	ENV_ADD(env, "type", create_cfn("type", L_type));
	ENV_ADD(env, "exit", create_cfn("exit", L_exit));

	env_add(env, create_cfn("gc-top", L_gc_top));
	env_add(env, create_cfn("gc-collect", L_gc_collect));


	ENV_ADD(env, "help", create_cfn("help", L_help));
	
	
	env_add(env, create_cfn("expand", L_expand));


	env_add(env, create_cfn("map-get", L_map_get));
	env_add(env, create_cfn("map-add", L_map_add));


	env_add(env, create_cfn("cdr", L_cdr));
	env_add(env, create_cfn("car", L_car));
	env_add(env, create_cfn("list", L_list));
	env_add(env, create_cfn("progn", L_progn));
	
	
	env_add(env, create_cfn("nth", L_nth));
	env_add(env, create_cfn("len", L_len));
	
	env_add(env, create_cfn("tést", L_test));

	env_add(env, create_cfn("fn", L_create_fn));
	env_add(env, create_cfn("use", L_use));
	env_add(env, create_cfn("load", L_load));
	env_add(env, create_cfn("obj-name", L_obj_name));
	env_add(env, create_cfn("test", L_test));
	
	
	env_add(env, create_cfn("input", L_get_input));


	env_add(env, create_cfn("fprint", L_fprint));
	
	env_add(env, create_cfn("file-open", L_file_open));
	env_add(env, create_cfn("file-write", L_file_write));
	env_add(env, create_cfn("file-close", L_file_close));
	env_add(env, create_cfn("file-read", L_file_read));
	

	env_add(env, create_cfn("random-num", L_get_random_num));
	
	
	env_add(env, &PI);
	env_add(env, &ELSE_ALIAS);

	
	// env_add(env, create_cfn("=", L_eq));
	// env_add(env, create_cfn("is?", L_is_same_obj));
}

