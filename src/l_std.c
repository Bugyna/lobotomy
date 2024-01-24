#include "obj.h"
#include "eval.h"


OBJ* L_less_than(OBJ_FN_ARGS)
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

OBJ* L_more_than(OBJ_FN_ARGS)
{
	OBJ* ret = empty_obj();
	ret->type = T_NUM;
	o = preeval(o);
	ret->num = (o->num > NT(o)->num);
	return ret;
}


OBJ* L_less_or_eq_than(OBJ_FN_ARGS)
{
	OBJ* ret = empty_obj();
	ret->type = T_NUM;
	o = preeval(o);
	ret->num = (o->num <= NT(o)->num);
	return ret;
}

OBJ* L_more_or_eq_than(OBJ_FN_ARGS)
{
	OBJ* ret = empty_obj();
	ret->type = T_NUM;
	o = preeval(o);
	ret->num = (o->num >= NT(o)->num);
	return ret;
}

OBJ* L_eq(OBJ_FN_ARGS)
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


OBJ* L_car(OBJ_FN_ARGS)
{
	o = preeval(o);
	print_obj_simple(o->car->cdr);
	return o->car;
	// return NIL;
}



OBJ* L_get_input(OBJ_FN_ARGS)
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


OBJ* L_cdr(OBJ_FN_ARGS)
{
	o = preeval(o);
	OBJ* tmp = empty_obj();
	tmp->type = T_LIST;
	tmp->car = o->cdr;
	return tmp;
	// print_objf("obj: ", o->cdr);
	return o->cdr;
}

OBJ* L_list(OBJ_FN_ARGS)
{
	OBJ* tmp = empty_obj();
	tmp->type = T_LIST;
	tmp->car = o;
	return tmp;
	// return o->cdr;
}

OBJ* L_progn(OBJ_FN_ARGS)
{
	/*!
		evaluate a list of expressions and return the last result
	*/
	// OBJ* o = o;
	OBJ* ret = empty_obj();
	OBJ* head = ret;

	OBJ* tmp;
	ITERATE_OBJECT(o, curr)
	{
		printd("progn: %s\n", type_name(curr->type));
		ret->cdr = __eval(curr, curr->len);
		ret = ret->cdr;
	}

	tmp = head;
	head = NT(head);
	GCL_free(tmp);
	return ret;
}

OBJ* L_nth(OBJ_FN_ARGS)
{
	o = preeval(o);
	OBJ* list = NT(o);
	OBJ* tmp = list->car;
	for (int i = 0; i < o->num; i++) {
		tmp = NT(tmp);
	}

	return L_copy(1, tmp);
}


OBJ* L_len(OBJ_FN_ARGS)
{
	o = preeval(o);
	OBJ* tmp = o;
	OBJ* ret = empty_obj_t(T_NUM);

	if (tmp->type == T_STR)
		ret->num = strlen(tmp->str);
	else if (tmp->type == T_LIST || tmp->type == T_EXPR) {
		ret->num = tmp->len;
	}
	else if (tmp->type == T_NIL)
		ret->num = 0;
	else
		ret->num = 1;

	return ret;
}


OBJ* L_insert(OBJ_FN_ARGS)
{
	o = preeval(o);
}

OBJ* L_append(OBJ_FN_ARGS)
{
	
}

OBJ* L_pop(OBJ_FN_ARGS)
{
	
}

OBJ* L_pop_at(OBJ_FN_ARGS)
{
	
}



OBJ* L_map(OBJ_FN_ARGS)
{
	
}


OBJ* L_map_get(OBJ_FN_ARGS)
{
	o = preeval(o);
	OBJ* ret = ENV_GET(o->map, NT(o)->str);
	if (o == NULL) return NIL;
	return ret;
}


OBJ* L_map_add(OBJ_FN_ARGS)
{
	OBJ* ret = empty_obj_t(T_REF);
	ret->name = NT(o)->name;
	ret->car = NT(NT(o));
	env_add(o->map, ret);
	return ret;
}


OBJ* L_exit(OBJ_FN_ARGS)
{
	if (o == NULL)
		exit(0);
	else
		exit(o->num);
}


