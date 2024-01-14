#include <stdio.h>
#include "obj.c"


OBJ* hello_world(OBJ* o)
{
	return &((OBJ){.type=T_STR, .name="hello world", .str="hello world"});
}

void sample_module_load(ENV* env)
{
	env_add(env, create_cfn("hello_world", hello_world));
}