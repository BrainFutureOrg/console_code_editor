result_name = TextManiac
source_files = src/main.c src/IO/DAO.c src/prj_types/String_struct.c src/test/string_tests.c src/test/test_funcs.c src/colors.c src/prj_error.c src/prj_types/String_struct_formatting.c src/IO/terminal_io.c src/bins.c src/test/speed_tests.c src/UI/user_interface.c src/IO/segments.c src/test/test_functionality_of_new_functions.c src/prj_types/String_array_struct.c src/file_system/file_system_work.c src/prj_types/Array_type.c src/signals_redefinition.c src/loging/log.c
flags = -Wall
libs = -lm
valgrind_flags = -v --tool=memcheck --leak-check=yes --track-origins=yes
create_version_script_source = ./src/get_git_name.sh

compile:
	gcc $(source_files) $(libs) $(flags) -o $(result_name)

compile_run:
	make get_git_name
	make compile
	./$(result_name)

run_in_terminal:
	gnome-terminal --command="make compile_run" &

run_with_valgrind:
	make compile
	valgrind $(valgrind_flags) ./TextManiac

get_git_name:
	bash $(create_version_script_source)