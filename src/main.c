// #include "parse.h"
// #include "lexer.h"
// #include "obj.h"

#include "eval.h"



void interactive_shell(char* text)
{
	while (1) {
		fputs("#> ", stdout);
		fgets(text, 2047, stdin);
		eval_program(text);
	}
}

int main(int argc, char* argv[]) {

	bool interactive = false;
	char* text = "";


	if (argc == 1) {
		text = read_file("t2.lb");
	}

	else if (argc == 2) {
		if (argv[1][1] == 'i') {
			printf("here\n");
			interactive = true;
			text = malloc(2048);
		}

		else {
			text = read_file(argv[1]);
		}
	}

	// printf("size of OBJ: %ld\n", sizeof(OBJ));
	// printf("size of OBJ*: %ld\n", sizeof(SCOPE));
	// printf("size of OBJ*: %ld\n", sizeof(TREE));
	// printf("size of OBJ*: %ld\n", sizeof(FUNC));

	// printf("h: %s\n", "tést");
	// printf("h: %08x\n", U'é');
	// // printf("hash: %d\n", hash("ábc"));
	// return 0;
	GC_init();
	global_env = calloc(1, sizeof(ENV));
	global_env->name = "global_env";
	printf("size 1: %p\n", global_env);
	ENV_INIT(global_env, 100);
	lobotomy_init(global_env);
	printf("size 2: %p\n", global_env);

	if (interactive) interactive_shell(text);
	else eval_program(text);


	return 0;
}