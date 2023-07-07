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
    puts(color.line);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    color_to_default();
}

void color_from_parts_printf(COLOR_PARTS sum_of_parts, char *format, ...)
{
    color_to(sum_of_parts);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    color_to_default();
}

COLOR color_create_foreground_rgb(unsigned char r, unsigned char g, unsigned char b)
{
    // 38 - Set foreground color; Next arguments are 2;<r>;<g>;<b>, see below
    COLOR my_color = string_create_from_fcharp(ESCAPE_COLOR "[38;2;%d;%d;%dm", r, g, b);
    return my_color;
}
COLOR color_create_background_rgb(unsigned char r, unsigned char g, unsigned char b)
{
    // 48 - Set background color; Next arguments are 2;<r>;<g>;<b>, see below
    COLOR my_color = string_create_from_fcharp(ESCAPE_COLOR "[48;2;%d;%d;%dm", r, g, b);
    return my_color;
}

#define print_free_color(color) printf("%s", color.line); \
                                free_color(color)

void color_to(COLOR_PARTS sum_of_parts)
{
    COLOR color = create_color(sum_of_parts);
    print_free_color(color);
}

void color_to_rgb_foreground(unsigned char r, unsigned char g, unsigned char b)
{
    COLOR color = color_create_foreground_rgb(r, g, b);
    print_free_color(color);
}

void color_to_rgb_background(unsigned char r, unsigned char g, unsigned char b)
{
    COLOR color = color_create_background_rgb(r, g, b);
    print_free_color(color);
}

void color_to_default()
{
    color_to(DEFAULT);
}
void color_inverse()
{
    COLOR color = string_create_from_fcharp(ESCAPE_COLOR "[7m");
    print_free_color(color);
}