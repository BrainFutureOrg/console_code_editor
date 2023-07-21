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
#define assert_equality_int(i1, i2, test_name) assertEqual(&i1, &i2,\
    int_pointer_equals, test_name, "ints: \"%d\" \"%d\"", i1, i2)

char int_pointer_equals(void *i1, void *i2)
{
    return (char)(*((int *)i1) == *((int *)i2));
}
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
void str_fast_str_add_test()
{
    string_fast s = string_fast_create_from_string(string_create_from_fcharp("abcd"));
    string s_e = string_create_from_fcharp("e");
    string_fast_add_string(&s, s_e);
    assert_equality_string_charp(s.string_part, "abcde", "adding string to string_fast");
    free_string_fast(s);
    free_string(s_e);
}
void str_fast_charp_add_test()
{
    string_fast s = string_fast_create_from_string(string_create_from_fcharp("abc"));
    string_fast_add_charp(&s, "de");
    assert_equality_string_charp(s.string_part, "abcde", "adding character array to string_fast");
    free_string_fast(s);
}

void str_fast_char_add_test()
{
    string_fast s = string_fast_create_from_string(string_create_from_fcharp("abcd"));

    string_fast_add_char(&s, 'e');
    assert_equality_string_charp(s.string_part, "abcde", "adding character to string_fast");
    free_string_fast(s);
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

void str_array_test()
{
    string_array arr = string_array_create();
    string to_add = string_create_from_fcharp("Something");
    int num = 10;
    for (int I = 0; I < num; ++I)
    {
        string_array_push(&arr, to_add);
    }
    string got_element = string_array_get_element(&arr, 1);
    assert_equality_string(got_element, to_add, "string array: add element");
    assert_equality_int(arr.size, num, "string array: change size");
    free_string(got_element);
    free_string(to_add);
    free_string_array(&arr);
}

void str_split_test()
{
    char *line = "Hello it`s me";
//    int num_of_splited = 3;
    string_array right_result = string_array_create();
    string_array_push_charp(&right_result, "Hello");
    string_array_push_charp(&right_result, "it`s");
    string_array_push_charp(&right_result, "me");

    string_array splited = string_split(line, string_split_is_space);

    assert_equality_int(splited.size, right_result.size, "string split: split_number");
    for (int i = 0; i < right_result.size; i++)
    {
        string got_element = string_array_get_element(&splited, i);
        string got_element1 = string_array_get_element(&right_result, i);
        string error_text = string_create_from_fcharp("string split: equality element %d", i);
        assert_equality_string(got_element, got_element1, error_text.line);
        free_string(error_text);
        free_string(got_element);
        free_string(got_element1);
    }
    free_string_array(&right_result);
    free_string_array(&splited);
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

    str_fast_char_add_test();
    str_fast_str_add_test();
    str_fast_charp_add_test();

    str_format_test();
    str_format_test_zfill();
    str_array_test();
    str_split_test();
    color_from_parts_printf(DEFAULT | BOLD | FOREGROUND_CYAN, "STRING TESTS END\n");
}