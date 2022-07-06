// #include "tokenize.h"
#include "interpret.h"


int main (int argc, char* argv[]) {

	if (argc > 1) {
		printf("no: %s\n", *argv);
	}

	const char* text = read_file("t.lb");
	printf("text:\n%s\n", text);
	// parse(text);
	interpret(text);

	

	return 0;
}
