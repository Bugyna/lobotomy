#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <float.h>
#include <stdint.h>


enum
{
	TT_LPAREN=1,
	TT_RPAREN,
	TT_PLUS,
	TT_MINUS,
	TT_MUL,
	TT_DIV,
	TT_AND,
	TT_OR,
	TT_NOT,
	TT_LESS,
	TT_GREATER,
	TT_EQUAL,
	TT_BIN_LESS,
	TT_BIN_GREATER,
	TT_BIN_EQUAL,
}



void tokenize(const char text[])
{
	
}