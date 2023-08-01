//
// Created by maximus on 22.07.23.
//

#include <stdlib.h>
#include <errno.h>
#include "Array_type.h"

array_voidp array_voidp_create(void (*free_elements_method)(voidp))
{
    uint start_alloc_size = 10;
    array_voidp result;
    result.size = 0;
    result.allocd_size = start_alloc_size;
    result.elements = calloc(result.allocd_size, sizeof(voidp));
    result.free_elements_method = free_elements_method;
    return result;
}

void array_voidp_add_elements_to_end(array_voidp *array, uint number, voidp *elements)
{
    while (number-- > 0)
    {
        array_voidp_push(array, *(elements++));
    }
}

voidp array_voidp_get_element(array_voidp *array, uint number)
{
    if (number < array->size)
    {
        return array->elements[number];
    }
    errno = E2BIG;
    return NULL;
}

void array_voidp_delete_element(array_voidp *array, uint number)
{
    if (number >= array->size)
    {
        errno = E2BIG;
        return;
    }
    voidp *deleted = array->elements + number;
    array->free_elements_method(*deleted);
    while (deleted - array->elements < array->size)
    {
        *deleted = *(deleted + 1);
        deleted++;
    }
    array->size--;
}
void array_voidp_set_element(array_voidp *array, uint number, voidp element)
{
    if (number >= array->size)
    {
        errno = E2BIG;
        return;
    }
    array->free_elements_method(*(array->elements + number));
    *(array->elements + number) = element;
}

void array_voidp_increase_len(array_voidp *my_voidp_array, uint increasing_number)
{
    my_voidp_array->allocd_size += increasing_number;
    my_voidp_array->elements =
        (voidp *)reallocarray(my_voidp_array->elements, my_voidp_array->allocd_size, sizeof(voidp));
    if (my_voidp_array->elements == NULL)
    {
        errno = E2BIG; // Memory error
    }
}

#define doubling_array_voidp_len(voidp_arr) array_voidp_increase_len(voidp_arr, voidp_arr->allocd_size == 0 ? 1 : voidp_arr->allocd_size)

void array_voidp_push(array_voidp *array, voidp element)
{
    while (array->size >= array->allocd_size)
    {
        doubling_array_voidp_len(array);
    }
    array->elements[array->size++] = element;
}

voidp array_voidp_pop(array_voidp *array)
{
    if (array->size == 0)
    {
        errno = E2BIG;
        return NULL;
    }
    return array->elements[--array->size];
}

void free_array_voidp(array_voidp *array)
{
    voidp *array_p = array->elements;
    for (int i = 0; i < array->size; i++)
    {
        array->free_elements_method(*array_p++);
    }
    free(array->elements);
    array->size = array->allocd_size = 0;
}

void array_voidp_insert(array_voidp *array, voidp element, uint number)
{
    if (number > array->size)
    {
        errno = E2BIG;
        return;
    }
    if (array->size >= array->allocd_size)
    {
        doubling_array_voidp_len(array);
    }
    voidp *elements_current = array->elements + array->size;
    while (elements_current - array->elements > number)
    {
        *elements_current = *(elements_current - 1);
        elements_current--;
    }
    *elements_current = element;
    array->size++;
}