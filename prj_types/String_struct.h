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

typedef struct
{
    string string_part;
    unsigned int last_element;
} string_fast;

typedef struct
{
    int size;
    int allocd_size;
    string *elements;
} string_array;

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

string_fast string_fast_create_new(uint len);
string_fast string_fast_create_from_string(string initial);

void string_fast_add_char(string_fast *my_string, char symbol);
void string_fast_add_string(string_fast *my_string1, string my_string2);
void string_fast_add_charp(string_fast *my_string, char *charp);

void free_string_fast(string_fast my_string);

string_array string_array_create();
void string_array_add_elements_to_end(string_array *array, int number, void **elements);
void *string_array_get_element(string_array *array, int number);
void string_array_delete_element(string_array *array, int number);
void string_array_set_element(string_array *array, int number, void *element);
void string_array_push(string_array *array, void *element);
void *string_array_pop(string_array *array);
void free_string_array(string_array *array);

#endif //CONSOLE_CODE_EDITOR__STRING_STRUCT_H
