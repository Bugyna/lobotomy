#include "tokenize.h"


int main (int argc, char* argv[]) {

	if (argc > 1) {
		printf("no: %s\n", *argv);
	}

	tokenize("(+ 2 2)");

	

	return 0;
}
