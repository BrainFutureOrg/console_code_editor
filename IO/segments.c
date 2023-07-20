//
// Created by kosenko on 11.07.23.
//

#include "segments.h"
#include "terminal_io.h"
void print_string_segment_primitive(string str, COLOR inner_color, urectangle screen_region)
{
    terminal_goto(screen_region.row_start, screen_region.col_start)
    printf("%s", ((string)inner_color).line);
    for (uint row = screen_region.row_start; row < screen_region.row_end; row++)
    {
        for (uint col = screen_region.col_start; col < screen_region.col_end; col++)
        {
            if (*str.line != '\0')
            {
                if (*str.line == '\n')
                {
                    while (col++ < screen_region.col_end)
                        printf(" ");
                }
                else
                {
                    if (*str.line == '#')
                        color_to_rgb_foreground(255, 255, 0);
                    if (*str.line == '{' || *str.line == '}')
                        color_to_rgb_foreground(0, 255, 0);
                    printf("%c", *str.line);
                    str.line++;
                }
            }
            else
            {
                printf(" ");
            }
        }
        if (*str.line == '\n')
        {
            str.line++;
        }
        else
        {
            while (*str.line != '\0' && *str.line++ != '\n');
        }
        color_to_rgb_foreground(255, 255, 255);
        terminal_goto(row + 1, screen_region.col_start);
    }
    color_to_default();
}
void print_segment_plaintext_shifted(char *text, urectangle screen_region, uint text_start_col)
{
    //printf("AAA");
    terminal_goto(screen_region.row_start, screen_region.col_start)
    for (uint row = screen_region.row_start; row < screen_region.row_end; row++)
    {
        for (int i = 0; i < text_start_col; i++)
        {
            if (*text == '\0' || *text == '\n')
                break;
            else
                text++;
        }
        for (uint col = screen_region.col_start; col < screen_region.col_end; col++)
        {
            if (*text != '\0')
            {
                if (*text == '\n')
                {
                    while (col++ < screen_region.col_end)
                        printf(" ");
                }
                else
                {
                    printf("%c", *text);
                    text++;
                }
            }
            else
            {
                printf(" ");
            }
        }
        if (*text == '\n')
        {
            text++;
        }
        else
        {
            while (*text != '\0' && *text++ != '\n');
        }
        terminal_goto(row + 1, screen_region.col_start);
    }
}
struct move_readonly_params
{
    uint shift_row;
    uint shift_col;
    string str;
    urectangle screen_region;
};
void move_arrow(ARROW arrow, void *args)
{
    struct move_readonly_params *args_struct = args;
    switch (arrow)
    {
        case UP:
            if (args_struct->shift_row > 0)
                args_struct->shift_row--;
            break;
        case DOWN:
            args_struct->shift_row++;
            break;
        case RIGHT:
            args_struct->shift_col++;
            break;
        case LEFT:
            if (args_struct->shift_col > 0)
                args_struct->shift_col--;
    }
    char *line = args_struct->str.line;
    uint newline_count = 0;
    while (*line != '\0' && newline_count < args_struct->shift_row)
    {
        newline_count += *line++ == '\n';
    }
    print_segment_plaintext_shifted(line, args_struct->screen_region, args_struct->shift_col);
}
void start_moving_readonly_segment(string str, urectangle screen_region)
{
    //printf("STARTED THIS");
    //terminal_erase_screen;
    print_segment_plaintext_shifted(str.line, screen_region, 0);
    //print_string_segment_primitive(str, color_create_background_rgb(200, 0, 0), screen_region);
    struct move_readonly_params args;
    args.shift_row = 0;
    args.shift_col = 0;
    args.screen_region = screen_region;
    args.str = str;
    process_arrow_func_list list_element;
    list_element.process_arrow = move_arrow;
    list_element.next = NULL;
    list_element.args = &args;
    process_arrow_func_list *list = general_arrow_process_funcs;
    if (general_arrow_process_funcs == NULL)
    {
        general_arrow_process_funcs = &list_element;
    }
    else
    {
        while (list->next != NULL)
        {
            list = list->next;
        }
        list->next = &list_element;
    }
    read_process_keys(general_arrow_process_funcs,
                      general_char_process_funcs,
                      general_ctrl_process_funcs);
    //printf("ENDED THIS");
}