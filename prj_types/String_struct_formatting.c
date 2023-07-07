//
// Created by maximus on 04.07.23.
//
#include <stdio.h>
#include "String_struct.h"

void reverse_charp(char *line)
{
    char *end = line + strlen(line) - 1;
    while (line < end)
    {
        char dop = *line;
        *line++ = *end;
        *end-- = dop;
    }
}

string int_to_string(int num)
{
    string result = string_create_new(5);
    if (num < 0)
    {
        string_add_char(&result, '-');
        num *= -1;
    }
    do
    {
        string_add_char(&result, (char)('0' + (num % 10)));
    }
    while ((num /= 10) != 0);
    reverse_charp(*result.line == '-' ? result.line + 1 : result.line);
    return result;
}
string double_to_string(double num, int fractional_symbol_num)
{
    string result = int_to_string((int)num);
    if (fractional_symbol_num > 0)
    {
        double fractional_part = num - (int)num;
        int fractional_int = (int)(fractional_part * pow(10, fractional_symbol_num));

        string_add_char(&result, '.');
        string fractional_string = int_to_string(fractional_int);
        string_add_string(&result, fractional_string);
        free_string(fractional_string);
    }
    return result;
}

void string_zfill(string *my_string, int size, char sign, char symbol)
{
    if (my_string->len >= size)
    {
        return;
    }
    uint len_add = size - my_string->len;
    string new_string = string_create_new(len_add + 1);
    while (len_add-- + 1 >= 1)
    {
        string_add_char(&new_string, symbol);
    }
    if (sign == 0)
    {
        string_add_string(&new_string, *my_string);
        free_string(*my_string);
        *my_string = new_string;
        return;
    }
    //else, sign == 1
    string_add_string(my_string, new_string);
    free_string(new_string);
}

string string_create_from_charp(char *charp)
{
    string new_string = string_create_new(strlen(charp) + 1);
    strcat(new_string.line, charp);
    return new_string;
}

#define str_to_int(int_var_name, fcharp)while (*++fcharp >= '0' && *fcharp <= '9'){\
int_var_name = int_var_name * 10 + *fcharp - '0';                       \
}

void format_format_symbol_detect(string *formatted, char **line_pointer, va_list va)
{
    char *fcharp = *line_pointer;
    int pre_decimal = 0, post_decimal = 0;
    char sign = *(fcharp + 1) == '-';//0 + 1 -
    if (sign)
    {
        fcharp++;
    }
    str_to_int(pre_decimal, fcharp)
    if (*fcharp == '.')
    {
        str_to_int(post_decimal, fcharp)
    }
    switch (*(fcharp))
    {
        case 'd':;
            string istr = int_to_string(va_arg(va, int));
            string_zfill(&istr, pre_decimal, sign, ' ');
            string_add_string(formatted, istr);
            free_string(istr);
            break;
        case 'f':;
            string dstr = double_to_string(va_arg(va, double), post_decimal);
            string_zfill(&dstr, pre_decimal + post_decimal + 1, sign, ' ');
            string_add_string(formatted, dstr);
            free_string(dstr);
            break;
        case 's':;
            string str = string_create_from_charp(va_arg(va, char *));
            string_zfill(&str, pre_decimal, sign, ' ');
            string_add_string(formatted, str);
            free_string(str);
            break;
        case 'c':
            string_add_char(formatted, va_arg(va, int));
            break;
        case '%':
            string_add_char(formatted, '%');
            break;
        default:
            errno = EINVAL;
            printf("error in char: '%c'", *fcharp);
    }
    *line_pointer = fcharp;
}

string string_create_from_fcharp(char *fcharp, ...)
{
    string formatted = string_create_new(strlen(fcharp) + 1);

    va_list va;
    va_start(va, fcharp);
    fcharp = fcharp;
    while (*fcharp != '\0')
    {
        if (*fcharp == '%')
        {
            format_format_symbol_detect(&formatted, &fcharp, va);
        }
        else
        {
            string_add_char(&formatted, *fcharp);
        }
        fcharp++;
    }
    va_end(va);
    return formatted;
}