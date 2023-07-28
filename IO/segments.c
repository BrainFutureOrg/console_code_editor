//
// Created by kosenko on 11.07.23.
//

#include "segments.h"
#include "terminal_io.h"
#include "../file_system/file_system_work.h"
#include "../prj_types/Array_type.h"
#include "../signals_redefinition.h"
#include "../loging/log.h"

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

struct tree_node
{
    union
    {
        struct directory_tree *open_dir;
        string *closed_dir;
    };
    enum
    {
        CLOSED, OPEN
    } dir_state;
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
    struct filesystem_printable *open_printable = calloc(1, sizeof(struct filesystem_printable));
    *open_printable = (struct filesystem_printable){dir.name, depth, DIRECTORY};
    array_voidp_push(filesystem_printable_array_voidp, open_printable);
    write_log(DEBUG, "directory_tree_to_printable start fst loop");
//    write_log(DEBUG, "directory_tree_to_printable dirs size = %d", dir.dirs.size);
    for (int i = 0; i < dir.dirs.size; i++)
    {
//        write_log(DEBUG, "directory_tree_to_printable loop %d iteration", i);
        struct tree_node *current_struct = array_voidp_get_element(&dir.dirs, i);
        if (current_struct->dir_state == OPEN)
        {
//            write_log(DEBUG, "directory_tree_to_printable OPENED dir", i);
            directory_tree_to_printable(*current_struct->open_dir, filesystem_printable_array_voidp, depth + 1);
        }
        else
        {
//            write_log(DEBUG, "directory_tree_to_printable creating filesystem_printable %d", current_struct->dir_state);
            struct filesystem_printable *closed_printable = calloc(1, sizeof(struct filesystem_printable));
            *closed_printable =
                (struct filesystem_printable){string_fast_create_from_string(*current_struct->closed_dir), depth + 1,
                                              DIRECTORY};
//            write_log(DEBUG, "directory_tree_to_printable wait for array_voidp_push");
            array_voidp_push(filesystem_printable_array_voidp, closed_printable);
//            write_log(DEBUG, "directory_tree_to_printable array_voidp_push - ok");
        }
    }
    write_log(DEBUG, "directory_tree_to_printable end fst loop");

    write_log(DEBUG, "directory_tree_to_printable start snd loop");
    for (int i = 0; i < dir.files.size; i++)
    {
        struct filesystem_printable *file_printable = calloc(1, sizeof(struct filesystem_printable));
        *file_printable =
            (struct filesystem_printable){string_fast_create_from_string(string_array_get_element(&dir.files, i)),
                                          depth + 1, FILE_DEFAULT};
        array_voidp_push(filesystem_printable_array_voidp, file_printable);
    }
    write_log(DEBUG, "directory_tree_to_printable end snd loop");
}

uint directory_tree_count(struct directory_tree dir)
{
    void *current = dir.dirs.elements;
    uint result = 1;
    for (int i = 0; i < dir.dirs.size; i++)
    {
        struct tree_node *current_struct = current++;
        if (current_struct->dir_state == OPEN)
        {
            result += directory_tree_count(*current_struct->open_dir);
        }
        else
        {
            result++;
        }
    }
    return result + dir.files.size;
}

void free_filesystem_printable_voidp(void *f_p)
{
    free_string_fast(((struct filesystem_printable *)f_p)->str);
}

void print_filesystem_segment(array_voidp filesystem_printable_arr,
                              struct filesystem_segment_params params)
{
    //array_voidp filesystem_printable_arr = array_voidp_create(free_filesystem_printable_voidp);
    //directory_tree_to_printable(dir, &filesystem_printable_arr, 0);
    //uint current_printed_line;
    uint prefix_len = strlen(params.prefix);
    uint segment_width = params.screen_region.col_end - params.screen_region.col_start;
    terminal_goto(params.screen_region.row_start, params.screen_region.col_start)
    //if (print_name)
    //{
//    write_log(DEBUG, "print_filesystem_segment creating - ok");
    for (uint i = params.vertical_shift + !params.print_name;
         i < params.vertical_shift + segment_width + !params.print_name && i < filesystem_printable_arr.size;
         i++)
    {
//        write_log(DEBUG, "print_filesystem_segment start loop - ok");
//        write_log(DEBUG, "type is %d", ((struct filesystem_printable *)filesystem_printable_arr.elements[i])->type);
        //TODO code problem
        switch (((struct filesystem_printable *)filesystem_printable_arr.elements[i])->type)
        {
            case DIRECTORY:
                printf("%s", params.color_scheme.dir.line);
                break;
            case FILE_DEFAULT:
            case FILE_C:
            case FILE_H:
                printf("%s", params.color_scheme.file.line);
                break;
            default:
                printf("%s", params.color_scheme.default_color.line);
        }
        if (i == params.cursor)
        {
            color_inverse();
        }
        uint j = segment_width + params.horizontal_shift;
//        write_log(DEBUG,
//                  "print_filesystem_segment loop %s",
//                  ((struct filesystem_printable *)array_voidp_get_element(&filesystem_printable_arr, i))->str
//                      .string_part.line);
        for (uint d =
            ((struct filesystem_printable *)array_voidp_get_element(&filesystem_printable_arr, i))->depth
                - !params.print_name; d-- && j;)
        {
            for (uint c = prefix_len; c && j; c--, j--)
            {
                if (j <= segment_width)
                    putchar(params.prefix[c]);
            }
            char *line = ((struct filesystem_printable *)array_voidp_get_element(&filesystem_printable_arr, i))->str
                .string_part.line;
            while (j)
            {
                putchar(*line++);
                j--;
            }
        }
//        write_log(DEBUG, "print_filesystem_segment end loop - ok");
    }

//    write_log(DEBUG, "print_filesystem_segment full end loop - ok");
    //TODO: why free memory?
//    free_array_voidp(&filesystem_printable_arr);
//    write_log(DEBUG, "print_filesystem_segment free - ok");
    //}
    //else
    //{
    //No.
    //}
}

