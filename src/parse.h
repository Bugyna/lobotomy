#pragma once
#include "tokenize.h"

typedef struct OBJ OBJ;
typedef struct EXPR EXPR;
typedef struct TREE TREE;


enum
{
	T_EXPR=0,
	T_IDENTIFIER,
	T_NUMBER,
	T_DECIMAL,
	T_STR,
	T_LIST,
};

struct EXPR
{
	char* keyword;
	OBJ* args;
	int args_i, args_max;
	int start, end;

};

struct OBJ
{
	int type;
	union
	{
		char* identifier;
		char* str;
		int64_t number;
		double decimal;
		OBJ* list;
		EXPR expr;
	};

};

struct TREE
{
	EXPR* expr;
	int index, max;
};

void add_expr(TREE* tree, const EXPR expr)
{
	tree->expr[tree->index++] = expr;
	if (tree->index+1 >= tree->max) {
		tree->max += 10;
		tree->expr = realloc(tree->expr, tree->max);
	}
}

void add_obj_to_expr(EXPR* expr, const OBJ obj)
{
	expr->args[expr->args_i++] = obj;
	if (expr->args_i+1 >= expr->args_max) {
		expr->args_max += 5;
		expr->args = realloc(expr->args, expr->args_max*sizeof(OBJ));
	}
}


void print_expr(EXPR expr)
{
	printf("\n\n-----EXPR-------\n\n");
	printf("keyword: %s ", expr.keyword);
	for (int i = 0; i < expr.args_max; i++) {
		if (expr.args[i].type == NULL)
			break;

		// printf("type: %d ", expr.args[i].type);

		print_obj(expr.args[i]);
	}
	printf("\n\n----------------\n\n");
	
}

void print_obj(OBJ obj)
{
	printf("\n\n-----OBJECT-------\n\n");
	if (obj.type == NULL)
		return;

	if (obj.type == T_NUMBER) {
		printf("%ld ", obj.number);
	}

	else if (obj.type == T_DECIMAL) {
		printf("%f ", obj.decimal);
	}

	else if (obj.type == T_STR) {
		printf("%s ", obj.str);
	}

	else if (obj.type == T_IDENTIFIER) {
		printf("%s ", obj.identifier);
	}
	printf("\n\n------------------\n\n");
}



EXPR parse_expr(LEXER lexer, int i)
{
	EXPR expr;
	expr.keyword = NULL;

	OBJ obj;
	static const OBJ obj_reset;
	
	int p_count = 0;
	expr.start = i;
	expr.args_i = 0;
	expr.args_max = 5;
	expr.args = malloc(expr.args_max*sizeof(OBJ));

	char* end;

	for (; i < lexer.size; i++) {
		if (lexer.tokens[i].text == NULL)
			break;

		printf("token: %d %s\n", i,lexer.tokens[i].text);
		switch (lexer.tokens[i].type)
		{
			case TT_LPAREN:
				p_count++;
				if (p_count > 1) {
					EXPR tmp_expr = parse_expr(lexer, i+1);
					obj.type = T_EXPR;
					obj.expr = tmp_expr;

					add_obj_to_expr(&expr, obj);
					i = tmp_expr.end;
					obj = obj_reset;
					// expr.end = tmp_expr.end;
					printf("expr end: %d\n", tmp_expr.end);
					continue;
				}
				break;

			case TT_RPAREN:
				p_count--;
				if (p_count <= 0) {
					expr.end = i;
				}
				goto exit;
				break;

			case TT_NUMBER:
				obj.type = T_NUMBER;
				// &lexer.tokens[i].text[lexer.tokens[i].len]
				obj.number = strtol(lexer.tokens[i].text, &end, 10);
				add_obj_to_expr(&expr, obj);
				obj = obj_reset;
				end = NULL;
				break;

			case TT_DECIMAL:
				obj.type = T_DECIMAL;
				obj.decimal = strtod(lexer.tokens[i].text, &end);
				add_obj_to_expr(&expr, obj);
				obj = obj_reset;
				end = NULL;
				
				break;

			case TT_STR:
				obj.type = T_STR;
				obj.str = malloc(lexer.tokens[i].len+1);
				obj.str = lexer.tokens[i].text;
				add_obj_to_expr(&expr, obj);
				obj = obj_reset;
				break;

			case TT_IDENTIFIER:
				if (expr.keyword == NULL) {
					printf("fucl: %s\n", lexer.tokens[i].text);
					expr.keyword = malloc(lexer.tokens[i].len+1);
					strcpy(expr.keyword, lexer.tokens[i].text);
					// expr.keyword = lexer.tokens[i].text;
				}

				else {
					obj.type = T_IDENTIFIER;
					// obj.identifier = lexer.tokens[i];
					obj.identifier = malloc(lexer.tokens[i].len+1);
					strcpy(obj.identifier, lexer.tokens[i].text);
					add_obj_to_expr(&expr, obj);
					obj = obj_reset;
				}
				break;
		}
	}

	exit:
		// printf("parsed expr: %s\n", expr.keyword.text);
		// print_expr(expr);
		return expr;
	// printf("breaking at %d %s %d %d\n", i, lexer.tokens[i-2].text, expr.start, expr.end);
}

TREE parse(const char text[])
{
	TREE tree;
	tree.index = 0;
	tree.max = 20;
	tree.expr = malloc(tree.max*sizeof(EXPR));
	EXPR expr;


	LEXER lexer = tokenize(text);
	// add_expr(&tree, parse_expr(lexer, 0));
	for (int i = 0;;) {
		if (lexer.tokens[i].text == NULL)
			break;
		expr = parse_expr(lexer, i);
		printf("end of expr: %d\n", i);
		i = expr.end+1;
		add_expr(&tree, expr);
	}
	printf("\n\n\n--------------------------PARSER DONE-----------------------------------\n\n\n");
	printf("\n\n\n------------------------------------------------------------------------\n\n\n");

	return tree;
}




