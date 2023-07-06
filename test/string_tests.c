//
// Created by maximus on 04.07.23.
//
#include "../prj_types/String_struct.h"
//#include "../prj_error.h"
#include "test_funcs.h"
#include <stdio.h>
#include "string_tests.h"
#include "../colors.h"
#define assert_equality_string(s1, s2, test_name) assertEqual(&s1, &s2,\
    string_pointer_equals, test_name, "strings: \"%s\" \"%s\"", s1.line, s2.line)
#define assert_equality_string_charp(s1, c2, test_name) assertEqual(&s1, c2,\
    string_pointer_char_pointer_equals, test_name, "strings: \"%s\" \"%s\"", s1.line, c2)
#define assert_non_equality_string(s1, s2, test_name) assertNotEqual(&s1, &s2,\
    string_pointer_equals, test_name, "strings: \"%s\" \"%s\"", s1.line, s2.line)
#define assert_non_equality_string_charp(s1, s2, test_name) assertNotEqual(&s1, &s2,\
    string_pointer_char_pointer_equals, test_name, "strings: \"%s\" \"%s\"", s1.line, s2.line)

char string_pointer_equals(void *str1, void *str2)
{
    return string_equals(*((string *)str1), *((string *)str2));
}
char string_pointer_char_pointer_equals(void *str, void *charp)
{
    return string_charp_equals(*((string *)str), charp);
}

void str_equality_test()
{
    string s1 = string_create_from_fcharp("abcde"), s2 = string_create_from_fcharp("abcde");
    assert_equality_string(s1, s2, "simple_equality_test");
    free_string(s1);
    free_string(s2);
}

void str_charp_equality_test()
{
    string s = string_create_from_fcharp("abcde");
    assert_equality_string_charp(s, "abcde", "string and char pointer equality test");
    free_string(s);
}

void str_non_equality_test()
{
    string s1 = string_create_from_fcharp("Hello"), s2 = string_create_from_fcharp("Hell"),
        s3 = string_create_from_fcharp("Hello "), s4 =
        string_create_from_fcharp("");
    assert_non_equality_string(s1, s2, "simple non equaility");
    assert_non_equality_string(s1, s3, NULL);
    assert_non_equality_string(s1, s4, NULL);
    assert_non_equality_string(s4, s1, NULL);
    free_string(s1);
    free_string(s2);
    free_string(s3);
    free_string(s4);
}

void str_char_add_test()
{
    string s = string_create_from_fcharp("abcd");

    string_add_char(&s, 'e');
    assert_equality_string_charp(s, "abcde", "adding character to string");
    free_string(s);
}
void str_str_add_test()
{
    string s = string_create_from_fcharp("abcd"), s_e = string_create_from_fcharp("e");
    string_add_string(&s, s_e);
    assert_equality_string_charp(s, "abcde", "adding string to string");
    free_string(s);
    free_string(s_e);
}
void str_charp_add_test()
{
    string s = string_create_from_fcharp("abc");
    string_add_charp(&s, "de");
    assert_equality_string_charp(s, "abcde", "adding character array to string");
    free_string(s);
}

void str_format_test()
{
    string s = string_create_from_fcharp("%s, %d, %.3f, %% %c", "Hello, world?", 20, 3.1415926535, 'p');
    assert_equality_string_charp(s, "Hello, world?, 20, 3.141, % p", "formatted strings: all types");
    free_string(s);
}

void str_format_test_zfill()
{
    string s1 = string_create_from_fcharp("%10s", "Hell");
    assert_equality_string_charp(s1, "      Hell", "formatted string: zfill");
    free_string(s1);

    s1 = string_create_from_fcharp("%-10s", "Hell");
    assert_equality_string_charp(s1, "Hell      ", NULL);
    free_string(s1);

    s1 = string_create_from_fcharp("%-1s", "Hell");
    assert_equality_string_charp(s1, "Hell", NULL);
    free_string(s1);
}

void apply_string_tests()
{
    color_from_parts_printf(DEFAULT | BOLD | FOREGROUND_CYAN, "STRING TESTS START\n");
    str_equality_test();
    str_charp_equality_test();
    str_non_equality_test();
    str_char_add_test();
    str_str_add_test();
    str_charp_add_test();
    str_format_test();
    str_format_test_zfill();
    color_from_parts_printf(DEFAULT | BOLD | FOREGROUND_CYAN, "STRING TESTS END\n");
}