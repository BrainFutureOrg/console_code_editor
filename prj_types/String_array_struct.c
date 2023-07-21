//
// Created by maximus on 20.07.23.
//

#include <stdio.h>
#include "String_struct.h"

string_array string_array_create()
{
    uint start_alloc_size = 10;
    return (string_array){0, start_alloc_size, calloc(start_alloc_size, sizeof(string))};
}

void string_array_add_elements_to_end(string_array *array, uint number, string *elements)
{
    while (number-- > 0)
    {
        string_array_push(array, *(elements++));
    }
}
string string_array_get_element(string_array *array, uint number)
{
    if (number < array->size)
    {
        return string_copy(array->elements[number]);
    }
    errno = E2BIG;
    return string_create_new(1);
}

void string_array_delete_element(string_array *array, uint number)
{
    if (number >= array->size)
    {
        errno = E2BIG;
        return;
    }
    string *deleted = array->elements + number;
    free_string(*deleted);
    while (deleted - array->elements < array->size)
    {
        *deleted = *(deleted + 1);
        deleted++;
    }
    array->size--;
}

void string_array_set_element(string_array *array, uint number, string element)
{
    if (number >= array->size)
    {
        errno = E2BIG;
        return;
    }
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
    if (array->size == 0)
    {
        errno = E2BIG;
        return string_create_new(1);
    }
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

void string_array_push_charp(string_array *array, char *element)
{
    string new = string_create_from_fcharp(element);
    string_array_push(array, new);
    free_string(new);
}

string_array string_split(char *my_string, char (*split_func)(char *))
{
    string_array result = string_array_create();
    char *end_pointer = my_string;
    int num_of_shift;
    while (*end_pointer != '\0')
    {
        num_of_shift = split_func(end_pointer);
        if (num_of_shift)
        {
            if (end_pointer != my_string)
            {
                string_fast new = string_fast_create_new(end_pointer - my_string + 1);
                while (end_pointer > my_string)
                {
                    string_fast_add_char(&new, *my_string++);
                }

                string_array_push(&result, new.string_part);
                free_string_fast(new);
            }
            end_pointer += num_of_shift;
            my_string = end_pointer;
            continue;
        }
        end_pointer++;
    }
    string_array_push_charp(&result, my_string);
    return result;
}
char string_split_is_space(char *charp)
{
    return *charp == ' ';
}

void string_array_print_type_1(string_array *array)
{
    for (int i = 0; i < array->size; i++)
    {
        printf("%s ", string_array_get_element(array, i).line);
    }
    putchar('\n');
}
void string_array_print_type_2(string_array *array)
{
    for (int i = 0; i < array->size; i++)
    {
        if (i != 0)
        {
            printf(", ");
        }
        printf("\"%s\"", string_array_get_element(array, i).line);
    }
    putchar('\n');
}
void string_array_print_type_3(string_array *array)
{
    for (int i = 0; i < array->size; i++)
    {
        printf("element %d: %s\n", i + 1, string_array_get_element(array, i).line);
    }
}

void string_array_print(string_array *array, char type)
{
    switch (type)
    {
        case 1:
            string_array_print_type_1(array);
            break;
        case 2:
            string_array_print_type_2(array);
            break;
        case 3:
            string_array_print_type_3(array);
            break;
        default:
            errno = EKEYEXPIRED;
            break;
    }
}