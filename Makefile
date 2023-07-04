compile:
	gcc main.c DAO.c prj_types/String_struct.c prj_types/String_struct_formatting.c test/string_tests.c test/test_funcs.c colors.c prj_error.c -lm -Wall -o main

compile_run:
	make compile
	./main