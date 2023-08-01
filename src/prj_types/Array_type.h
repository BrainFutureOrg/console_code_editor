//
// Created by maximus on 22.07.23.
//

#ifndef CONSOLE_CODE_EDITOR_PRJ_TYPES_ARRAY_TYPE_H
#define CONSOLE_CODE_EDITOR_PRJ_TYPES_ARRAY_TYPE_H

#include <sys/types.h>
/* typedef for developer easier code understanding */
typedef void *voidp;

typedef struct
{
    unsigned int size;
    unsigned int allocd_size;
    void
    (*free_elements_method)(voidp);
    voidp *elements;
} array_voidp;

/*
 * Function to initialize array of void pointers (any pointer type)
 */
array_voidp array_voidp_create(void (*free_elements_method)(voidp));

/*
 * Function that add {number} elements to the end of array
 */
void array_voidp_add_elements_to_end(array_voidp *array, uint number, voidp *elements);

/*
 * Function to get {number} element (counting from 0)
 */
voidp array_voidp_get_element(array_voidp *array, uint number);

/*
 * Function to delete {number} element (counting from 0)
 */
void array_voidp_delete_element(array_voidp *array, uint number);

/*
 * Function to set {number} element instead of the previous (counting from 0)
 */
void array_voidp_set_element(array_voidp *array, uint number, voidp element);

/*
 * Function to push element to the array (add to the end)
 */
void array_voidp_push(array_voidp *array, voidp element);

/*
 * Function to pop element from the array (get the end element and delete it from the array)
 */
voidp array_voidp_pop(array_voidp *array);

/*
 * Function to free all memory allocated for the array
 */
void free_array_voidp(array_voidp *array);

//insert element into voidp array at place specified by number
void array_voidp_insert(array_voidp *array, voidp element, uint number);

#endif //CONSOLE_CODE_EDITOR_PRJ_TYPES_ARRAY_TYPE_H
