//
// Created by kosenko on 11.07.23.
//
#include "../prj_types/String_struct.h"
#include "../colors.h"
#include <stdio.h>

#ifndef CONSOLE_CODE_EDITOR_SEGMENTS_H
#define CONSOLE_CODE_EDITOR_SEGMENTS_H
#define terminal_goto(row, col)printf("\033[%d;%dH", row, col);//row, col > 0

//rectangle specified by rows and columns (>=0 each)
typedef struct urectangle
{
    uint row_start, row_end, col_start, col_end;
} urectangle;
void print_string_segment_primitive(string str, COLOR inner_color, urectangle screen_region);
void start_moving_readonly_segment(string str, urectangle screen_region);
void start_write_segment(string *str, urectangle screen_region);
void print_segment_plaintext_shifted(char *text, urectangle screen_region, uint text_start_col);

#endif //CONSOLE_CODE_EDITOR_SEGMENTS_H
