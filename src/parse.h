#pragma once
#include "tokenize.h"
#include "obj.h"



OBJ parse_expr(LEXER* lexer)
{
	SCOPE tmp_scope;
	bool in_func_def = 0;

	OBJ expr;
	expr.name = NULL;
	expr.type = T_LIST;

	OBJ obj;
	static const OBJ obj_reset;
	
	int p_count = 0;
	expr.index = 0;
	expr.list = malloc(5*sizeof(OBJ));

	char* end;

	for (; lexer->peek < lexer->index; lexer->peek++) {
		if (lexer->tokens[lexer->peek].text == NULL)
			break;

		// printf("token: %d %s\n", lexer->peek, lexer->tokens[lexer->peek].text);
		switch (lexer->tokens[lexer->peek].type)
		{
			case TT_LPAREN:
				p_count++;
				if (p_count > 1) {
					// if (expr.name == NULL) {
						// expr.type = T_LIST;
					// }
					// printf("\nnested expr\n");
					OBJ tmp_expr = parse_expr(lexer);
					// print_obj_full(tmp_expr);
					// obj.type = T_EXPR;
					// obj.expr = tmp_expr;
					add_obj_to_obj(&expr, tmp_expr);
					printf("tmp_expr: %s %s\n", type_name(tmp_expr.type), type_name(expr.list[expr.index-1].type));
					// i = tmp_expr.end;
					obj = obj_reset;
					lexer->peek--;
					// expr.end = tmp_expr.end;
					// printf("expr end: %d\n", tmp_expr.end);
					continue;
				}
				break;

			case TT_RPAREN:
				p_count--;
				// my_assert((p_count < 0), 
				// if ((expr.type == T_UNDEFINED || expr.type == T_NULL) && expr.index > 0) {
					// expr.type = T_LIST;
					// expr.name = NULL;
				// }

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
				if (expr.index == 1 && strcmp(expr.list[0].name, "func") == 0) {
					expr.type = T_FUNC_DEF;
				}
				// if (expr.name == NULL) {
					// expr.type = T_EXPR;
					// printf("fucl: %s\n", lexer.tokens[lexer->peek].text);
					
					// expr.name = malloc(lexer->tokens[lexer->peek].len+1);
					// strcpy(expr.name, lexer->tokens[lexer->peek].text);
					// OBJ tmp = find_in_scope(global, obj.name);
					// if (tmp.type != T_UNDEFINED) {
						// obj = tmp;
						// printf("obj type replace: %s\n", type_name(obj.type));
					// }
					// expr.keyword = lexer.tokens[lexer->peek].text;
				// }

				// else {
					obj.type = T_IDENTIFIER;
					obj.name = malloc(lexer->tokens[lexer->peek].len+1);
					strcpy(obj.name, lexer->tokens[lexer->peek].text);
					// printf("ident: %s\n", obj.name);
					
					// OBJ tmp = find_in_scope(global, obj.name);
					// if (expr.type != T_FUNC_DEF && tmp.type != T_UNDEFINED) {
						// obj = tmp;
					// }

					add_obj_to_obj(&expr, obj);
					obj = obj_reset;
				// }
				break;
		}
	}

	exit:
		// lexer->peek++;
		// printf("parsed expr: %s\n", expr.name);
		// printf("return\n");
		// if (expr.list[0].type == T_IDENTIFIER && strcmp(expr.list[0].name, "func") == 0) {
			// expr.type = T_FUNC_DEF;
			// add_to_scope(&global, expr);
		// }

		// if (expr.type == T_FUNC_DEF) {
			// if (expr.index >= 3) {
				// for (int iter = 0; iter < expr.list[3].index; iter++) {
					// for (int iter1 = 0; iter1 < expr.list[2].index; iter1++) {
						// if (
							// expr.list[3].list[iter].type == T_IDENTIFIER &&
							// strcmp(expr.list[2].list[iter1].name, expr.list[3].list[iter].name) == 0
						// ) {
							// expr.list[3].list[iter].ref = expr.list[2].list[iter1].ref;
							// expr.list[3].list[iter].type = T_REF;
						// }
						
					// }
				// }
			// }
			expr.type = T_LIST;
		// }
		return expr;
	// printf("breaking at %d %s %d %d\n", i, lexer.tokens[i-2].text, expr.start, expr.end);
}


TREE parse(const char text[])
{
	global.name = "global";
	TREE tree;
	tree.index = 0;
	tree.max = 20;
	tree.expr = malloc(tree.max*sizeof(OBJ));
	OBJ expr;


	LEXER lexer = tokenize(text);
	// add_expr(&tree, parse_expr(lexer, 0));
	for (;;) {
		if (lexer.tokens[lexer.peek].text == NULL)
			break;
		expr = parse_expr(&lexer);
		
		// printf("name: %s<%s> %d\n", expr.name, type_name(expr.type), expr.type);
		// print_obj(expr);
		lexer.peek++;
		// printf("end of expr: %d\n", lexer.peek);
		// i = expr.end+1;
		add_expr(&tree, expr);
		// printf("f: %d %s %d", expr.type, expr.name, expr.index);
		// print_obj_full(expr);
	}
	// printf("\n\n\n--------------------------PARSER DONE-----------------------------------\n\n\n");
	return tree;
}




