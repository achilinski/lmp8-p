#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "alex.h"

int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		analizatorSkladni(argv[i]);
	}
	wypisz_funkcje();
	return 0;
}

