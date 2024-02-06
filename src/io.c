#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "io.h"
#include "util.h"
#include "obj.c"



struct L_FILE
{
	i32 flags;
	FILE* file;
};