#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "obj.h"


OBJ* eval_into(OBJ* base, OBJ* res);
OBJ* preeval_symbols(OBJ* o);
OBJ* preeval(OBJ* o);
OBJ* __eval(OBJ* head, int argc);
void eval_program_file(const char* filename);
void eval_program(const char filename[], const char text[]);