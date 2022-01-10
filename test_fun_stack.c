#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fun_stack.h"


int main() {
	put_on_fun_stack(1, "jakas_funkcja_1");
	put_on_fun_stack(69, "jakas_inna_funkcja");

	printf("%d\n", top_of_funstack());
	printf("%s\n", get_from_fun_stack());
	printf("%d\n", top_of_funstack());

	return 0;
}