//
// Created by maximus on 03.07.23.
//
#include "String_struct.h"

string string_create_new(uint len)
{
    string result;
    result.len = len;
    result.line = calloc(len, sizeof(char));
    *result.line = '\0';
    return result;
}

void string_increase_len(string *my_string, uint increasing_number)
{
    my_string->len += increasing_number;
    my_string->line = (char *)reallocarray(my_string->line, my_string->len, sizeof(char));
    if (my_string->line == NULL)
    {
        errno = E2BIG; // Memory error
    }

}

#define doubling_len(str) string_increase_len(str, str->len)

void string_add_char(string *my_string, char symbol)
{
    uint len_str = strlen(my_string->line) + 1;
    while (len_str >= my_string->len)
    {
        doubling_len(my_string);
    }
    my_string->line[len_str - 1] = symbol;
    my_string->line[len_str] = '\0';
}

void string_add_string(string *my_string1, string my_string2)
{
    string_add_charp(my_string1, my_string2.line);
}

#define get_end_pointer(name, str) name = str;\
                                   while(*name++ != '\0'); \
                                   name--

#define get_end_pointer_with_create(name, str) char* name;\
                                               get_end_pointer(name, str)

void string_add_charp(string *my_string, char *charp)
{
    get_end_pointer_with_create(str1, my_string->line);
    get_end_pointer_with_create(str2, charp);
    uint len1 = str1 - my_string->line;
    uint len2 = str2 - charp;
    uint full_len = len1 + len2 + 1;
    if (full_len > my_string->len)
    {
        string_increase_len(my_string, full_len - my_string->len);
    }
    str1 = my_string->line + len1;
    while ('\0' != (*str1++ = *charp++));
}

void free_string(string my_string)
{
    free(my_string.line);
}

char string_equals(string string1, string string2)
{
    char *str1 = string1.line, *str2 = string2.line;
    while (str1 - string1.line < string1.len && str2 - string2.line < string2.len && *str1 == *str2++)
    {
        if (*str1++ == '\0')
            return 1;
    }
    return 0;
}

char string_charp_equals(string str, char *charp)
{
    char *str1 = str.line, *str2 = charp;
    while (str1 - str.line < str.len && *str1++ == *str2)
    {
        if (*str2++ == '\0')
            return 1;
    }
    return 0;
}