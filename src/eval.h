#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "obj.h"
#include "l_std.h"


OBJ* preeval_symbols(OBJ* o);
OBJ* preeval(OBJ* o);
void eval_program_file(const char* filename);
void eval_program(const char filename[], const char text[]);