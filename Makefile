compile:
	gcc main.c IO/DAO.c prj_types/String_struct.c test/string_tests.c test/test_funcs.c colors.c prj_error.c prj_types/String_struct_formatting.c IO/terminal_io.c bins.c test/speed_tests.c UI/user_interface.c IO/segments.c test/test_functionality_of_new_functions.c prj_types/String_array_struct.c file_system/file_system_work.c prj_types/Array_type.c signals_redefinition.c loging/log.c -lm -Wall -o main

compile_run:
	make get_git_name
	make compile
	./main

run_in_terminal:
	gnome-terminal --command="make compile_run" &

run_with_valgrind:
	make compile
	valgrind -v --tool=memcheck --leak-check=yes --track-origins=yes ./main

get_git_name:
	bash get_git_name.sh