#include "interpret.h"


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
		text = read_file("t.lb");
	}

	else if (argc == 2) {
		if (argv[1][1] == 'i') {
			printf("here\n");
			interactive = true;
			text = malloc(2048);
		}
	}

	printf("interactive: %d\n", interactive);
	printf("text:\n%s\n", text);
	// parse(text);

	global.occupied = 0;
	global.size = 200;
	global.values = malloc(global.size*sizeof(OBJ));
	init();

	printf("hash: %d\n", hash("exit") % 200);

	if (interactive)
		interactive_shell(text);
	else
		interpret(text);


	return 0;
}