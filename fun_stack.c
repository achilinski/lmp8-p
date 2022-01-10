#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fun_stack.h"

fun_data stack;

int top_of_funstack(void) {
	if (stack.size == 0) {
		return 0;
	}
	return stack.data[stack.size - 1].par_level;
}

void put_on_fun_stack(int par_level, char* funame) {

	if (stack.size++ == 0) {
		stack.data = malloc(sizeof * stack.data);
	}
	else {
		stack.data = realloc(stack.data, stack.size * sizeof * stack.data);
	}
	stack.data[stack.size - 1].funame = malloc((strlen(funame) + 1) * sizeof * funame);
	stack.data[stack.size - 1].par_level = par_level;
	strcpy(stack.data[stack.size - 1].funame, funame);
}

char* get_from_fun_stack(void) {

	if (stack.size == 0) {
		fprintf(stderr, "Nie udało dostać się do stosu");
	}


	char* tmp = malloc((strlen(stack.data[stack.size - 1].funame) + 1) * sizeof * tmp);
	strcpy(tmp, stack.data[stack.size - 1].funame);
	stack.data = realloc(stack.data, stack.size-- * sizeof * stack.data);
	return tmp;
}