//
// Created by kosenko on 11.07.23.
//

#include "segments.h"
#include "terminal_io.h"
#include "../file_system/file_system_work.h"
#include "../prj_types/Array_type.h"
#include "../signals_redefinition.h"

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
    COLOR color;
};

void render_readonly_segment(void *args)
{
    struct move_readonly_params *args_struct = args;
    printf("%s", args_struct->color.line);
    char *line = args_struct->str.line;
    uint newline_count = 0;
    while (*line != '\0' && newline_count < args_struct->shift_row)
    {
        newline_count += *line++ == '\n';
    }
    print_segment_plaintext_shifted(line, args_struct->screen_region, args_struct->shift_col);
    color_to_default();
}

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
    render_readonly_segment(args);
}

//initiates moving readonly segment: segment that allows shifting text inside horizontally and vertically
void start_moving_readonly_segment(string str,
                                   urectangle screen_region,
                                   void(*changer_function)(void *args, struct winsize new_size), COLOR color)
{
    //print_segment_plaintext_shifted(str.line, screen_region, 0);
    struct move_readonly_params *args = calloc(1, sizeof(struct move_readonly_params));
    args->shift_row = 0;
    args->shift_col = 0;
    args->screen_region = screen_region;
    args->str = str;
    args->color = color;
    //TODO: add free for list of actions
    process_arrow_func_list *list_element = calloc(1, sizeof(process_arrow_func_list));
    list_element->process_arrow = move_arrow;
    list_element->next = NULL;
    list_element->args = args;
    append_processing(process_arrow_func_list, general_arrow_process_funcs, list_element)
    registration_for_window_size_update(args, changer_function);
    render_readonly_segment(args);
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
    COLOR color;
};

void render_writeable_segment(void *args)
{
    struct write_segment_params *args_struct = args;
    printf("%s", args_struct->color.line);
    char *line = args_struct->str->line;
    uint newline_count = 0;
    while (*line != '\0' && newline_count < args_struct->shift_row)
    {
        newline_count += *line++ == '\n';
    }
    print_segment_plaintext_shifted(line, args_struct->screen_region, args_struct->shift_col);
    terminal_goto(args_struct->str_row - args_struct->shift_row + args_struct->screen_region.row_start,
                  args_struct->str_col - args_struct->shift_col + args_struct->screen_region.col_start)
    color_to_default();
}

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
    render_writeable_segment(args);
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

    render_writeable_segment(args);

    //terminal_goto(args_struct->str_row - args_struct->shift_row + args_struct->screen_region.row_start,
    //              args_struct->str_col - args_struct->shift_col + args_struct->screen_region.col_start)
}

//initiates writeable segment: segment which allows printing text in and which behaves like text editor when moving cursor
void start_write_segment(string *str,
                         urectangle screen_region,
                         void (*changer_function)(void *args, struct winsize w),
                         COLOR color)
{
    print_segment_plaintext_shifted(str->line, screen_region, 0);
    terminal_goto(screen_region.row_start, screen_region.col_start)

    struct write_segment_params *args = calloc(1, sizeof(struct write_segment_params));
    args->str_row = args->str_col = args->shift_row = args->shift_col = 0;
    args->screen_region = screen_region;
    args->str = str;
    args->color = color;

    process_arrow_func_list *list_element = calloc(1, sizeof(process_arrow_func_list));
    list_element->next = NULL;
    list_element->args = args;
    list_element->process_arrow = process_arrow_in_writeable;
    append_processing(process_arrow_func_list, general_arrow_process_funcs, list_element)

    process_char_func_list *list_element_char = calloc(1, sizeof(process_char_func_list));
    list_element_char->args = args;
    list_element_char->next = NULL;
    list_element_char->process_char = process_char_in_writeable;
    append_processing(process_char_func_list, general_char_process_funcs, list_element_char)

    registration_for_window_size_update(args, changer_function);

    render_writeable_segment(args);
}

struct filesystem_segment_params
{
    urectangle screen_region;
    //TODO: REPLACE WITH ACTUAL CODE!
    //DUMMY CODE
    string str;
    COLOR color;
    //END DUMMY CODE
};

void render_filesystem_segment(void *args)
{
    //TODO: REPLACE WITH ACTUAL CODE!
    //DUMMY CODE
    struct filesystem_segment_params *args_struct = args;
    printf("%s", args_struct->color.line);
    print_segment_plaintext_shifted(args_struct->str.line, args_struct->screen_region, 0);
    color_to_default();
    //END DUMMY CODE
}
struct tree_node
{
    union
    {
        struct directory_tree *open_dir;
        string *closed_dir;
    };
    enum { CLOSED, OPEN } dir_state;
};
struct directory_tree
{
    array_voidp *dirs;
    string_array files;
    string name;
};

