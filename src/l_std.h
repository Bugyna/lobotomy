#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "obj.h"


OBJ* __eval(OBJ*);
OBJ* preeval(OBJ*);
OBJ* preeval_symbols(OBJ*);

#define DEF_ARITHMETIC_OPERATION(NAME, SIGN)\
OBJ* L_##NAME(OBJ* o)\
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
	if (o == NULL) {printf("\nhappend\n"); return ret;} \
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
OBJ* L_##NAME(OBJ* o)\
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


// Arithmetic operations
DEF_ARITHMETIC_OPERATION(add, +)
DEF_ARITHMETIC_OPERATION(subtract, -)
DEF_ARITHMETIC_OPERATION(multiply, *)
DEF_ARITHMETIC_OPERATION(divide, /)
OBJ* L_mod(OBJ* o)
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
DEF_BINARY_OPERATION(neg, !)


// numeric comparisons
OBJ* L_less_than(OBJ* o);
OBJ* L_more_than(OBJ* o);
OBJ* L_less_or_eq_than(OBJ* o);
OBJ* L_more_or_eq_than(OBJ* o);
OBJ* L_eq(OBJ* o);


// Math
OBJ* L_pow(OBJ* o);
OBJ* L_sqrt(OBJ* o);
OBJ* L_cbrt(OBJ* o);
OBJ* L_nth_root(OBJ* o);
OBJ* L_log(OBJ* o);
OBJ* L_log10(OBJ* o);
OBJ* L_ceil(OBJ* o);
OBJ* L_floor(OBJ* o);
OBJ* L_round(OBJ* o);
OBJ* L_max(OBJ* o);
OBJ* L_min(OBJ* o);

OBJ* L_cos(OBJ* o);
OBJ* L_sin(OBJ* o);
OBJ* L_tan(OBJ* o);
OBJ* L_acos(OBJ* o);
OBJ* L_asin(OBJ* o);
OBJ* L_atan(OBJ* o);
OBJ* L_atan2(OBJ* o);



// lisp functions
OBJ* L_progn(OBJ* o);
OBJ* L_car(OBJ* o);
OBJ* L_cdr(OBJ* o);
OBJ* L_list(OBJ* o);
OBJ* L_nth(OBJ* o);

// Map functions
OBJ* L_map(OBJ* o);
OBJ* L_map_get(OBJ* o);
OBJ* L_map_add(OBJ* o);


// helper functions
OBJ* L_copy(OBJ* o);
OBJ* L_type(OBJ* o);
OBJ* L_help(OBJ* o);
OBJ* L_exit(OBJ* o);
OBJ* L_obj_name(OBJ* o);


// variables and functions
OBJ* L_let(OBJ* o);
OBJ* L_create_fn(OBJ* o);

// flow control
OBJ* L_cond(OBJ* o);
OBJ* L_loop(OBJ* o);

// IO
OBJ* L_get_input(OBJ* o);
OBJ* L_print(OBJ* o);


// Random
OBJ* L_get_random_num(OBJ* o);

// other
OBJ* L_test(OBJ* o);

OBJ* L_load(OBJ* o);
OBJ* L_use(OBJ* o);

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


	ENV_ADD(env, "help", create_cfn("help", L_help));


	env_add(env, create_cfn("map-get", L_map_get));
	env_add(env, create_cfn("map-add", L_map_add));


	env_add(env, create_cfn("cdr", L_cdr));
	env_add(env, create_cfn("car", L_car));
	env_add(env, create_cfn("list", L_list));
	env_add(env, create_cfn("nth", L_nth));
	env_add(env, create_cfn("tést", L_test));

	env_add(env, create_cfn("fn", L_create_fn));
	env_add(env, create_cfn("use", L_use));
	env_add(env, create_cfn("load", L_load));
	env_add(env, create_cfn("obj-name", L_obj_name));
	env_add(env, create_cfn("test", L_test));
	env_add(env, create_cfn("input", L_get_input));

	env_add(env, create_cfn("random-num", L_get_random_num));
	
	
	static const OBJ PI = (OBJ){.type=T_DECIMAL, .name="PI", .decimal=3.141595265};
	env_add(env, &PI);
	
	static const OBJ ELSE_ALIAS = (OBJ){.type=T_TRUE, .name="else", .num=1};
	env_add(env, &ELSE_ALIAS);

	
	// env_add(env, create_cfn("=", L_eq));
	// env_add(env, create_cfn("is?", L_is_same_obj));
}

