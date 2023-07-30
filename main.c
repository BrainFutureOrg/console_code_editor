#include <stdio.h>
#include <errno.h>
#include "test/string_tests.h"
#include "test/test_funcs.h"
#include "UI/user_interface.h"
#include "IO/segments.h"
#include "IO/terminal_io.h"
#include "test/test_functionality_of_new_functions.h"
#include "loging/log.h"
#include "signals_redefinition.h"
#include "prj_error.h"

#if __has_include("git_variables.h")
#include "git_variables.h"
#endif

#ifndef GIT_LAST_COMMIT_HASH
#define GIT_LAST_COMMIT_HASH "hasn't_version"
#endif

void tests();

//void moving_text_prototype()
//{
//    terminal_erase_screen;
//    color_to_rgb_background(0, 0, 50);
//    string text = string_create_from_fcharp("");
//    urectangle region = {5, 15, 5, 50};
//    //start_moving_readonly_segment(text, region);
//    start_write_segment(&text, region);
//    free_string(text);
//    color_to_default();
//}

void UIprototype()
{
    string text = string_create_from_fcharp("");
    start_plaintext_editor_UI_regular(&text);
    free_string(text);
}

enum STOP_RESUME
{
    STOP_PROGRAM,
    RESUME_PROGRAM
};

void print_help()
{
    printf("Here will be some help info\n");
}
void print_version()
{
    printf("TextManiac - Version alpha 0.0.%s\n", GIT_LAST_COMMIT_HASH);
}

enum STOP_RESUME argv_checker(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        write_log(INFO, "argc = %d\n", argc);
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            print_help();
            return STOP_PROGRAM;
        }
        if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0)
        {
            print_version();
            return STOP_PROGRAM;
        }
        if (strcmp(argv[i], "--open") == 0 || strcmp(argv[i], "-o") == 0)
        {
            write_log(DEBUG, "open\n");
            //TODO: realization
            return RESUME_PROGRAM;
        }

        if (strcmp(argv[i], "--test") == 0)
        {
            tests();
            return STOP_PROGRAM;
        }
        printf("To see available options type '%s --help'\n", argv[0]);
        return STOP_PROGRAM;
    }
    return STOP_PROGRAM;
}

int main(int argc, char **argv)
{
    init_logger(DEBUG, "log.txt");
    write_log(INFO, "Program start");
    redefine_signals();
    print_logo();

    if (argv_checker(argc, argv) == STOP_PROGRAM)
    {
        return 0;
    }
    //test_cursor_functions();// test that prints strange things
//    test_console_games_functions(2);
//    test_urectangle_region_functionality();
//    test_interesting_picture();
//    read_keys();//no delete

    //moving_text_prototype();
    UIprototype();

    if (errno)
    {
        print_error();
        printf("Fuck!\n");
    }
    write_log(INFO, "Program end");
    return 0;
}

void tests()
{
    apply_string_tests();//no delete
    print_test_success();// no delete
}
