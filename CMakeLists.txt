cmake_minimum_required(VERSION 3.25)
project(console_code_editor C)

set(CMAKE_C_STANDARD 11)

execute_process(COMMAND git log -1 --pretty=format:%h
        OUTPUT_VARIABLE outv)
file(WRITE src/git_variables.h "#define GIT_LAST_COMMIT_HASH \"${outv}\"")

add_executable(console_code_editor src/main.c src/IO/DAO.c src/prj_types/String_struct.c src/test/string_tests.c src/test/test_funcs.c src/colors.c src/prj_error.c src/prj_types/String_struct_formatting.c src/IO/terminal_io.c src/bins.c src/test/speed_tests.c src/UI/user_interface.c src/IO/segments.c src/test/test_functionality_of_new_functions.c src/prj_types/String_array_struct.c src/file_system/file_system_work.c src/prj_types/Array_type.c src/signals_redefinition.c src/loging/log.c)

target_link_libraries(console_code_editor m)