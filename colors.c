//
// Created by maximus on 04.07.23.
//

#include "colors.h"
#include <stdio.h>
#define ESCAPE_COLOR "\033"

COLOR create_color(COLOR_PARTS sum_of_parts)
{
    COLOR my_color = string_create_from_fcharp(ESCAPE_COLOR "[");
    if (sum_of_parts & DEFAULT)
    {
        string_add_charp(&my_color, "0;");
    }
    if (sum_of_parts & BOLD)
    {
        string_add_charp(&my_color, "1;");
    }
    if (sum_of_parts & UNDERLINED)
    {
        string_add_charp(&my_color, "4;");
    }
    if (sum_of_parts & FLASHING)
    {
        string_add_charp(&my_color, "5;");
    }
    if (sum_of_parts & FOREGROUND_BLACK)
    {
        string_add_charp(&my_color, "30;");
    }
    if (sum_of_parts & FOREGROUND_RED)
    {
        string_add_charp(&my_color, "31;");
    }
    if (sum_of_parts & FOREGROUND_GREEN)
    {
        string_add_charp(&my_color, "32;");
    }
    if (sum_of_parts & FOREGROUND_YELLOW)
    {
        string_add_charp(&my_color, "33;");
    }
    if (sum_of_parts & FOREGROUND_BLUE)
    {
        string_add_charp(&my_color, "34;");
    }
    if (sum_of_parts & FOREGROUND_PURPLE)
    {
        string_add_charp(&my_color, "35;");
    }
    if (sum_of_parts & FOREGROUND_CYAN)
    {
        string_add_charp(&my_color, "36;");
    }
    if (sum_of_parts & FOREGROUND_WHITE)
    {
        string_add_charp(&my_color, "37;");
    }
    if (sum_of_parts & BACKGROUND_BLACK)
    {
        string_add_charp(&my_color, "40;");
    }
    if (sum_of_parts & BACKGROUND_RED)
    {
        string_add_charp(&my_color, "41;");
    }
    if (sum_of_parts & BACKGROUND_GREEN)
    {
        string_add_charp(&my_color, "42;");
    }
    if (sum_of_parts & BACKGROUND_YELLOW)
    {
        string_add_charp(&my_color, "43;");
    }
    if (sum_of_parts & BACKGROUND_BLUE)
    {
        string_add_charp(&my_color, "44;");
    }
    if (sum_of_parts & BACKGROUND_PURPLE)
    {
        string_add_charp(&my_color, "45;");
    }
    if (sum_of_parts & BACKGROUND_CYAN)
    {
        string_add_charp(&my_color, "46;");
    }
    if (sum_of_parts & BACKGROUND_WHITE)
    {
        string_add_charp(&my_color, "47;");
    }
    uint len = strlen(my_color.line);
    my_color.line[len - 1] = 'm';
    return my_color;
}

void color_printf(COLOR color, char *format, ...)
{
    printf("%s", color.line);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    COLOR default_color = create_color(DEFAULT);
    printf("%s", default_color.line);
    free_string(default_color);
}

void color_from_parts_printf(COLOR_PARTS sum_of_parts, char *format, ...)
{
    COLOR color = create_color(sum_of_parts);

    printf("%s", color.line);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    COLOR default_color = create_color(DEFAULT);
    printf("%s", default_color.line);
    free_string(default_color);
    free_string(color);
}