void render_filesystem_segment(void *args)
{
    struct filesystem_segment_params *args_struct = args;
    write_log(DEBUG, "creating - ok");
    array_voidp filesystem_printable_arr = array_voidp_create(free_filesystem_printable_voidp);
    write_log(DEBUG, "array_voidp_create - ok");
    directory_tree_to_printable(args_struct->dir, &filesystem_printable_arr, 0);
    write_log(DEBUG, "directory_tree_to_printable - ok");
    print_filesystem_segment(filesystem_printable_arr, *args_struct);
    write_log(DEBUG, "print_filesystem_segment - ok");
    free_array_voidp(&filesystem_printable_arr);
    write_log(DEBUG, "free - ok");
}

void process_arrow_filesystem(ARROW arrow, void *args)
{
    struct filesystem_segment_params *args_struct = args;
    switch (arrow)
    {
        case UP:
            if (args_struct->cursor > !args_struct->print_name)
            {
                args_struct->cursor--;
                if (args_struct->cursor < args_struct->vertical_shift)
                {
                    args_struct->vertical_shift = args_struct->cursor;
                }
            }
            break;
        case DOWN:
            if (args_struct->cursor < directory_tree_count(args_struct->dir) - !args_struct->print_name)
            {
                args_struct->cursor++;
                if (args_struct->cursor > args_struct->vertical_shift + args_struct->screen_region.col_end
                    - args_struct->screen_region.col_start)
                {
                    args_struct->vertical_shift =
                        args_struct->cursor - args_struct->screen_region.col_end +
                            args_struct->screen_region.col_start;
                }
            }
            break;
        case RIGHT:
            args_struct->horizontal_shift++;
            break;
        case LEFT:
            if (args_struct->horizontal_shift)
                args_struct->horizontal_shift--;
    }
    render_filesystem_segment(args);
}

char enter_cursor_recursive(file_system_anchor *anchor_to_change,
                            struct directory_tree *dir_to_change,
                            uint cursor, uint depth)
{
    depth++;
    for (int i = 0; i < dir_to_change->dirs.size; i++)
    {
        //struct tree_node* node=dir_to_change->dirs.
    }
}

void process_char_filesystem(char c, void *args)
{
    struct filesystem_segment_params *args_struct = args;
    if (c == '\n')
    {
        if (args_struct->cursor)
        {
            for (int i = 0; i < args_struct->dir.dirs.size; i++)
            {
                if (((struct tree_node *)args_struct->dir.dirs.elements[i])->dir_state == OPEN)
                {

                }
            }
        }
    }
    if (c == 'o' || c == 'O')
    {

    }
    if (c == 'm' || c == 'M')
    {

    }
    if (c == 'b' || c == 'B')
    {

    }
}

void free_directory_tree_voidp(void *dir)
{
    struct directory_tree *dir_struct = dir;
    free_array_voidp(&(dir_struct->dirs));
    free_string_array(&dir_struct->files);
    free_string(dir_struct->name);
}

void start_filesystem_segment(file_system_anchor anchor,
                              urectangle screen_region,
                              void (*changer_function)(void *element, struct winsize w),
                              filesystem_color_scheme color_scheme,
                              char *prefix)
{
    struct filesystem_segment_params *args = calloc(1, sizeof(struct filesystem_segment_params));
    args->screen_region = screen_region;
    args->color_scheme = color_scheme;
    args->vertical_shift = 0;
    args->horizontal_shift = 0;
    args->cursor = 0;
    args->print_name = 1;
    args->current_outermost_dir = anchor;
    args->dir.dirs = array_voidp_create(free_directory_tree_voidp);
    args->dir.files = string_array_create();
    args->prefix = prefix;
    files_dirs_from_directory content = system_anchor_get_dir_content(anchor);

    for (int i = 0; i < content.dirs.size; i++)
    {
        struct tree_node *node = calloc(1, sizeof(struct tree_node));
        node->dir_state = CLOSED;
        node->closed_dir = &content.dirs.elements[i];
        array_voidp_push(&args->dir.dirs, node);
    }
    for (int i = 0; i < content.files.size; i++)
    {
        string_array_push(&args->dir.files, content.files.elements[i]);
    }

    process_arrow_func_list *list_element = calloc(1, sizeof(process_arrow_func_list));
    list_element->next = NULL;
    list_element->args = args;
    list_element->process_arrow = process_arrow_filesystem;
    append_processing(process_arrow_func_list, general_arrow_process_funcs, list_element)

    process_char_func_list *list_element_char = calloc(1, sizeof(process_char_func_list));
    list_element_char->args = args;
    list_element_char->next = NULL;
    list_element_char->process_char = process_char_filesystem;

    append_processing(process_char_func_list, general_char_process_funcs, list_element_char)
    write_log(DEBUG, "append processing - ok");

    registration_for_window_size_update(args, changer_function);
    write_log(DEBUG, "registration for update - ok");

    render_filesystem_segment(args);
}

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