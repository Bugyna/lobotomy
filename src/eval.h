#include "parse.h"
#include "obj.h"
#include "linked_list.h"






#define NT(X) X->next
#define NEXT(X) X = X->next

OBJ* __eval(OBJ*);
void preeval(OBJ*, ...);

#define DEF_ARITHMETIC_OPERATION(NAME, SIGN)\
OBJ* L_##NAME(OBJ* o, ...)\
{\
	o = NT(o);\
	OBJ* ret = NEW();\
	ret->type = o->type;\
	if (ret->type == T_NUM)\
		ret->num = o->num;\
	else if (ret->type == T_DECIMAL)\
		ret->num = o->decimal;\
\
	NEXT(o);\
	if (o == NULL) ret->num \
\
	ITERATE_OBJECT(o, curr)\
	{\
		if (curr->type == T_DECIMAL) ret->type = T_DECIMAL;\
		\
		if (ret->type == T_DECIMAL && curr->type == T_NUM)\
			ret->decimal SIGN##= curr->num;\
		\
		else if (ret->type == T_DECIMAL && curr->type == T_DECIMAL)\
			ret->num SIGN##= curr->decimal;\
\
		else if (ret->type == T_NUM && curr->type == T_DECIMAL)\
			ret->num SIGN##= curr->decimal;\
		\
		else if (ret->type == T_NUM && curr->type == T_NUM)\
			ret->num SIGN##= curr->num;\
	} \
	printf("RET: %ld\n", ret->num);\
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
		if ((*curr)->type == T_NUM)
			ret->num += (*curr)->num;

		else if ((*curr)->type == T_LIST)
		{
			// __print_obj_full(*curr);
			OBJ *tmp = __eval(((*curr)));
			// print_obj_simple(tmp);
			// printf("happening %ld\n", tmp->num);
			ret->num += tmp->num;
		}
	}
	
	return ret;
}

DEF_ARITHMETIC_OPERATION(subtract, -)

OBJ* L_less_than(OBJ* o, ...)
{
	OBJ* ret = NEW();
	ret->type = T_NUM;
	o = NT(o);
	ret->num = (o->num < NT(o)->num);
	return ret;
}

OBJ* L_more_than(OBJ* o, ...)
{
	OBJ* ret = NEW();
	ret->type = T_NUM;
	o = NT(o);
	ret->num = (o->num > NT(o)->num);
	return ret;
}


OBJ* L_less_or_eq_than(OBJ* o, ...)
{
	OBJ* ret = NEW();
	ret->type = T_NUM;
	o = NT(o);
	ret->num = (o->num <= NT(o)->num);
	return ret;
}


OBJ* L_more_or_eq_than(OBJ* o, ...)
{
	OBJ* ret = NEW();
	ret->type = T_NUM;
	o = NT(o);
	ret->num = (o->num >= NT(o)->num);
	return ret;
}


void preeval(OBJ* o, ...)
{
	ITERATE_OBJECT_PTR(o, curr)
	{
		if ((*curr)->type == T_IDENTIFIER)
		{
			printf("getting: %s\n", (*curr)->name);
			// TODO keep linked list order
			**curr = *ENV_GET(&global_env, (*curr)->name);
			printf("GOT: "); print_obj_simple(*curr);
		}
		// else if ((*curr)->type == T_LIST)
			// preeval(NT((*curr)));
	}
}


OBJ* L_loop(OBJ* o, ...)
{
	OBJ* cond_expr = NT(o);
	OBJ* exec_expr = NT(cond_expr);

	preeval(cond_expr);
	preeval(exec_expr);
	OBJ* ret = empty_obj();

	while (__eval(cond_expr)->num)
	{
		printf("aaaaa");
		// // ret = __eval(exec_expr);
	}

	return NT(o);
	
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
	OBJ* val = NT(var);
	OBJ* ret = ENV_GET(&global_env, var->name);

	if (ret == NULL)
	{

		if (val->type == T_LIST){printf("xxx\n"); ret = __eval(NT(val));}
		else ret = L_copy(val);
		ret->name = var->name;

		// printf("\npp: %s\n", ret->name);
		ENV_ADD(&global_env, ret->name, ret);
		return ret;
	}

	if (val->type == T_LIST){printf("xxx\n"); ret = __eval(NT(val));}
	else *ret = *val;
	ret->name = var->name;
	return ret;
}

OBJ* run_func(OBJ* fn, OBJ* args)
{
	return NEW();
}


OBJ* L_print(OBJ* o, ...)
{
	preeval(o);
	print_obj_simple(NT(o));
	return NT(o);
}

void lobotomy_init()
{
	ENV_ADD(&global_env, "+", create_cfn("+", lobotomy_add));
	ENV_ADD(&global_env, "-", create_cfn("-", L_subtract));
	ENV_ADD(&global_env, "loop", create_cfn("loop", L_loop));
	ENV_ADD(&global_env, "<", create_cfn("<", L_less_than));
	ENV_ADD(&global_env, "let", create_cfn("let", L_let));
	ENV_ADD(&global_env, "print", create_cfn("print", L_print));
}


OBJ* __eval(OBJ* head)
{
	OBJ* o;
	switch (head->type)
	{
		case T_LIST: case T_NUM: case T_STR: case T_DECIMAL:
			return head;
		break;

		case T_C_FN:
			return head->c_fn(head);
		break;

		case T_FN:
			return run_func(head, head->next);
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
					return run_func(o, head->next);
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
