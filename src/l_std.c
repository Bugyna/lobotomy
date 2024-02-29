#include "obj.c"
#include "eval.h"
#include "io.c"
#include "l_std.h"
#include "gc.h"

#include <dlfcn.h>

OBJ* L_less_than(OBJ_FN_ARGS)
{
	L_CHECK_ARITY(argc, 2);
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
	L_CHECK_ARITY(argc, 2);

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
	if (o->type == T_NUM || o->type == T_DECIMAL)
	{
		if (NT(o)->type == T_NUM || NT(o)->type == T_DECIMAL) {
			ret->num = (o->num == NT(o)->num);
		}
		else {
			lobotomy_error("Invalid types for comparison: trying to compare '%s' and '%s'\n", type_name(o->type), type_name(NT(o)->type));
		}
	}

	else if (o->type == T_STR)
	{
		if (NT(o)->type == T_STR) {
			ret->num = !strcmp(o->str, NT(o)->str);
		}
		else {
			lobotomy_error("Invalid types for comparison: trying to compare '%s' and '%s'\n", type_name(o->type), type_name(NT(o)->type));
		}
	}
	
	return ret;
}

OBJ* L_not_eq(OBJ_FN_ARGS)
{
	OBJ* ret = L_eq(argc, o);
	ret->num = !(bool)ret->num;
}


OBJ* L_car(OBJ_FN_ARGS)
{
	o = preeval(o);
	// print_obj_simple(o->car->cdr);
	return o->car;
	// return NIL;
}

OBJ* L_cdr(OBJ_FN_ARGS)
{
	o = preeval(o);
	OBJ* tmp = empty_obj();
	tmp->type = T_LIST;
	tmp->car = o->car->cdr;
	return tmp;
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
	OBJ* ret = NIL;
	OBJ* head = ret;

	// if (o->type == T_LIST || o->type == T_EXPR) o = o->car;
	ITERATE_OBJECT(o, curr)
	{
		// printd("progn: %s\n", type_name(curr->type));
		ret->cdr = __eval(curr, curr->len);
		ret = ret->cdr;
	}

	head = NT(head);
	return ret;
}

OBJ* L_nth(OBJ_FN_ARGS)
{
	o = preeval(o);
	OBJ* list = NT(o);
	OBJ* ret = NIL;

	if (list->type == T_LIST || list->type == T_EXPR)
	{
		ret = list->car;
		for (int i = 0; i < o->num; i++) {
			ret = NT(ret);
		}
		ret = L_copy(1, ret);
	}

	else {
		ret = empty_obj();
		ret->type = T_STR;
		// char x[] = {list->str[o->num], '\0'};
		ret->str = malloc(2);
		ret->str[0] = list->str[o->num];
		ret->str[1] = '\0';
	}
	
	return ret;
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
	return o;
}

OBJ* L_append(OBJ_FN_ARGS)
{

	return NIL;
}

OBJ* L_pop(OBJ_FN_ARGS)
{
	
	return o;
}

OBJ* L_pop_at(OBJ_FN_ARGS)
{

	return NIL;
}



OBJ* L_map(OBJ_FN_ARGS)
{

	return NIL;
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

	return NIL;
}


OBJ* L_test(OBJ_FN_ARGS)
{
	// printf("pp: %p\n", o->cdr->cdr);
	o = preeval(o);
	return NIL;
}


OBJ* L_gc_top(OBJ_FN_ARGS)
{
	// printf("GC: %d\n", gcl->occupied);
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
	gcl->curr = gcl->top;
	GCL_collect(gcl);
	return NIL;
}


OBJ* L_print(OBJ_FN_ARGS)
{
	// o = NT(o);
	// o = preeval(o);
	// printf("type: %s\n", type_name(o->cdr->type));
	OBJ* tmp = preeval(o);
	// printf("print>>");
	__print_obj_full(tmp);
	// print_objf("aa: ", tmp);

	// ITERATE_OBJECT(tmp, curr)
	// {
		// __print_obj_simple(__eval(curr, curr->len));
	// }
	// printf("\n");
	// print_objf("print>>", NT(tmp));
	return NIL;
}


OBJ* L_fprint(OBJ_FN_ARGS)
{
	/*! */
}


OBJ* L_file_open(OBJ_FN_ARGS)
{
	o = preeval(o);
	L_CHECK_ARITY(2, argc);
	OBJ* ret = empty_obj_t(T_FILE);
	OBJ* file_name = o;
	OBJ* mode = NT(file_name);
	FILE* f = fopen(file_name->str, mode->str);
	printf("opened file: %s in %s\n", file_name->str, mode->str);
	if (f == NULL) {
		LOBOTOMY_ERROR_S(ERR_RUNTIME, "Couldn't open file '%s' with mode '%s'", file_name->str, mode->str);
	}
	ret->file = f;
	return ret;
}
	

OBJ* L_file_write(OBJ_FN_ARGS)
{
	return o;
}

OBJ* L_file_close(OBJ_FN_ARGS)
{
	o = preeval(o);
	L_CHECK_ARITY(1, argc);
	fclose(o->file);
	return NIL;
}

OBJ* L_file_read(OBJ_FN_ARGS)
{
	o = preeval(o);
	OBJ* ret = empty_obj_t(T_STR);
	OBJ* file = o;
	OBJ* n = NT(file);
	char* str = malloc(n->num+1);
	fread(str, sizeof(char), n->num, file->file);
	return NIL;
}



