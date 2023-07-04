//
// Created by maximus on 04.07.23.
//

#include "test_funcs.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include "../colors.h"
#include "../prj_error.h"
//#define TEST_SUCCESS 0


#define TEST_print_formated_error(ftext_error) va_list args;                  \
                                               va_start(args, ftext_error);   \
                                               printf("%s", RED_COLOR);       \
                                               printf("ERROR text:");         \
                                               vprintf(ftext_error, args);    \
                                               printf("\n%s", DEFAULT_COLOR); \
                                               va_end(args);
#define unsuccess_test test_success = 0
#define unsuccess_test_actions(test_name, ftext_error) printf(RED_B_COLOR "Test \"%s\" failed\n" DEFAULT_COLOR, \
                                            test_name == NULL ? "Unnamed" : test_name); \
                                            unsuccess_test;                          \
                                                                                     \
                                            if (errno){                              \
                                                print_error();                       \
                                                errno = 0;                           \
                                            }                                        \
                                                                                     \
                                            TEST_print_formated_error(ftext_error)

char test_success = 1;
void print_test_success()
{
    if (test_success)
    {
        printf(GREEN_B_COLOR "All tests successful\n" DEFAULT_COLOR);
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
            printf(GREEN_COLOR "Test \"%s\" success\n" DEFAULT_COLOR, test_name);
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
            printf(GREEN_COLOR "Test \"%s\" success\n" DEFAULT_COLOR, test_name);
        }
        return;
    }

    unsuccess_test_actions(test_name, ftext_error)
}