OBJ* L_test(OBJ_FN_ARGS)
{
	// printf("pp: %p\n", o->cdr->cdr);
	o = preeval(o);
	return NIL;
}


OBJ* L_gc_top(OBJ_FN_ARGS)
{
	printf("GC: %d\n", gcl->top);
	return NIL;
}


OBJ* L_gc_size(OBJ_FN_ARGS)
{
	printf("GC: %d\n", gcl->top);
	return NIL;
}


OBJ* L_gc_print(OBJ_FN_ARGS)
{
	printf("GC: %d\n", gcl->top);
	return NIL;
}

OBJ* L_gc_collect(OBJ_FN_ARGS)
{
	GCL_collect();
	return NIL;
}


OBJ* L_print(OBJ_FN_ARGS)
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

OBJ* L_type(OBJ_FN_ARGS)
{
	preeval(o);
	printf("%s\n", type_name(o->type));
	return NIL;
}


OBJ* L_help(OBJ_FN_ARGS)
{
	for (int i = 0; i < o->env->size; i++) {
		if (o->env->list[i].key != NULL)
			printf("::> '%s'\n", o->env->list[i].key);
	}
	return NIL;
}


OBJ* L_create_fn(OBJ_FN_ARGS)
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

OBJ* L_obj_name(OBJ_FN_ARGS)
{
	OBJ* ret = NEW();
	ret->type = T_STR;
	ret->str = o->name;
	return ret;
}


OBJ* L_let(OBJ_FN_ARGS)
{
	ENV* e = o->env;
	OBJ* var = o;
	if (var->type != T_IDENTIFIER) {
		if (var->name != NULL && var->name != "") var->type = T_IDENTIFIER;
		else var = preeval(var);
	}
	OBJ* val = NT(var);
	val = preeval(val);
	// printd("let: %s %s\n", var->name, var->env->name);
	OBJ* ret = ENV_GET(var->env, var->name); // can't use env_get because we have to check only the scope we're currently in

	if (ret == NULL)
	{

		if (val->type == T_EXPR){printf("xxx\n"); ret = __eval(val->car, val->len);}
		else ret = val;
		ret->name = var->name;

		// printf("\npp: %s\n", ret->name);
		// printf("blabla: %s :", ret->name); print_obj_simple(ret);
		env_add(e, ret);
		// print_obj_simple(ENV_GET(&global_env, var->name));
		return ret;
	}

	if (val->type == T_EXPR)
		*ret = *__eval(val->car, var->len);
	else
		*ret = *val;

	ret->name = var->name;
	// printf("blabla: "); print_obj_simple(ret);


	return L_copy(1, ret);


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


OBJ* L_loop(OBJ_FN_ARGS)
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
		OBJ* tmp = __eval(cond_expr->car, cond_expr->len);
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
		ret = __eval(exec_expr->car, cond_expr->len);
		// __print_obj_full(__eval(exec_expr->car));
		// usleep(200);
		// sleep(1);
	}

	ret:
	return ret;
	// return NT(o);
}

OBJ* L_cond(OBJ_FN_ARGS)
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
		OBJ* tmp = __eval(cond_expr->car, cond_expr->len);
		// print_objf("tmp: ", tmp);


		if (tmp->num || tmp->type == T_TRUE) {
			// printd("aaa: %s\n", exec_expr->car->cdr->env->name);
			ret = __eval(exec_expr, exec_expr->len);
			goto ret;
		}
	}

	ret:
	return ret;
}

OBJ* L_copy(OBJ_FN_ARGS)
{
	OBJ* ret = empty_obj();
	*ret = *o;
	ret->cdr = NIL;
	return ret;
}

OBJ* L_get_random_num(OBJ_FN_ARGS)
{
	OBJ* ret = empty_obj_t(T_NUM);
	ret->num = time(NULL);
	// printf("t: %d", ret->num);
	if (o != NULL && o->type != T_NIL) {
		ret->cdr = o;
		ret = L_mod(argc, ret);
		// o = preeval(NT(o));
		// ret->num %= o->num;
	}
	return ret;
}

OBJ* L_use(OBJ_FN_ARGS)
{
	eval_program_file(o->str);
	return NIL;
}

OBJ* L_load(OBJ_FN_ARGS)
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