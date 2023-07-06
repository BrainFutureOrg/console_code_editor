cmake_minimum_required(VERSION 3.25)
project(console_code_editor C)

set(CMAKE_C_STANDARD 11)

add_executable(console_code_editor main.c IO/DAO.c prj_types/String_struct.c test/string_tests.c test/test_funcs.c colors.c prj_error.c prj_types/String_struct_formatting.c)

target_link_libraries(console_code_editor m)