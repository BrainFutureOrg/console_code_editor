//
// Created by maximus on 04.07.23.
//

#ifndef CONSOLE_CODE_EDITOR_TEST_TEST_FUNCS_H
#define CONSOLE_CODE_EDITOR_TEST_TEST_FUNCS_H
//char test_success;
void assertEqual(void *first,
                 void *second,
                 char (*equal_function)(void *first, void *second),
                 char *test_name,
                 char *text_error_formated,
                 ...);
void assertNotEqual(void *first,
                    void *second,
                    char (*equal_function)(void *first, void *second),
                    char *test_name,
                    char *text_error_formated,
                    ...);
void print_test_success();

#endif //CONSOLE_CODE_EDITOR_TEST_TEST_FUNCS_H
