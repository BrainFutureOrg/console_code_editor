compile:
	gcc main.c IO/DAO.c prj_types/String_struct.c test/string_tests.c test/test_funcs.c colors.c prj_error.c prj_types/String_struct_formatting.c IO/terminal_io.c bins.c test/speed_tests.c UI/user_interface.c IO/segments.c test/test_functionality_of_new_functions.c prj_types/String_array_struct.c file_system/file_system_work.c prj_types/Array_type.c signals_redefinition.c loging/log.c -lm -Wall -o main

compile_run:
	make compile
	./main