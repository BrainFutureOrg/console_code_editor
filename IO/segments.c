//
// Created by kosenko on 11.07.23.
//

#include "segments.h"
#include "terminal_io.h"

#define DEL 127

//prints segment with given string and color in screen region, "{", "}" and "#..." are highlighted
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

//prints text in screen region with given horizontal shift
void print_segment_plaintext_shifted(char *text, urectangle screen_region, uint text_start_col)
{
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

//arg for moving readonly segment
struct move_readonly_params
{
    uint shift_row;
    uint shift_col;
    string str;
    urectangle screen_region;
};

//arrow processor for moving readonly segment: shifts text horizontally or vertically, then reprints segment
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

//initiates moving readonly segment: segment that allows shifting text inside horizontally and vertically
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
    append_processing(process_arrow_func_list, general_arrow_process_funcs, &list_element)
    read_process_keys(general_arrow_process_funcs,
                      general_char_process_funcs,
                      general_ctrl_process_funcs);
}

//args for writeable segments char processing methods
struct write_segment_params
{
    uint shift_row;
    uint shift_col;
    uint str_row;
    uint str_col;
    string *str;
    urectangle screen_region;
};

//arrow processing method for writeable segment
void process_arrow_in_writeable(ARROW arrow, void *args)
{
    struct write_segment_params *args_struct = args;
    char *current_char = args_struct->str->line;
    switch (arrow)
    {
        case UP:
            if (args_struct->str_row > 0)
            {
                if (args_struct->str_row == args_struct->shift_row)
                {
                    args_struct->shift_row--;
                }
                args_struct->str_row--;
                for (int row = 0; row < args_struct->str_row; row += *current_char++ == '\n');
                int new_col = 0;
                for (; new_col < args_struct->str_col && *current_char++ != '\n'; new_col++);
                args_struct->str_col = new_col;
                if (args_struct->shift_col > args_struct->str_col)
                {
                    args_struct->shift_col = args_struct->str_col - 1;
                }
                if (args_struct->shift_col < 0)
                {
                    args_struct->shift_col = 0;
                }
            }
            break;
        case DOWN:;
            uint line_count = 0;

            while (*current_char != '\0')
                line_count += *current_char++ == '\n';
            if (args_struct->str_row < line_count)
            {
                if (args_struct->str_row == args_struct->shift_row + args_struct->screen_region.row_end
                    - args_struct->screen_region.row_start - 1)
                {
                    args_struct->shift_row++;
                }
                args_struct->str_row++;
                current_char = args_struct->str->line;
                for (int row = 0; row < args_struct->str_row; row += *current_char++ == '\n');
                int new_col = 0;
                for (; new_col < args_struct->str_col && *current_char++ != '\n'; new_col++);
                args_struct->str_col = new_col;

                if (args_struct->shift_col > args_struct->str_col)
                {
                    args_struct->shift_col = args_struct->str_col - 1;
                }
                if (args_struct->shift_col < 0)
                {
                    args_struct->shift_col = 0;
                }
            }
            break;
        case RIGHT:
            current_char = args_struct->str->line;
            for (uint row = 0; row < args_struct->str_row; row += *current_char++ == '\n');
            for (uint col = 0; col < args_struct->str_col; col++, current_char++);
            if (*current_char == '\n')
            {
                if (args_struct->str_row == args_struct->shift_row + args_struct->screen_region.row_end
                    - args_struct->screen_region.row_start - 1)
                {
                    args_struct->shift_row++;
                }
                args_struct->str_row++;
                args_struct->str_col = 0;
                args_struct->shift_col = 0;
            }
            else if (*current_char != '\0')
            {
                if (args_struct->str_col == args_struct->shift_col + args_struct->screen_region.col_end
                    - args_struct->screen_region.col_start - 1)
                {
                    args_struct->shift_col++;
                }
                args_struct->str_col++;
            }
            break;
        case LEFT:

            if (args_struct->str_col)
            {
                if (args_struct->str_col == args_struct->shift_col)
                {
                    args_struct->shift_col--;
                }
                args_struct->str_col--;
            }
            else
            {
                if (args_struct->str_row == 0)
                    break;
                current_char = args_struct->str->line;
                for (uint row = 1; row < args_struct->str_row; row += *current_char++ == '\n');
                for (args_struct->str_col = 0; *current_char++ != '\n'; args_struct->str_col++);
                if (args_struct->shift_row == args_struct->str_row)
                    args_struct->shift_row--;
                args_struct->str_row--;
                if (args_struct->shift_col + args_struct->screen_region.col_end - args_struct->screen_region.col_start
                    < args_struct->str_col)
                    args_struct->shift_col = args_struct->str_col - args_struct->screen_region.col_end
                        + args_struct->screen_region.col_start + 1;
                //if (args_struct->shift_col < 0)
                //    args_struct->shift_col = 0;

            }
    }
    char *line = args_struct->str->line;
    uint newline_count = 0;
    while (*line != '\0' && newline_count < args_struct->shift_row)
    {
        newline_count += *line++ == '\n';
    }
    print_segment_plaintext_shifted(line, args_struct->screen_region, args_struct->shift_col);
    terminal_goto(args_struct->str_row - args_struct->shift_row + args_struct->screen_region.row_start,
                  args_struct->str_col - args_struct->shift_col + args_struct->screen_region.col_start)
}

