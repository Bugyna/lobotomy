#include <stdio.h>
#include "obj.c"


static OBJ* hello_world(OBJ_FN_ARGS)
{
	printf("hello-world\n");
	// return NIL;
	return &((OBJ){.type=T_STR, .name="hello world", .str="hello world"});
}

void sample_module_load(ENV* env)
{
	ENV_ADD(env, "hello_world", create_cfn("hello_world", hello_world));
	// env_add(env, create_cfn("hello-world", hello_world));
	// printf("hello world\n");
}