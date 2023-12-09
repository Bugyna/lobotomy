#pragma once
#include "lexer.h"
#include "obj.h"

typedef struct
{
	OBJ* head;
	OBJ* tail;
} OBJ_PAIR;

OBJ parse_atom(TOKEN t)
{
	OBJ atom;
	char* end;
	atom.name = malloc(1);
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
			// atom->str = malloc(t.len);
			atom.str = t.text;
			// atom.index = t.len-1;
		break;
		
		case TT_IDENTIFIER:
			free(atom.name);
			atom.type = T_IDENTIFIER;
			atom.name = malloc(t.len+1);
			strcpy(atom.name, t.text);
	
		break;
	}
	return atom;
}

OBJ_PAIR parse_expr(LEXER* lexer, int n)
{
	TOKEN t = lexer->tokens[lexer->peek];
	// printf("start token of %d: %s\n", n, t.text);
	OBJ* head = empty_obj();
	head->type = T_EXPR;
	OBJ* cur = head;
	int p_count = 0, b_count = 0;

	// bool last_was_quote = false;
	
	for (; lexer->peek < lexer->index; lexer->peek++) {
		t = lexer->tokens[lexer->peek];
		printf("N: %d, b: %d ", n, b_count);
		// printf("token: %d\n", lexer->peek);
		print_token(t);
		// print_obj_simple(cur);

		switch (t.type)
		{
			case TT_LPAREN:
				p_count++;
				if (p_count > 1) {
					cur->type = T_EXPR;
					// printf("starting rec: %d\n", lexer->peek);
					// lexer->peek++;
					OBJ_PAIR pair = parse_expr(lexer, n+1);
					// printf("end rec: %d\n", lexer->peek);
					// OBJ* tmp = cur->cdr;
					cur->car = pair.head;
					// print_obj_simple(cur);
					// cur = tmp;
					cur->cdr = empty_obj();
					cur = cur->cdr;
					// OBJ tmp_expr = parse_expr(lexer);
					// add_obj_to_obj(&expr, tmp_expr);

					// obj = obj_reset;
					// lexer->peek--;
					p_count--;
				}
			break;

			case TT_LBRACKET:
				printf("hereaaaaaa\n");
				b_count++;
				lexer->b_count++;
				cur->type = T_LIST;
				lexer->peek++;
				OBJ_PAIR pair = parse_expr(lexer, n+1);
				// b_count--;
				// lexer->peek--;
				cur->car = pair.head;
				cur->cdr = empty_obj();
				cur = cur->cdr;

			break;

			case TT_RPAREN:
				p_count--;
				if (p_count <= 0) {
					goto exit;
				}
			break;

			case TT_RBRACKET:
				lexer->b_count--;
				b_count--;
				if (b_count <= 0) {
					goto exit;
				}
			break;


			default:
				*cur = parse_atom(t);
				if (lexer->tokens[lexer->peek+1].type == TT_RPAREN) continue;
				// *cur->cdr = empty_obj();
				cur->cdr = empty_obj();
				cur = cur->cdr;
		}
	}


	exit:
	cur->cdr = NULL;
	// printf("deep: %d %s %d\n", n, type_name(cur->type), lexer->peek);
	// __print_obj_full(head);
	return (OBJ_PAIR){.head=head, .tail=cur};
}



OBJ_LIST* parse(const char text[])
{
	OBJ_LIST* parsed = malloc(sizeof(OBJ_LIST));
	OBJ_LIST_INIT(parsed);
	OBJ* head;

	LEXER lexer = tokenize(text);

	for (;;) {
		if (lexer.tokens[lexer.peek].text == NULL)
			break;
		head = parse_expr(&lexer, 0).head;
		// print_obj_simple(head);
		
		lexer.peek++;
		OBJ_LIST_APPEND(parsed, head);
	}

	printf("\n\n-------------------\n\n");
	return parsed;
}




