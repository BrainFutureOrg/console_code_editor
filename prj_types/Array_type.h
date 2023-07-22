//
// Created by maximus on 22.07.23.
//

#ifndef CONSOLE_CODE_EDITOR_PRJ_TYPES_ARRAY_TYPE_H
#define CONSOLE_CODE_EDITOR_PRJ_TYPES_ARRAY_TYPE_H

#include <sys/types.h>
typedef void *voidp;
typedef struct
{
    unsigned int size;
    unsigned int allocd_size;
    voidp *elements;
} array_voidp;

array_voidp array_voidp_create();
void array_voidp_add_elements_to_end(array_voidp *array, uint number, voidp *elements);
voidp array_voidp_get_element(array_voidp *array, uint number);
void array_voidp_delete_element(array_voidp *array, uint number);
void array_voidp_set_element(array_voidp *array, uint number, voidp element);
void array_voidp_push(array_voidp *array, voidp element);
voidp array_voidp_pop(array_voidp *array);
void free_string_array(array_voidp *array);

#endif //CONSOLE_CODE_EDITOR_PRJ_TYPES_ARRAY_TYPE_H
