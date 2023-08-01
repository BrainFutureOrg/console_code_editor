//
// Created by kosenko on 11.07.23.
//
#include "../prj_types/String_struct.h"
#include "../colors.h"
#include <stdio.h>
#include "../file_system/file_system_work.h"
#include <sys/ioctl.h>
#include "../prj_types/Array_type.h"

#ifndef CONSOLE_CODE_EDITOR_SEGMENTS_H
#define CONSOLE_CODE_EDITOR_SEGMENTS_H
#define terminal_goto(row, col)printf("\033[%d;%dH", row, col);//row, col > 0

//rectangle specified by rows and columns (>=0 each)
typedef struct urectangle
{
    uint row_start, row_end, col_start, col_end;
} urectangle;

typedef struct
{
    COLOR dir;
    COLOR file;
    COLOR default_color;
} filesystem_color_scheme;

struct move_readonly_params
{
    uint shift_row;
    uint shift_col;
    string str;
    urectangle screen_region;
    COLOR color;
};
void render_readonly_segment(void *args);
void start_moving_readonly_segment(string str,
                                   urectangle screen_region,
                                   void (*changer_function)(void *element, struct winsize w), COLOR color);

struct write_segment_params
{
    uint shift_row;
    uint shift_col;
    uint str_row;
    uint str_col;
    string *str;
    urectangle screen_region;
    COLOR color;
    COLOR current_line_color;
    char active;
    struct filesystem_segment_params *filesystem_segment_args;
    struct file_name_params *file_name_args;
    struct static_params *instruction_args;
};
void render_writeable_segment(void *args);
struct write_segment_params *start_write_segment(string *str,
                                                 urectangle screen_region,
                                                 void (*changer_function)(void *element, struct winsize w),
                                                 COLOR color, COLOR current_line_color);

struct directory_tree
{
    array_voidp dirs;
    string_array files;
    string name;
};
struct filesystem_segment_params
{
    urectangle screen_region;
    filesystem_color_scheme color_scheme;
    uint vertical_shift, horizontal_shift, cursor;
    char *prefix, print_name;
    file_system_anchor current_outermost_dir;
    struct directory_tree dir;
    char active;
    struct write_segment_params *write_segment_args;
    struct file_name_params *file_name_segment_args;
    struct static_params *instruction_args;
};
void render_filesystem_segment(void *args);
struct filesystem_segment_params *start_filesystem_segment(file_system_anchor anchor,
                                                           urectangle screen_region,
                                                           void (*changer_function)(void *element, struct winsize w),
                                                           filesystem_color_scheme color_scheme,
                                                           char *prefix);

struct static_params
{
    string str;
    urectangle screen_region;
    COLOR color;
};
void render_static_segment(void *args);
struct static_params *start_static_segment(string str,
                                           COLOR color,
                                           urectangle screen_region,
                                           void (*changer_function)(void *element, struct winsize w));

struct file_name_params
{
    string str;
    urectangle screen_region;
    file_system_anchor anchor;
    COLOR anchor_color;
    COLOR name_color;
    uint cursor;
    uint shift;
    char active;
    struct write_segment_params *write_args;
    struct static_params *instruction_args;
};
void render_file_name_segment(void *args);
struct file_name_params *start_file_name_segment(file_system_anchor anchor, string str,
                                                 COLOR color_anchor,
                                                 COLOR color_name,
                                                 urectangle screen_region,
                                                 void (*changer_function)(void *element, struct winsize w));

#endif //CONSOLE_CODE_EDITOR_SEGMENTS_H
