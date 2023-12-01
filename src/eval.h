#include "parse.h"
#include "obj.h"
#include "linked_list.h"
#include <math.h>


#define NT(X) X->cdr
#define NEXT(X) X = X->cdr

OBJ* __eval(OBJ*);
void preeval(OBJ*, ...);


static OBJ* NIL = &((OBJ){.type=T_NIL});


#define DEF_ARITHMETIC_OPERATION(NAME, SIGN)\
OBJ* L_##NAME(OBJ* o, ...)\
{\
	o = NT(o);\
	OBJ* ret = NEW();\
	preeval(o);\
	OBJ* curr = o;\
	if (o->type == T_IDENTIFIER)\
		o = o->car;\
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
OBJ* L_##NAME(OBJ* o, ...)\
{\
	o = NT(o);\
	OBJ* ret = NEW();\
	preeval(o);\
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




OBJ* create_cfn(const char* name, C_FUNC_DEC fn)
{
	OBJ* o = empty_obj();
	o->type = T_C_FN;
	o->name = name;
	o->c_fn = fn;
	return o;
}

OBJ* lobotomy_add(OBJ* o, ...)
{
	OBJ* ret = NEW();
	o = NT(o);
	preeval(o);
	printf("addd: "); print_obj_simple(o);
	
	// printf("x: %ld", o->num + NT(o)->num);
	ret->type = T_NUM;
	ret->num = o->num;
	// print_obj_simple(NT(NT(o)));

	ITERATE_OBJECT_PTR(NT(o), curr)
	{
		if ((*curr)->type == T_IDENTIFIER)
			*curr = (*curr)->car;
		if ((*curr)->type == T_NUM)
			ret->num += (*curr)->num;

		else if ((*curr)->type == T_LIST)
		{
			// __print_obj_full(*curr);
			OBJ *tmp = __eval(*curr);
			// print_obj_simple(tmp);
			// printf("happening %ld\n", tmp->num);
			ret->num += tmp->num;
		}
	}
	
	return ret;
}

DEF_ARITHMETIC_OPERATION(add, +)
DEF_ARITHMETIC_OPERATION(subtract, -)
DEF_ARITHMETIC_OPERATION(multiply, *)
DEF_ARITHMETIC_OPERATION(divide, /)

DEF_BINARY_OPERATION(and, &)
DEF_BINARY_OPERATION(or, |)
DEF_BINARY_OPERATION(xor, ^)
// DEF_ARITHMETIC_OPERATION(modulo, %)

OBJ* L_less_than(OBJ* o, ...)
{
	OBJ* ret = NEW();
	ret->type = T_NUM;
	o = NT(o);
	preeval(o);

	if (o->type == T_IDENTIFIER)
		ret->num = (o->car->num < NT(o)->num);
	else
		ret->num = (o->num < NT(o)->num);

	return ret;
}

OBJ* L_more_than(OBJ* o, ...)
{
	OBJ* ret = NEW();
	ret->type = T_NUM;
	o = NT(o);
	preeval(o);
	ret->num = (o->num > NT(o)->num);
	return ret;
}


OBJ* L_less_or_eq_than(OBJ* o, ...)
{
	OBJ* ret = NEW();
	ret->type = T_NUM;
	o = NT(o);
	preeval(o);
	ret->num = (o->num <= NT(o)->num);
	return ret;
}


OBJ* L_car(OBJ* o)
{
	return o->car;
}


OBJ* L_cdr(OBJ* o)
{
	OBJ* tmp = NEW();
	tmp->type = T_LIST;
	tmp->car = o->cdr;
	return tmp;
	// return o->cdr;
}

OBJ* L_more_or_eq_than(OBJ* o, ...)
{
	OBJ* ret = NEW();
	ret->type = T_NUM;
	o = NT(o);
	preeval(o);
	ret->num = (o->num >= NT(o)->num);
	return ret;
}


OBJ* L_exit(OBJ* o)
{
	exit(0);
}

void preeval(OBJ* o, ...)
{
	ITERATE_OBJECT_PTR(o, curr)
	{
		if ((*curr)->type == T_IDENTIFIER && (*curr)->car == NULL)
		{
			printf("getting: %s\n", (*curr)->name);
			// TODO keep linked list order
			// OBJ* tmp = (*curr)->cdr;
			(*curr)->car = ENV_GET(&global_env, (*curr)->name);
			// *curr = ENV_GET(&global_env, (*curr)->name);
			// (*curr)->cdr = tmp;
			printf("GOT: "); print_obj_simple(*curr);
		}
		else if ((*curr)->type == T_EXPR)
			preeval((*curr)->car);
	}
	// print_obj_simple(o);
}


OBJ* L_loop(OBJ* o, ...)
{
	// TODO: exec_expr get by second list obvi
	// that means remake structure so it adheres to lisp properly


	OBJ* cond_expr = NT(o);
	// ITERTE_OBJECT(cond_expr, curr)
	// {
		
	// }
	OBJ* exec_expr = NT(cond_expr);
	// print_obj_simple(cond_expr);

	// preeval(cond_expr);
	// preeval(exec_expr);
	OBJ* ret = empty_obj();

	while (1)
	{
		OBJ* tmp = __eval(cond_expr->car);
		printf("tmp: "); print_obj_simple(tmp);
		if (!tmp->num)
		{
			// printf("aaaaa: "); print_obj_simple(tmp);
			break;
		}
		// (loop (a) (let a (+ a 2)))
			
		// printf("aaaaa");
		ret = __eval(exec_expr->car);
		// __print_obj_full(__eval(exec_expr->car));
	}

	return ret;
	// return NT(o);
	
}

OBJ* L_copy(OBJ* o, ...)
{
	OBJ* ret = NEW();
	*ret = *o;
	return ret;
}



OBJ* L_let(OBJ* o, ...)
{
	OBJ* var = NT(o);
	preeval(var);
	OBJ* val = NT(var);
	// OBJ* ret = ENV_GET(&global_env, var->name);

	// if (ret == NULL)
	// {

		// if (val->type == T_EXPR){printf("xxx\n"); *ret = *__eval(val->car);}
		// else ret = L_copy(val);
		// ret->name = var->name;

		// // printf("\npp: %s\n", ret->name);
		// printf("blabla: "); print_obj_simple(ret);
		// ENV_ADD(&global_env, ret->name, ret);
		// return ret;
	// }

	// if (val->type == T_EXPR){printf("xxx\n"); *ret = *__eval(val->car);}
	// else
	// {
		// *ret = *val;
	// }

	
	// ret->name = var->name;
	// printf("blabla: "); print_obj_simple(ret);


	// return ret;


	if (var->car == NULL)
	{
		printf("\neee\n");
		if (val->type == T_EXPR)
		{
			printf("xxx\n");
			OBJ* tmp = __eval(val->car);
			var->car = malloc(tmp);
			*var->car = *tmp;
		}
		else
		{
			var->car = malloc(sizeof(val));
			*var->car = *L_copy(val);
		}
		// var->car->name = var->name;

		// printf("\npp: %s\n", ret->name);
		printf("blabla: "); print_obj_simple(var->car);
		var->car = ENV_ADD(&global_env, var->name, var->car);
		
		return var;
	}

	if (val->type == T_EXPR){printf("xxx\n"); *var->car = *__eval(val->car);}
	else
	{
		*var->car = *val;
	}
	
	return var;
}

OBJ* run_func(OBJ* fn, OBJ* args)
{
	return NEW();
}


OBJ* L_print(OBJ* o, ...)
{
	preeval(o);
	// print_obj_simple(o);
	// printf("type: %s\n", type_name(o->cdr->type));
	print_obj_simple(NT(o));
	return NIL;
}

OBJ* L_type(OBJ* o, ...)
{
	preeval(o);
	o = NT(o);
	printf("%s\n", type_name(o->type));
	return NIL;
}

void lobotomy_init()
{
	// ENV_ADD(&global_env, "+", create_cfn("+", lobotomy_add));
	ENV_ADD(&global_env, "+", create_cfn("+", L_add));
	ENV_ADD(&global_env, "-", create_cfn("-", L_subtract));
	ENV_ADD(&global_env, "*", create_cfn("*", L_multiply));
	ENV_ADD(&global_env, "/", create_cfn("/", L_divide));

	ENV_ADD(&global_env, "&", create_cfn("&", L_and));
	ENV_ADD(&global_env, "|", create_cfn("|", L_or));
	ENV_ADD(&global_env, "xor", create_cfn("xor", L_xor));
	// ENV_ADD(&global_env, "%", create_cfn("%", L_modulo));
	ENV_ADD(&global_env, "loop", create_cfn("loop", L_loop));
	ENV_ADD(&global_env, "<", create_cfn("<", L_less_than));
	ENV_ADD(&global_env, "<=", create_cfn("<=", L_less_or_eq_than));
	ENV_ADD(&global_env, ">", create_cfn(">", L_more_than));
	ENV_ADD(&global_env, ">=", create_cfn(">=", L_more_or_eq_than));
	ENV_ADD(&global_env, "let", create_cfn("let", L_let));
	ENV_ADD(&global_env, "print", create_cfn("print", L_print));
	ENV_ADD(&global_env, "type", create_cfn("type", L_type));
	ENV_ADD(&global_env, "exit", create_cfn("exit", L_exit));


	env_add(&global_env, create_cfn("cdr", L_cdr));
	env_add(&global_env, create_cfn("car", L_car));
}


OBJ* __eval(OBJ* head)
{
	OBJ* o;
	switch (head->type)
	{

		// case T_EXPR:
			// return __eval(head);
		// break;

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
			o = ENV_GET(&global_env, head->name);
			if (o == NULL) {printf("%s not found\n", head->name); exit(-1); }

			switch (o->type)
			{
				case T_C_FN:
					return o->c_fn(head);
				break;

				case T_FN:
					return run_func(o, head->cdr);
				break;

				default:
					return o;
			}

			// print_obj_simple(o);
		break;

		default:
			LOBOTOMY_WARNING("[%s] not implemented yet", type_name(head->type));
	}

	return head;
}



void eval_program(const char text[])
{
	OBJ_LIST* parsed = parse(text);
	// print_obj_simple(parsed->first->val);

	// exit(-1);
	ITERATE_LINKED_LIST_VN(OBJ_LIST, parsed, OBJ, head)
	{
		print_obj_simple(__eval(head));
	}
}
