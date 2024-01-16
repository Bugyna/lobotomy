#pragma once
#include <wchar.h>
#include "lexer.h"
#include "util.h"
#include "obj.h"


typedef struct
{
	OBJ* head;
	OBJ* tail;
} OBJ_PAIR;

OBJ parse_map(LEXER*, int, int);
OBJ*  parse_object(LEXER*, int, int);
OBJ_PAIR parse_expr(LEXER*, int, int);

OBJ parse_atom(TOKEN t)
{
	OBJ atom;
	atom.env = global_env;
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

OBJ parse_map(LEXER* lexer, int p, int b)
{
	OBJ map;
	map.name = malloc(2);
	map.type = T_MAP;
	map.env = global_env;

	
	map.map = calloc(1, sizeof(ENV));
	ENV_INIT(map.map, 10);
	for (; lexer->peek < lexer->index; )
	{
		TOKEN t = lexer->tokens[lexer->peek];
		switch (t.type)
		{
			case TT_RCBRACKET:
				lexer->peek++;
				goto exit;
			break;
		}
		OBJ* key = parse_object(lexer, p, b);
		OBJ* val = parse_object(lexer, p, b);
		print_objf("key: ", key);
		print_objf("val: ", val);
		val->name = key->name;
		env_add(map.map, val);
		print_objf("bb: ", env_get(map.map, key->name));
	}

	exit:
		return map;
}



OBJ* parse_object(LEXER* lexer, int p, int b)
{
	if (lexer->peek >= lexer->index) return NIL;
	TOKEN t = lexer->tokens[lexer->peek++];
	OBJ* cur = empty_obj();
	OBJ_PAIR pair;

	switch (t.type)
	{
		case TT_LPAREN:
			printd("starting list\n");
			lexer->p_count++;
			cur->type = T_EXPR;
			cur->car = parse_expr(lexer, p+1, b).head;
		break;

		case TT_LBRACKET:
			cur->type = T_LIST;
			// lexer->peek++;
			cur->car = parse_expr(lexer, p, b+1).head;

			// cur->car = pair.head;

		break;

		case TT_LCBRACKET:
			*cur = parse_map(lexer, p, b);
			cur->cdr = NIL;
		break;

		case TT_RPAREN:
			lexer->p_count--;
			if (lexer->p_count <= p) {
				printd("ending list\n");
				goto exit;
			}
		break;

		case TT_RBRACKET:
			lexer->b_count--;
			if (lexer->b_count <= b)
				goto exit;
		break;


		default:
			*cur = parse_atom(t);
			cur->cdr = NIL;
	}


	exit:
	print_of(cur, "returning: %s %d %d ", t.text, p, b);
	return cur;
}


OBJ_PAIR parse_expr(LEXER* lexer, int p, int b)
{
	TOKEN t = lexer->tokens[lexer->peek];
	// printf("start of expr %d: \n", t.start);
	if (p == 0) print_token_pos(t, "start of expr");
	// OBJ* head = empty_obj_t(T_EXPR);

	OBJ* head = parse_object(lexer, p, b);
	OBJ* cur = head;
	cur->cdr = parse_object(lexer, p, b);
	
	for (; lexer->peek < lexer->index; )
	{
		if (cur->cdr->type == T_NIL) goto exit;
		print_of(cur->cdr, "still going %d %d\n", lexer->peek, lexer->text_len);
		cur = cur->cdr;
		cur->cdr = parse_object(lexer, p, b);
		// cur = cur->cdr;
	}
	// if (cur->cdr->type == T_NIL) goto exit;
	// else cur->cdr->cdr = parse_object(lexer, n);
	

	exit:
	// if (n == 0) print_token_pos(t, "end of expr: ");
	// cur->cdr = NULL;
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
		head = parse_expr(&lexer, 0, 0).head;
		
		lexer.peek++;
		OBJ_LIST_APPEND(parsed, head);
	}

	// printf("\n\n-------------------\n\n");
	return parsed;
}




