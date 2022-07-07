#pragma once
#include "tokenize.h"


typedef struct OBJ OBJ;
typedef struct TREE TREE;


enum
{
	T_EXPR=1,
	T_FUNC,
	T_IDENTIFIER,
	T_NUMBER,
	T_DECIMAL,
	T_STR,
	T_LIST,
};

struct OBJ
{
	int type;
	int list_i, list_max;
	char* name; // NULL if literal
	
	union
	{
		char* str;
		int64_t number;
		double decimal;
		OBJ* list;
	};

};

struct TREE
{
	OBJ* expr;
	int index, max, peek;
};

void add_expr(TREE* tree, const OBJ expr)
{
	tree->expr[tree->index++] = expr;
	if (tree->index+1 >= tree->max) {
		tree->max += 10;
		tree->expr = realloc(tree->expr, tree->max);
	}
}

// void add_obj_to_expr(OBJ* expr, const OBJ obj)
// {
	// expr->args[expr->args_i++] = obj;
	// if (expr->args_i+1 >= expr->args_max) {
		// expr->args_max += 5;
		// expr->args = realloc(expr->args, expr->args_max*sizeof(OBJ));
	// }
// }


void _print_obj(OBJ obj, int indent_size)
{
	if (obj.type == 0)
		return;


	char* indent = malloc(indent_size);
	if (indent_size > 0 && indent_size < 100) {
		memset(indent, '\t', indent_size);
	}
	indent[indent_size] = '\0';
	
	printf("\n\n%s-----OBJECT-------\n\n%s%s", indent, indent, indent);
	// printf("\n\n-----OBJECT-------\n\n");
	// printf("debg: %d\n", obj.type);
	// if (obj.type == 0)
		// return;

	if ((obj.type == T_IDENTIFIER || obj.type == T_EXPR || obj.type == T_FUNC) && obj.name != NULL) {
		printf("%s", obj.name);
	}

	if (obj.type == T_NUMBER) {
		printf("%ld ", obj.number);
	}

	else if (obj.type == T_DECIMAL) {
		printf("%f ", obj.decimal);
	}

	else if (obj.type == T_STR) {
		printf("%s ", obj.str);
	}

	else if (obj.type == T_EXPR || obj.type == T_LIST) {
		for (int i = 0; i < obj.list_i; i++) {
			// printf("ddd: %d\n", obj.list[i].type);
			_print_obj(obj.list[i], indent_size+1);
		}
	}
	printf("\n\n%s------------------\n\n", indent);
	// printf("\n\n------------------\n\n");
}

void print_obj(OBJ obj)
{
	_print_obj(obj, 0);
}

void add_obj_to_obj(OBJ* dest, const OBJ src)
{
	dest->list[dest->list_i++] = src;
	if (dest->list_i >= dest->list_max) {
		dest->list_max += 10;
		dest->list = realloc(dest->list, dest->list_max*sizeof(OBJ));
	}
}



OBJ parse_expr(LEXER* lexer)
{
	OBJ expr;
	expr.name = NULL;

	OBJ obj;
	static const OBJ obj_reset;
	
	int p_count = 0;
	expr.list_i = 0;
	expr.list_max = 5;
	expr.list = malloc(expr.list_max*sizeof(OBJ));

	char* end;

	for (; lexer->peek < lexer->index; lexer->peek++) {
		if (lexer->tokens[lexer->peek].text == NULL)
			break;

		printf("token: %d %s\n", lexer->peek, lexer->tokens[lexer->peek].text);
		switch (lexer->tokens[lexer->peek].type)
		{
			case TT_LPAREN:
				p_count++;
				if (p_count > 1) {
					if (expr.name == NULL) {
						expr.type = T_LIST;
					}
					printf("\nnested expr\n");
					OBJ tmp_expr = parse_expr(lexer);
					// printf("fucK");
					// print_obj(tmp_expr);
					// obj.type = T_EXPR;
					// obj.expr = tmp_expr;

					add_obj_to_obj(&expr, tmp_expr);
					// i = tmp_expr.end;
					obj = obj_reset;
					lexer->peek++;
					// expr.end = tmp_expr.end;
					// printf("expr end: %d\n", tmp_expr.end);
					continue;
				}
				break;

			case TT_RPAREN:
				p_count--;
				// my_assert((p_count < 0), 
				if (p_count <= 0) {					
					goto exit;
				}
				break;

			case TT_NUMBER:
				obj.type = T_NUMBER;
				// &lexer.tokens[i].text[lexer.tokens[i].len]
				obj.number = strtol(lexer->tokens[lexer->peek].text, &end, 10);
				add_obj_to_obj(&expr, obj);
				obj = obj_reset;
				end = NULL;
				break;

			case TT_DECIMAL:
				obj.type = T_DECIMAL;
				obj.decimal = strtod(lexer->tokens[lexer->peek].text, &end);
				add_obj_to_obj(&expr, obj);
				obj = obj_reset;
				end = NULL;
				break;

			case TT_STR:
				obj.type = T_STR;
				obj.str = malloc(lexer->tokens[lexer->peek].len+1);
				obj.str = lexer->tokens[lexer->peek].text;
				add_obj_to_obj(&expr, obj);
				obj = obj_reset;
				break;

			case TT_IDENTIFIER:
				if (expr.name == NULL) {
					expr.type = T_EXPR;
					// printf("fucl: %s\n", lexer.tokens[lexer->peek].text);
					expr.name = malloc(lexer->tokens[lexer->peek].len+1);
					strcpy(expr.name, lexer->tokens[lexer->peek].text);
					// expr.keyword = lexer.tokens[lexer->peek].text;
				}

				else {
					obj.type = T_IDENTIFIER;
					// obj.identifier = lexer.tokens[i];
					obj.name = malloc(lexer->tokens[lexer->peek].len+1);
					strcpy(obj.name, lexer->tokens[lexer->peek].text);
					add_obj_to_obj(&expr, obj);
					obj = obj_reset;
				}
				break;
		}
	}

	exit:
		// lexer->peek++;
		// printf("parsed expr: %s\n", expr.keyword.text);
		printf("return\n");
		return expr;
	// printf("breaking at %d %s %d %d\n", i, lexer.tokens[i-2].text, expr.start, expr.end);
}

TREE parse(const char text[])
{
	TREE tree;
	tree.index = 0;
	tree.max = 20;
	tree.expr = malloc(tree.max*sizeof(OBJ));
	OBJ expr;


	LEXER lexer = tokenize(text);
	// add_expr(&tree, parse_expr(lexer, 0));
	printf("\n\n\n--------------------------PARSER-----------------------------------\n\n\n");
	for (;;) {
		if (lexer.tokens[lexer.peek].text == NULL)
			break;
		expr = parse_expr(&lexer);
		lexer.peek++;
		// printf("end of expr: %d\n", lexer.peek);
		// i = expr.end+1;
		add_expr(&tree, expr);
		printf("f: %d %s %d", expr.type, expr.name, expr.list_i);
		// print_obj(expr);
	}
	printf("\n\n\n------------------------------------------------------------------------\n\n\n");

	return tree;
}




