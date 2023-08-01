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
    unsigned int size;
    unsigned int allocd_size;
    string *elements;
} string_array;

/*
 * Initialize empty string with {len} allocated memory
 */
string string_create_new(uint len);

/*
 * Initialize string from formatted message (like "%d + %d = %d", 2, 2, 4)
 * Realized most printf format
 */
string string_create_from_fcharp(char *fcharp, ...);

/*
 * Makes copy of another string
 */
string string_copy(string old);

/*
 * Add char to the end of string function (slow)
 */
void string_add_char(string *my_string, char symbol);

/*
 * Add string to the another string (result saving to the {my_string1})
 */
void string_add_string(string *my_string1, string my_string2);

/*
 * Add char pointer to the string (result saving to the {my_string})
 */
void string_add_charp(string *my_string, char *charp);

/*
 * Free memory allocated to the {my_string}
 */
void free_string(string my_string);

/*
 * Return result of comparing two strings (only lines, without size)
 */
char string_equals(string string1, string string2);

/*
 * Return result of comparing string with char pointer (only lines)
 */
char string_charp_equals(string str, char *charp);

/*
 * Convert int to string
 */
string int_to_string(int num);

/*
 * Convert double to string
 */
string double_to_string(double num, int fractional_symbol_num);

/*
 * Copy of zfill python function
 * arguments: my_string - string for operation
 *            size      - the minimum size of the line
 *            sign      - place where {symbol} should paste
 *                        0     - to the start of the line
 *                        other - to the end of the line
 *            symbol    - symbol for fill line to the {size}
 * result is writing to the {my_string}
 * example:
 *            string s1 = string_create_from_fcharp("Hello");
 *            string_zfill(s1, 10, 0, ' ');
 *            s1.line; // "     Hello"
 */
void string_zfill(string *my_string, int size, char sign, char symbol);

string_fast string_fast_create_new(uint len);
string_fast string_fast_create_from_string(string initial);

void string_fast_add_char(string_fast *my_string, char symbol);
void string_fast_add_string(string_fast *my_string1, string my_string2);
void string_fast_add_charp(string_fast *my_string, char *charp);

void free_string_fast(string_fast my_string);

string_array string_array_create();
void string_array_add_elements_to_end(string_array *array, uint number, string *elements);
string string_array_get_element(string_array *array, uint number);
void string_array_delete_element(string_array *array, uint number);
void string_array_set_element(string_array *array, uint number, string element);
void string_array_push(string_array *array, string element);
void string_array_push_charp(string_array *array, char *element);
string string_array_pop(string_array *array);
/* Function to print string array
 * arguments: array - array for print
 *            type - 1 - in one line with space separation
 *                   2 - with "" and separate with ,
 *                   3 - every element in new line with their number*/
void string_array_print(string_array *array, char type);
char string_array_is_charp_in(string_array *array, char *element);
void free_string_array(string_array *array);

/*
 * insert character into string, string is viewed as a text with lines ended with '\n'
 */
void insert_into_string_multiline(string *insert_into, char c, uint row, uint col);
/*
 * delete character from string, string is viewed as a text with lines ended with '\n'
 */
void delete_from_string_multiline(string *delete_from, uint row, uint col);

/*
 * Split function
 * arguments: my_string - pointer to the start of line
 *            split_func - function that return number of symbols that fit else 0
 * return: array of split strings
 */
string_array string_split(char *my_string, char (*split_func)(char *));

char string_split_is_space(char *charp);
#endif //CONSOLE_CODE_EDITOR__STRING_STRUCT_H
