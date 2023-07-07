#include <stdio.h>
#include <errno.h>
#include "test/string_tests.h"
#include "test/test_funcs.h"
#include "IO/terminal_io.h"
#include "UI/user_interface.h"
#include "colors.h"
#include "IO/DAO.h"
//#define program_logo printf("\033[48;2;0;0;0m\033[38;2;200;150;50mC Code \033[48;2;200;150;50m\033[38;2;0;0;0mEditor\n" DEFAULT_COLOR)

int main(int argc, char **argv)
{
    print_logo();
    apply_string_tests();//no delete
    print_test_success();// no delete

//    string file_text = read_file("example.txt");
//    save_file(file_text, "smth.txt");
//    printf("%s", file_text.line);
//    free_string(file_text);

    read_keys();//no delete
    if (errno)
    {
        printf("Fuck!\n");
    }
/*
    void speed_test();
    speed_test();*/

    return 0;
}


