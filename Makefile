compile:
	gcc main.c IO/DAO.c prj_types/String_struct.c test/string_tests.c test/test_funcs.c colors.c prj_error.c prj_types/String_struct_formatting.c IO/terminal_io.c bins.c test/speed_tests.c test/speed_tests.h UI/user_interface.c UI/user_interface.h IO/segments.c IO/segments.h test/test_functionality_of_new_functions.c test/test_functionality_of_new_functions.h prj_types/String_array_struct.c -lm -Wall -o main

compile_run:
	make compile
	./main