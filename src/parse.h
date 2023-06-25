#pragma once
#include "lexer.h"
#include "obj.h"



ENV global;
OBJ_LIST tree;

OBJ parse_expr(LEXER* lexer)
{
	OBJ obj;
}


OBJ parse_atom(TOKEN t)
{
	OBJ atom;
	char* end;
	switch (t.type)
	{
		case TT_NUMBER:
			atom.type = T_NUM;
			atom.num = strtol(t.text, &end, 10);
		break;
		
		case TT_DECIMAL:
			atom.type = T_DECIMAL;
			atom.decimal = strtod(t.text, &end);
		break;
		
		case TT_STR:
			atom.type = T_STR;
			atom.str = malloc(t.len);
			atom.str = t.text;
			// atom.index = t.len-1;
		break;
		
		case TT_IDENTIFIER:
			atom.type = T_IDENTIFIER;
			atom.name = malloc(t.len+1);
			strcpy(atom.name, t.text);
	
		break;
	}
	return atom;
}

OBJ parse_program(LEXER* lexer)
{
	ENV tmp_scope;
	bool in_func_def = 0;

	OBJ expr;
	expr.name = NULL;
	expr.type = T_LIST;
	expr.scope = &global;

	OBJ obj;
	static const OBJ obj_reset;
	
	int p_count = 0;
	// expr.index = 0;
	expr.list = malloc(5*sizeof(OBJ));

	char* end;

	for (; lexer->peek < lexer->index; lexer->peek++) {
		TOKEN t = lexer->tokens[lexer->peek];
		if (t.text == NULL)
			break;

		switch (t.type)
		{
			case TT_LPAREN:
				p_count++;
				if (p_count > 1) {
					OBJ tmp_expr = parse_expr(lexer);
					add_obj_to_obj(&expr, tmp_expr);
					obj = obj_reset;
					lexer->peek--;
					continue;
				}
				break;

			case TT_RPAREN:
				p_count--;
				if (p_count <= 0) {
					goto exit;
				}
				break;

			
		}
	}

	exit:
	expr.type = T_LIST;
	return expr;
}


OBJ_LIST parse(const char text[])
{
	global.name = "global";
	OBJ_LIST tree;
	OBJ_LIST_INIT(&tree);
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
		OBJ_LIST_APPEND(&tree, &expr);
		// printf("f: %d %s %d", expr.type, expr.name, expr.index);
		// print_obj_full(expr);
	}
	// printf("\n\n\n--------------------------PARSER DONE-----------------------------------\n\n\n");
	return tree;
}




