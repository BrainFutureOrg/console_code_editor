//
// Created by kosenko on 11.07.23.
//
#include "../prj_types/String_struct.h"
#include "../colors.h"
#include <stdio.h>
#include "../file_system/file_system_work.h"
#include <sys/ioctl.h>

#ifndef CONSOLE_CODE_EDITOR_SEGMENTS_H
#define CONSOLE_CODE_EDITOR_SEGMENTS_H
#define terminal_goto(row, col)printf("\033[%d;%dH", row, col);//row, col > 0

//rectangle specified by rows and columns (>=0 each)
typedef struct urectangle
{
    uint row_start, row_end, col_start, col_end;
} urectangle;
void render_readonly_segment(void *args);
void start_moving_readonly_segment(string str,
                                   urectangle screen_region,
                                   void (*changer_function)(void *element, struct winsize w), COLOR color);

void render_writeable_segment(void *args);
void start_write_segment(string *str, urectangle screen_region,
                         void (*changer_function)(void *element, struct winsize w), COLOR color);

void render_filesystem_segment(void *args);
void start_filesystem_segment(file_system_anchor anchor,
                              urectangle screen_region,
                              void (*changer_function)(void *element, struct winsize w));

void render_static_segment(void *args);
void start_static_segment(string str,
                          COLOR color,
                          urectangle screen_region,
                          void (*changer_function)(void *element, struct winsize w));

typedef struct filesystem_color_scheme
{
    COLOR dir;
    COLOR file;
    COLOR default_color;
};

#endif //CONSOLE_CODE_EDITOR_SEGMENTS_H
