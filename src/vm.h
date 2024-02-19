#pragma once
#include "util.h"
#include "obj.h"


typedef struct
{
	void* memory_pool;
	

	ENV* global_env;
	u8 op_stack[];

	
} LVM;