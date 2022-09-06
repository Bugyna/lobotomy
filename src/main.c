#include "eval.h"


void interactive_shell(char* text)
{
	while (1) {
		fputs("#> ", stdout);
		fgets(text, 2047, stdin);
		interpret(text);
	}
}

int main(int argc, char* argv[]) {

	bool interactive = false;
	char* text = "";


	if (argc == 1) {
		text = read_file("d.lb");
	}

	else if (argc == 2) {
		if (argv[1][1] == 'i') {
			printf("here\n");
			interactive = true;
			text = malloc(2048);
		}
	}

	printf("size of OBJ: %ld\n", sizeof(OBJ));
	printf("size of OBJ*: %ld\n", sizeof(SCOPE));
	printf("size of OBJ*: %ld\n", sizeof(TREE));
	// printf("size of OBJ*: %ld\n", sizeof(FUNC));
	printf("text:\n%s\n", text);
	// parse(text);

	scope_init(&global, 200);
	global.func_scope = malloc(5*sizeof(SCOPE));
	global.func_index = 0;
	init();

	if (interactive) {
		interactive_shell(text);
	}
		
	else
		interpret_(text);


	return 0;
}