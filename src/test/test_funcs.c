//
// Created by maximus on 04.07.23.
//

#include "test_funcs.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include "../colors.h"
#include "../prj_error.h"
#include "../loging/log.h"

#define TEST_print_formated_error(ftext_error) va_list args;                  \
                                               va_start(args, ftext_error);   \
                                               color_to(FOREGROUND_RED);      \
                                               printf("ERROR text:");         \
                                               vprintf(ftext_error, args);    \
                                               putchar('\n');                 \
                                               color_to(DEFAULT);             \
                                               va_end(args);

#define unsuccess_test test_success = 0
#define unsuccess_test_actions(test_name, ftext_error) color_from_parts_printf(FOREGROUND_RED, "Test \"%s\" failed\n", \
                                            test_name == NULL ? "Unnamed" : test_name); \
                                            unsuccess_test;                          \
                                                                                     \
                                            if (errno){                              \
                                                print_error();                       \
                                                errno = 0;                           \
                                            }                                        \
                                                                                     \
                                            TEST_print_formated_error(ftext_error)

#define unsuccess_test_actions_log(test_name, ftext_error) write_log(ERROR, "Test \"%s\" failed\n", \
                                            test_name == NULL ? "Unnamed" : test_name); \
                                            unsuccess_test;                          \
                                                                                     \
                                            if (errno){                              \
                                                print_error();                       \
                                                errno = 0;                           \
                                            }                                        \
                                                                                     \
                                            va_list args;                  \
                                            va_start(args, ftext_error);   \
                                            write_vlog("ERROR text:" ftext_error, args);    \
                                            va_end(args);

char test_success = 1;
void print_test_success()
{
    if (test_success)
    {
        color_from_parts_printf(DEFAULT | BOLD | FOREGROUND_GREEN, "All tests successful\n");
    }
}

void print_test_success_log()
{
    if (test_success)
    {
        write_log(INFO, "All tests successful\n");
    }
}

void assertEqual(void *first,
                 void *second,
                 char (*equal_function)(void *first, void *second),
                 char *test_name,
                 char *ftext_error,
                 ...)
{
    char result = equal_function(first, second);
    if (result)
    {
        if (test_name != NULL)
        {
            color_from_parts_printf(DEFAULT | FOREGROUND_GREEN, "Test \"%s\" success\n", test_name);
        }
        return;
    }

    unsuccess_test_actions(test_name, ftext_error)
}

void assertNotEqual(void *first,
                    void *second,
                    char (*equal_function)(void *first, void *second),
                    char *test_name,
                    char *ftext_error,
                    ...)
{
    char result = equal_function(first, second);
    if (!result)
    {
        if (test_name != NULL)
        {
            color_from_parts_printf(DEFAULT | FOREGROUND_GREEN, "Test \"%s\" success\n", test_name);
        }
        return;
    }

    unsuccess_test_actions(test_name, ftext_error)
}

void assertEqual_log(void *first,
                     void *second,
                     char (*equal_function)(void *first, void *second),
                     char *test_name,
                     char *ftext_error,
                     ...)
{
    char result = equal_function(first, second);
    if (result)
    {
        if (test_name != NULL)
        {
            write_log(INFO, "Test \"%s\" success\n", test_name);
        }
        return;
    }

    unsuccess_test_actions(test_name, ftext_error)
}

void assertNotEqual_log(void *first,
                        void *second,
                        char (*equal_function)(void *first, void *second),
                        char *test_name,
                        char *ftext_error,
                        ...)
{
    char result = equal_function(first, second);
    if (!result)
    {
        if (test_name != NULL)
        {
            write_log(INFO, "Test \"%s\" success\n", test_name);
        }
        return;
    }

    unsuccess_test_actions(test_name, ftext_error)
}