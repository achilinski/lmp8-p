CC=gcc




alex:
	                $(CC) test_alex.c alex.c fun_stack.c parser.c -oanalizator

funstack:
	                $(CC) funstack_test.c fun_stack.c -ofunstack-