typedef enum
{
    DIRECTORY, FILE_C, FILE_H, FILE_DEFAULT, DEFAULT_TYPE
} printable_file_type;

struct filesystem_printable
{
    string_fast str;
    uint depth;
    printable_file_type type;
};

void directory_tree_to_printable(struct directory_tree dir, array_voidp *filesystem_printable_array_voidp, uint depth)
{
    void *current = dir.dirs->elements;

    struct filesystem_printable open_printable = {dir.name, depth, DIRECTORY};
    array_voidp_push(filesystem_printable_array_voidp, &open_printable);

    for (int i = 0; i < dir.dirs->size; i++)
    {
        struct tree_node *current_struct = current;
        if (current_struct->dir_state == OPEN)
        {
            directory_tree_to_printable(*current_struct->open_dir, filesystem_printable_array_voidp, depth + 1);
        }
        else
        {
            struct filesystem_printable
                closed_printable = {string_fast_create_from_string(*current_struct->closed_dir), depth + 1, DIRECTORY};
            array_voidp_push(filesystem_printable_array_voidp, &closed_printable);
        }
    }
    for (int i = 0; i < dir.files.size; i++)
    {
        struct filesystem_printable
            file_printable = {string_fast_create_from_string(dir.files.elements[i]), depth + 1, FILE_DEFAULT};
        array_voidp_push(filesystem_printable_array_voidp, &file_printable);
    }
}

void free_filesystem_printable_voidp(void *f_p)
{
    free_string_fast(((struct filesystem_printable *)f_p)->str);
}

void print_filesystem_segment(struct directory_tree dir,
                              char print_name,
                              struct filesystem_color_scheme color_scheme,
                              urectangle screen_region,
                              uint vertical_shift, uint horizontal_shift, char *prefix)
{
    if (print_name)
    {
        array_voidp filesystem_printable_arr = array_voidp_create(free_filesystem_printable_voidp);
        directory_tree_to_printable(dir, &filesystem_printable_arr, 0);
        //uint current_printed_line;
        uint prefix_len = strlen(prefix);
        uint segment_width = screen_region.col_end - screen_region.col_start;
        terminal_goto(screen_region.row_start, screen_region.col_start)
        for (uint i = vertical_shift;
             i < vertical_shift + segment_width && i < filesystem_printable_arr.size;
             i++)
        {
            uint j = segment_width + horizontal_shift;
            for (uint d = ((struct filesystem_printable *)array_voidp_get_element(&filesystem_printable_arr, i))->depth;
                 d-- && j;)
            {
                for (uint c = prefix_len; c && j; c--, j--)
                {
                    if (j <= segment_width)
                        putchar(prefix[c]);
                }
                //TODO: uncomment after compile run and finish!
                //for(uint c= ((struct filesystem_printable*)array_voidp_get_element(&filesystem_printable_arr,i))->str.last_element)
            }
        }

        free_array_voidp(&filesystem_printable_arr);
    }
    else
    {
        //TODO finish!
    }
}

void start_filesystem_segment(file_system_anchor anchor,
                              urectangle screen_region,
                              void (*changer_function)(void *element, struct winsize w))
{
    //TODO: REPLACE WITH ACTUAL CODE!
    //DUMMY CODE
    struct filesystem_segment_params *args = calloc(1, sizeof(struct filesystem_segment_params));
    args->screen_region = screen_region;
    args->color = color_create_background_rgb(50, 10, 50);
    args->str = string_create_from_fcharp("filesystem");

    registration_for_window_size_update(args, changer_function);
    //END DUMMY CODE
}

struct static_params
{
    string str;
    urectangle screen_region;
    COLOR color;
};

void render_static_segment(void *args)
{
    struct static_params *args_struct = args;
    printf("%s", args_struct->color.line);
    print_segment_plaintext_shifted(args_struct->str.line, args_struct->screen_region, 0);
    color_to_default();
}

void start_static_segment(string str,
                          COLOR color,
                          urectangle screen_region,
                          void (*changer_function)(void *element, struct winsize w))
{
    struct static_params *args = calloc(1, sizeof(struct static_params));
    args->color = color;
    args->screen_region = screen_region;
    args->str = str;

    registration_for_window_size_update(args, changer_function);
    render_static_segment(args);
}