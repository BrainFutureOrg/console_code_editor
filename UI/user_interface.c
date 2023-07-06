//
// Created by maximus on 06.07.23.
//

#include <stdio.h>
#include "user_interface.h"
#include "../colors.h"

void print_logo()
{
    COLOR foreground_logo = color_create_foreground_rgb(200, 150, 50);
    COLOR background_logo = color_create_background_rgb(0, 0, 0);
    printf("%s", background_logo.line);
    printf("%s", foreground_logo.line);
    free_color(foreground_logo);
    free_color(background_logo);

    printf("C Code ");
    color_inverse();
    printf("Editor\n");
    color_to_default();
}