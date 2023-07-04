//
// Created by maximus on 03.07.23.
//

#ifndef CONSOLE_CODE_EDITOR__STRING_STRUCT_H
#define CONSOLE_CODE_EDITOR__STRING_STRUCT_H

#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef struct
{
    char *line;
    unsigned int len;
} string;

string string_create_new(uint len);
string string_create_from_fcharp(char *fcharp, ...);

void string_add_char(string *my_string, char symbol);
void string_add_string(string *my_string1, string my_string2);
void string_add_charp(string *my_string, char *charp);

void free_string(string my_string);

char string_equals(string string1, string string2);
char string_charp_equals(string str, char *charp);

string int_to_string(int num);
string double_to_string(double num, int fractional_symbol_num);
void string_zfill(string *my_string, int size, char sign, char symbol);
#endif //CONSOLE_CODE_EDITOR__STRING_STRUCT_H