//char processor for writeable segment
void process_char_in_writeable(char c, void *args)
{
    struct write_segment_params *args_struct = args;
    if (c != DEL)
        insert_into_string_multiline(args_struct->str, c, args_struct->str_row, args_struct->str_col);
    if (c == '\n')
    {
        args_struct->str_col = 0;
        if (args_struct->str_row
            == args_struct->shift_row + args_struct->screen_region.row_end - args_struct->screen_region.row_start - 1)
            args_struct->shift_row++;
        args_struct->shift_col = 0;
        args_struct->str_row++;
    }
    else if (c == DEL)
    {
        if (args_struct->str_col)
        {
            if (args_struct->str_col == args_struct->shift_col)
            {
                args_struct->shift_col--;
            }
            args_struct->str_col--;
        }
        else
        {
            char *current_char = args_struct->str->line;
            if (args_struct->str_row == 0)
                return;
            current_char = args_struct->str->line;
            for (uint row = 1; row < args_struct->str_row; row += *current_char++ == '\n');
            for (args_struct->str_col = 0; *current_char++ != '\n'; args_struct->str_col++);
            if (args_struct->shift_row == args_struct->str_row)
                args_struct->shift_row--;
            args_struct->str_row--;
            if (args_struct->shift_col + args_struct->screen_region.col_end - args_struct->screen_region.col_start
                < args_struct->str_col)
                args_struct->shift_col = args_struct->str_col - args_struct->screen_region.col_end
                    + args_struct->screen_region.col_start + 1;
        }
        delete_from_string_multiline(args_struct->str, args_struct->str_row, args_struct->str_col);
    }
    else
    {
        if (args_struct->str_col
            == args_struct->shift_col + args_struct->screen_region.col_end - args_struct->screen_region.col_start - 1)
            args_struct->shift_col++;
        args_struct->str_col++;
    }

    char *line = args_struct->str->line;
    uint newline_count = 0;
    while (*line != '\0' && newline_count < args_struct->shift_row)
    {
        newline_count += *line++ == '\n';
    }
    print_segment_plaintext_shifted(line, args_struct->screen_region, args_struct->shift_col);

    terminal_goto(args_struct->str_row - args_struct->shift_row + args_struct->screen_region.row_start,
                  args_struct->str_col - args_struct->shift_col + args_struct->screen_region.col_start)
}

//initiates writeable segment: segment which allows printing text in and which behaves like text editor when moving cursor
void start_write_segment(string *str, urectangle screen_region)
{
    print_segment_plaintext_shifted(str->line, screen_region, 0);
    terminal_goto(screen_region.row_start, screen_region.col_start)

    struct write_segment_params args;
    args.str_row = args.str_col = args.shift_row = args.shift_col = 0;
    args.screen_region = screen_region;
    args.str = str;
    process_arrow_func_list list_element;
    list_element.next = NULL;
    list_element.args = &args;
    list_element.process_arrow = process_arrow_in_writeable;
    append_processing(process_arrow_func_list, general_arrow_process_funcs, &list_element)

    process_char_func_list list_element_char;
    list_element_char.args = &args;
    list_element_char.next = NULL;
    list_element_char.process_char = process_char_in_writeable;
    { append_processing(process_char_func_list, general_char_process_funcs, &list_element_char)}
    read_process_keys(general_arrow_process_funcs,
                      general_char_process_funcs,
                      general_ctrl_process_funcs);
}