//
// Created by maximus on 20.07.23.
//

#include <stdio.h>
#include "String_struct.h"

string_array string_array_create()
{
    return (string_array){0, 0, NULL};
}

void string_array_add_elements_to_end(string_array *array, int number, string *elements)
{
    while (number-- > 0)
    {
        string_array_push(array, *(elements++));
    }
}
string string_array_get_element(string_array *array, int number)
{
    if (number < array->size)
    {
        return string_copy(array->elements[number]);
    }
    errno = E2BIG;
    return string_create_new(1);
}

void string_array_delete_element(string_array *array, int number)
{
    string *deleted = array->elements + number;
    free_string(*deleted);
    while (deleted - array->elements < array->size)
    {
        *deleted = *(deleted + 1);
        deleted++;
    }
    array->size--;
}

void string_array_set_element(string_array *array, int number, string element)
{
    *(array->elements + number) = string_copy(element);
}

void string_array_increase_len(string_array *my_string_array, uint increasing_number)
{
    my_string_array->allocd_size += increasing_number;
    my_string_array->elements =
        (string *)reallocarray(my_string_array->elements, my_string_array->allocd_size, sizeof(string));
    if (my_string_array->elements == NULL)
    {
        errno = E2BIG; // Memory error
    }
}

#define doubling_string_array_len(str_arr) string_array_increase_len(str_arr, str_arr->allocd_size == 0 ? 1 : str_arr->allocd_size)

void string_array_push(string_array *array, string element)
{
    while (array->size >= array->allocd_size)
    {
        doubling_string_array_len(array);
    }
    array->elements[array->size++] = string_copy(element);
}
string string_array_pop(string_array *array)
{
    return array->elements[--array->size];
}
void free_string_array(string_array *array)
{
    string *array_p = array->elements;
    for (int i = 0; i < array->size; i++)
    {
        free_string(*array_p++);
    }
    free(array->elements);
    array->size = array->allocd_size = 0;
}