//
// Created by maximus on 06.07.23.
//

#include <stdio.h>
#include "user_interface.h"
#include "../colors.h"

void print_logo()
{
    color_to_rgb_foreground(200, 150, 50);
    color_to_rgb_background(0, 0, 0);

    printf("C Code ");
    color_inverse();
    printf("Editor\n");
    color_to_default();
}