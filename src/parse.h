#pragma once
#include <wchar.h>
#include "lexer.h"
#include "util.h"
#include "obj.h"


typedef struct
{
	
} PARSER;

typedef struct
{
	OBJ* head;
	OBJ* tail;
	int size;
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
	OBJ* cur = NIL;
	OBJ_PAIR pair;

	switch (t.type)
	{
		case TT_LPAREN:
			// printd("starting list\n");
			lexer->p_count++;
			cur = empty_obj();
			cur->type = T_EXPR;
			pair = parse_expr(lexer, p+1, b);
			cur->car = pair.head;
			cur->len = pair.size;
			// lexer->peek--;
		break;

		case TT_LBRACKET:
			lexer->b_count++;

			cur = empty_obj();

			cur->type = T_LIST;
			pair = parse_expr(lexer, p, b+1);
			cur->car = pair.head;
			cur->len = pair.size;
		break;

		case TT_LCBRACKET:
			cur = empty_obj();
			*cur = parse_map(lexer, p, b);
			cur->cdr = NIL;
		break;

		case TT_RPAREN:
			lexer->p_count--;
			if (lexer->p_count <= p) {
				// printd("ending list\n");
				goto exit;
			}
		break;

		case TT_RBRACKET:
			lexer->b_count--;
			if (lexer->b_count <= b)
				goto exit;
		break;

		case TT_ACCESS:
			cur = parse_object(lexer, p, b);
			cur->env_name = t.text;
		break;


		default:
			cur = empty_obj();
			*cur = parse_atom(t);
			cur->cdr = NIL;
	}


	exit:
	// print_of(cur, "returning: %s %d %d ", t.text, p, b);
	return cur;
}


OBJ_PAIR parse_expr(LEXER* lexer, int p, int b)
{
	TOKEN t = lexer->tokens[lexer->peek];
	// printf("start of expr %d: \n", t.start);
	// if (p == 0) print_token_pos(t, "start of expr");
	// OBJ* head = empty_obj_t(T_EXPR);
	int size = 1;
	
	OBJ* head = parse_object(lexer, p, b);
	OBJ* cur = head; // needs to be here because it gets returned as .tail in OBJ_PAIR
	if (p == 0 && b == 0) lexer->peek--; // without this it connects all expressions when parsing a file
	if (head->type == T_NIL) {
		goto exit;
	}

	
	for (; lexer->peek <= lexer->index; )
	{
		cur->cdr = parse_object(lexer, p, b);
		if (cur->cdr->type == T_NIL) goto exit;
		// print_of(cur->cdr, "still going %d %d\n", lexer->peek, lexer->text_len);
		size++;
		cur = cur->cdr;
		// cur->cdr = parse_object(lexer, p, b);
		// cur = cur->cdr;
	}
	// if (cur->cdr->type == T_NIL) goto exit;
	// else cur->cdr->cdr = parse_object(lexer, n);
	// lexer->peek++;
	exit:
	// if (n == 0) print_token_pos(t, "end of expr: ");
	// cur->cdr = NULL;
	// printf("deep: %d %s %d\n", n, type_name(cur->type), lexer->peek);
	// print_obj_full(head);
	return (OBJ_PAIR){.head=head, .tail=cur, .size=size};
}



OBJ_LIST* parse(const char filename[], const char text[])
{
	OBJ_LIST* parsed = malloc(sizeof(OBJ_LIST));
	OBJ_LIST_INIT(parsed);
	OBJ* head;

	LEXER lexer = tokenize(filename, text);

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




