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

string_fast string_fast_create_new(uint len)
{
    string new = string_create_new(len);
    return (string_fast){new, 0};
}

string_fast string_fast_create_from_string(string initial)
{
    string_fast result = {string_copy(initial), 0};
    result.last_element = strlen(initial.line);
    return result;
}

void string_fast_add_char(string_fast *my_string, char symbol)
{
//    char is_empty = *my_string->string_part.line == '\0';
    while (my_string->last_element + 1 >= my_string->string_part.len)
    {
        string_increase_len(&my_string->string_part, my_string->string_part.len);
    }
    my_string->string_part.line[my_string->last_element++] = symbol;
    my_string->string_part.line[my_string->last_element] = '\0';
}
void string_fast_add_string(string_fast *my_string1, string my_string2)
{
    string_fast_add_charp(my_string1, my_string2.line);
}

void string_fast_add_charp(string_fast *my_string, char *charp)
{
    get_end_pointer_with_create(str2, charp);
    uint len2 = str2 - charp;
    uint full_len = my_string->last_element + len2 + 1;
    if (full_len > my_string->string_part.len)
    {
        string_increase_len(&my_string->string_part, full_len - my_string->string_part.len);
    }
    char *str1 = my_string->string_part.line + my_string->last_element;
    while ('\0' != (*str1++ = *charp++));
}

void free_string_fast(string_fast my_string)
{
    free_string(my_string.string_part);
}

void insert_into_string_multiline(string *insert_into, char c, uint row, uint col)
{
    //uint current_row = 0, current_col = 0;
    char *current_char = insert_into->line;
    while (row)
        row -= *current_char++ == '\n';

    while (col--)
        current_char++;
    char temp;
    while (*current_char != '\0')
    {
        temp = *current_char;
        *current_char = c;
        c = temp;
        current_char++;
    }
    string_add_char(insert_into, c);
}

void delete_from_string_multiline(string *delete_from, uint row, uint col)
{
    char *current_char = delete_from->line;
    while (row)
        row -= *current_char++ == '\n';
    while (col--)
        current_char++;
    while (*current_char != '\0')
    {
        *current_char = *(current_char + 1);
        current_char++;
    }
}

string string_copy(string old)
{
    string new = string_create_new(old.len);
    char *str_new_charp = new.line;
    while ((*str_new_charp++ = *old.line++) != '\0');
    return new;
}