OBJ* L_expand(OBJ_FN_ARGS)
{
	OBJ* tmp = preeval(o);
	__print_obj_expand(tmp);
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
	// ENV* env = gcl->env;
	ENV* env = global_env;
	if (o != NULL && o->type != T_NIL) env = o->env;
	for (size_t i = 0; i < env->size; i++) {
		if (env->list[i].key == NULL) continue;
		__ITERATE_HASHMAP(ENV, env, OBJ, env->list[i].key)
		{
			printf("::> '%s'\n", BUCKET->key);
		}
			// printf("::> '%s'\n", o->env->list[i].key);
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
	fn->args = args;
	fn->body = body;
	env_add(global_env, fn);
	// env_add(gcl->env, fn);
	return fn;
}

OBJ* L_obj_name(OBJ_FN_ARGS)
{
	OBJ* ret = empty_obj();
	ret->type = T_STR;

	ret->str = o->name;
	return ret;
}


OBJ* L_let(OBJ_FN_ARGS)
{
	ENV* e = o->env;
	OBJ* var = o;
	if (var->type != T_IDENTIFIER) {
		if (var->name != NULL) var->type = T_IDENTIFIER;
		else var = __eval(var, var->len);
	}
	OBJ* val = NT(var);
	val = __eval(val, val->len);
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


	return ret;


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
	// OBJ* o_exec_expr = L_copy(exec_expr->len, exec_expr);
	
	// OBJ* o_exec_expr = empty_obj_t(T_EXPR);
	// o_exec_expr->len = exec_expr->len;
	// o_exec_expr->car = copy_literals(exec_expr->car);
	OBJ* o_exec_expr = exec_expr;
	// OBJ* new_exec_expr = preeval_symbols(exec_expr);
	// exec_expr->car = preeval_symbols(exec_expr->car);
	// print_obj_simple(exec_expr);

	// print_objf("exec_expr: ", exec_expr);

	// preeval(cond_expr);
	// preeval(exec_expr);
	OBJ* ret = NIL;

	// printd("loop starting now\n");
	int i = 0;
	while (1)
	{
		OBJ* tmp = __eval(cond_expr, cond_expr->len);
		// printf("tmp: "); print_obj_simple(tmp);
		// print_objf("cond_expr: ", cond_expr);
		// print_objf("exec_expr: ", o_exec_expr);

		// (loop (< a 10) (let a (+ a 2)))
		if (!tmp->num)
		{
			// printf("aaaaa: "); print_obj_simple(tmp);
			break;
			goto ret;
		}
		// (loop (a) (let a (+ a 2)))
			
		// printf("aaaaa");
		// print_objf("ret: ", ret);
		// eval_into(exec_expr, new_exec_expr);
		// ret = L_progn(new_exec_expr->len, new_exec_expr);
		
		ret = L_progn(o_exec_expr->len, o_exec_expr);
		// preeval_copy(exec_expr->car, o_exec_expr->car);
		
		
		// __print_obj_full(__eval(exec_expr->car));
		// usleep(200);
		// sleep(1);
	}

	ret:
	return ret;
	// return NT(o);
}

OBJ* L_if(OBJ_FN_ARGS)
{
	OBJ* ret = NIL;
	OBJ* cond_expr = o;
	OBJ* if_true_expr = NT(cond_expr);
	OBJ* else_expr = NT(if_true_expr);

	OBJ* tmp = __eval(cond_expr, cond_expr->len);
	if (tmp->num || tmp->type == T_TRUE) {
		ret = __eval(if_true_expr, if_true_expr->len);
	}
	else {
		ret = __eval(else_expr, else_expr->len);
	}

	ret:
	return ret;
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
		OBJ* tmp = __eval(cond_expr, cond_expr->len);
		// print_objf("tmp: ", tmp);


		if (tmp->num || tmp->type == T_TRUE) {
			// printd("aaa: %s\n", exec_expr->car->cdr->env->name);
			ret = __eval(exec_expr, exec_expr->len);
			// print_objf("ret: ", ret);
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
	if (o->type == T_LIST)
	{
		ITERATE_OBJECT(o->car, curr)
		{
			switch(curr->type)
			{
				case T_EXPR: case T_LIST:
					ret->cdr = L_copy(curr->len, curr);
				break;
				default:
					ret->cdr = empty_obj();
					*ret->cdr = *curr;
			}
			ret = ret->cdr;
		}
	}
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
	if (o->type = T_IDENTIFIER) o->str = o->name;
	size_t len = strlen(o->str);
	char x[len+6];
	char xx[len+6];
	x[0] = '.';
	x[1] = '/';
	strcpy(x+2, o->str);
	strcpy(xx, o->str);

	strcat(x, ".so");
	strcat(xx, "_load");
	printd("strcat: %s\n", x);
	printd("strcat: %s\n", xx);

	handle = dlopen(x, RTLD_LAZY | RTLD_GLOBAL);
	if (handle == NULL) {
		fputs(dlerror(), stderr);
		lobotomy_error("unable to load library '%s'\n", x);
	}
	fn = dlsym(handle, xx);
	if (fn == NULL) {
		lobotomy_error("unable to load library '%s' because %s couldn't be loaded\n", x, xx);
	}
	printd("load: %p\n", fn);
	fn(gcl, gcl->env);
	// dlclose(handle);
	
	return NIL;
}