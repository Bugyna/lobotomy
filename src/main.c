#define DEBUGGING 1

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>
#include <dlfcn.h>

#include "util.h"
#include "tui.h"
#include "obj.c"
#include "l_std.h"
#include "eval.c"




// void interactive_shell()
// {
	// printf = printw;
	// wint_t c;
	// // fgetc(stdin);
	// tui_init();
	// exit = &tui_exit;
	// char text[1000] = "";
	// int index = 0;
	// int x = 0, y = 0;

	// while (1) {
		// printw("#> ");
		// for (; c != '\n'; )
		// {
			// getyx(stdscr, y, x);
			// get_wch(&c);
			// // printw("dd: %d\n", c);
			// if (c == KEY_BACKSPACE || c == KEY_DC || c == 127 || c == '\b' || c == 7) {
				// if (index-1 >= 0) {
					// index--;
					// move(y, x-1);
					// delch();
				// }
			// }
			
			// else {
				// switch (c)
				// {
					// case KEY_UP:
						
					// break;
					// case KEY_DOWN:
						
					// break;
					// case KEY_LEFT:
						
					// break;
					// case KEY_RIGHT:
						
					// break;
					// default:
						// addch(c);
						// text[index++] = c;
				// }
			// }
		// }
		// addch(c);
		// text[index] = '\0';
		// c = '\0';
		// index = 0;
		// // printw(":: %s\n", text);
		// eval_program(text);
		// // y = getcury(stdscr);
		// // move(y, 0);
		// // refresh();
	// }
// }

void interactive_shell()
{
	char text[1000] = "";
	while (1) {
		fputs("#> ", stdout);
		fgets(text, 1000, stdin);
		eval_program(text);
	}
}

int main(int argc, char* argv[]) {

	bool interactive = false;
	char* text = "";
	const char help_text[] = "lob error: No arguments provided\nUsage:\n\tlob [file] -> interprets a file\n\tlob -i opens a repl enviroment\n\n";
	printd("orintf: %p\n", printf);


	if (argc == 1) {
		// text = read_file("t2.lb");
		fputs(help_text, stderr);
	}

	else if (argc == 2) {
		if (argv[1][1] == 'i') {
			interactive = true;
			// text = malloc(2048);
		}

		else {
			text = read_file(argv[1]);
		}
	}

	printd("orintf: %p %p\n", printw, printf);
	// printf("size of OBJ: %ld\n", sizeof(OBJ));
	// printf("size of OBJ*: %ld\n", sizeof(SCOPE));
	// printf("size of OBJ*: %ld\n", sizeof(TREE));
	// printf("size of OBJ*: %ld\n", sizeof(FUNC));

	// printf("h: %s\n", "tést");
	// printf("h: %08x\n", U'é');
	// // printf("hash: %d\n", hash("ábc"));
	// return 0;
	printd("hash test: %s, %d \n", "help", hash("help"));
	GC_init();
	global_env = calloc(1, sizeof(ENV));
	global_env->name = "global_env";
	ENV_INIT(global_env, 100);
	lobotomy_init(global_env);
	// L_help(NIL);

	if (interactive) {
		interactive_shell();
	}
	else {
		eval_program(text);
	}


	return 0;
}