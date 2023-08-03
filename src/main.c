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
#include "../git_variables.h"
#endif

#define PROGRAM_NAME                "TextManiac"
#define SPACE_FOR_HELP              "    "

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

void UIprototype(file_system_anchor anchor, string filename)
{
    terminal_save_screen;
    cursor c = cursor_get_cursor_position();
    string text;
    if (strcmp(filename.line, "") == 0)
    {
        text = string_create_from_fcharp("");
    }
    else
    {
        text = anchor_read_file(anchor, filename.line);
    }
    start_plaintext_editor_UI_regular(&text, anchor, filename);
    free_string(text);
    terminal_restore_screen;
    cursor_go_to_position(c);
}

enum STOP_RESUME
{
    STOP_PROGRAM,
    RESUME_PROGRAM
};

void print_help(char *prog_name)
{
    printf("NAME\n" SPACE_FOR_HELP PROGRAM_NAME " - the text editor from BFO\n");
    printf("SYNOPSIS\n" SPACE_FOR_HELP "%s [OPTION]\n", prog_name);
    printf("OPTIONS\n");
    printf(SPACE_FOR_HELP "Parameter       Full version  Opinion\n");
    printf(SPACE_FOR_HELP "-h              --help        Show help info\n");
    printf(SPACE_FOR_HELP "-v              --version     Show version info\n");
    printf(SPACE_FOR_HELP "-o <file_name>  --open        Run program with already opened file that name is <file_name>\n");
    printf(SPACE_FOR_HELP "                --test        Run program tests\n");
}

void print_version()
{
    printf(PROGRAM_NAME " - Version alpha 0.0.%s\n\n", GIT_LAST_COMMIT_HASH);
    printf("It is free software: you are free to modify and distribute it.\n");
    printf("NO WARRANTIES are made to you, other than those provided by law.\n\n");
    printf("Authors of program are: Kosenko Olexander, Shkarupylo Maksym\n");
}

char *initial_file;

char split_on_dir_slash(char *c)
{
    if (*c == dir_slash)
        return 1;
    return 0;
}

enum STOP_RESUME argv_checker(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        write_log(INFO, "argc = %d", argc);
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            print_help(argv[0]);
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
            if (argc <= ++i)
            {
                printf("To open file, specify it's name after -o ar -open");
                return STOP_PROGRAM;
            }
            else
            {
                file_system_anchor anchor;
                if (*argv[i] == dir_slash)
                {
                    anchor.path = string_create_from_fcharp("");
                }
                else
                {
                    anchor = system_anchor_init();
                }
                string_array names = string_split(argv[i] + 1 + (*argv[i] != dir_slash), split_on_dir_slash);
                for (int j = 0; j < names.size - 1; j++)
                {
                    if (!anchor_is_file_exist(anchor,
                                              string_array_get_element(&names, j)
                                                  .line))//TODO: check existence of 1st folder in path
                    {
                        write_log(DEBUG,
                                  "nonexistent folder %s / %s",
                                  anchor.path.line,
                                  string_array_get_element(&names, j).line);
                        printf("You can not open nonexistent file/directory");
                        return STOP_PROGRAM;
                    }
                    system_anchor_go_to_dir(&anchor, string_array_get_element(&names, j));

                }
                if (!anchor_is_file_exist(anchor, string_array_get_element(&names, names.size - 1).line))
                {
                    write_log(DEBUG,
                              "nonexistent file %s / %s",
                              anchor.path.line,
                              string_array_get_element(&names, names.size - 1).line);
                    printf("You can not open nonexistent file/directory");
                    return STOP_PROGRAM;
                }
                string filename = string_array_pop(&names);

                write_log(DEBUG, "open from file, anchor=\"%s\", name=\"%s\"", anchor.path.line, filename.line);
                UIprototype(anchor, filename);

                free_string(filename);
                free_file_system_anchor(anchor);
            }
            return STOP_PROGRAM;
        }

        if (strcmp(argv[i], "--test") == 0)
        {
            tests();
            return STOP_PROGRAM;
        }
        printf("To see available options type '%s --help'\n", argv[0]);
        return STOP_PROGRAM;
    }
    return RESUME_PROGRAM;
}

int main(int argc, char **argv)
{
    init_logger(INFO, "log.txt");
    write_log(INFO, "Program start");
    redefine_signals();
    print_logo();

    if (argv_checker(argc, argv) == STOP_PROGRAM)
    {
        return 0;
    }

    string filename = string_create_from_fcharp("");
    file_system_anchor anchor = system_anchor_init();
    UIprototype(anchor, filename);
    free_string(filename);
    free_file_system_anchor(anchor);

    if (errno)
    {
        print_error();
    }
    write_log(INFO, "Program end");
    return 0;
}

void tests()
{
    apply_string_tests();//no delete
    print_test_success();// no delete
}
