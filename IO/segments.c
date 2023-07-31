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

void print_segment_plaintext_shifted_line_highlight(char *text,
                                                    urectangle screen_region,
                                                    uint text_start_col,
                                                    COLOR main_color,
                                                    COLOR highlight_color,
                                                    uint highlight_line,
                                                    char highlight)
{
    terminal_goto(screen_region.row_start, screen_region.col_start)
    for (uint row = screen_region.row_start; row < screen_region.row_end; row++)
    {
        if (row - screen_region.row_start == highlight_line && highlight)
        {
            printf("%s", highlight_color.line);
        }
        else
        {
            printf("%s", main_color.line);
        }
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
    //print_segment_plaintext_shifted(line, args_struct->screen_region, args_struct->shift_col);
    print_segment_plaintext_shifted_line_highlight(line,
                                                   args_struct->screen_region,
                                                   args_struct->shift_col,
                                                   args_struct->color,
                                                   args_struct->current_line_color,
                                                   args_struct->str_row - args_struct->shift_row,
                                                   args_struct->active);
    //write_log(DEBUG, "pre-goto");
    terminal_goto(args_struct->str_row - args_struct->shift_row + args_struct->screen_region.row_start,
                  args_struct->str_col - args_struct->shift_col + args_struct->screen_region.col_start)
    if (args_struct->active)
        terminal_visible_cursor;
    color_to_default();
}

//arrow processing method for writeable segment
void process_arrow_in_writeable(ARROW arrow, void *args)
{
    struct write_segment_params *args_struct = args;
    if (!args_struct->active)
        return;
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
    if (!args_struct->active)
        return;
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

void render_file_name_segment_self_delete(void *args)
{
    render_file_name_segment(args);
    delete_from_after_key_list_global(render_file_name_segment_self_delete, args);
}

void process_ctrl_in_writeable(char c, void *args)
{
    struct write_segment_params *args_struct = args;
    if (!args_struct->active)
        return;
    if (c == CTRL_('F'))
    {
        terminal_invisible_cursor;
        args_struct->active = 0;
        args_struct->filesystem_segment_args->active = 1;
        args_struct->instruction_args->str = string_create_from_fcharp(
            "enter - enter directory or file  o - open directory in tree-like structure or open file\nm - go to main text segment  ctrl+e - exit");
        render_static_segment(args_struct->instruction_args);
        render_filesystem_segment(args_struct->filesystem_segment_args);
        render_writeable_segment(args);
    }
    else if (c == CTRL_('N'))
    {
        //write_log(DEBUG, "ctrl n");
        args_struct->active = 0;
        args_struct->file_name_args->active = 1;
        //render_file_name_segment(args_struct->file_name_args);
        process_after_key_list *element = calloc(1, sizeof(process_after_key_list));
        element->args = args_struct->file_name_args;
        element->process_after_key = render_file_name_segment_self_delete;
        element->free_args = NULL;
        append_processing(process_after_key_list, general_after_key_funcs, element)

        args_struct->instruction_args->str =
            string_create_from_fcharp("enter - go to main text segment  ctrl+e - exit");
        render_static_segment(args_struct->instruction_args);
    }
}

//initiates writeable segment: segment which allows printing text in and which behaves like text editor when moving cursor
struct write_segment_params *start_write_segment(string *str,
                                                 urectangle screen_region,
                                                 void (*changer_function)(void *args, struct winsize w),
                                                 COLOR color, COLOR current_line_color)
{
    print_segment_plaintext_shifted(str->line, screen_region, 0);
    terminal_goto(screen_region.row_start, screen_region.col_start)

    struct write_segment_params *args = calloc(1, sizeof(struct write_segment_params));
    args->str_row = args->str_col = args->shift_row = args->shift_col = 0;
    args->screen_region = screen_region;
    args->str = str;
    args->color = color;
    args->current_line_color = current_line_color;

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

    process_ctrl_func_list *list_element_ctrl = calloc(1, sizeof(process_ctrl_func_list));
    list_element_ctrl->args = args;
    list_element_ctrl->next = NULL;
    list_element_ctrl->process_ctrl_char = process_ctrl_in_writeable;
    append_processing(process_ctrl_func_list, general_ctrl_process_funcs, list_element_ctrl)

    registration_for_window_size_update(args, changer_function);

    render_writeable_segment(args);
    return args;
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

void free_directory_tree_voidp(void *dir)
{
    struct directory_tree *dir_struct = dir;
    free_array_voidp(&(dir_struct->dirs));
    free_string_array(&dir_struct->files);
    free_string(dir_struct->name);
}

void free_tree_node_voidp(void *node)
{
    struct tree_node *node_struct = node;
    if (node_struct->dir_state == OPEN)
    {
        free_directory_tree_voidp(node_struct->open_dir);
    }
    else
    {
        free_string(*node_struct->closed_dir);
    }
}

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
    *open_printable = (struct filesystem_printable){string_fast_create_from_string(dir.name), depth, DIRECTORY};//
    array_voidp_push(filesystem_printable_array_voidp, open_printable);
    //(DEBUG, "directory_tree_to_printable start fst loop");
    //write_log(DEBUG, "directory_tree_to_printable dirs size = %d", dir.dirs.size);
    for (int i = 0; i < dir.dirs.size; i++)
    {
        //write_log(DEBUG, "directory_tree_to_printable loop %d iteration", i);
        struct tree_node *current_struct = array_voidp_get_element(&dir.dirs, i);
        if (current_struct->dir_state == OPEN)
        {
            //write_log(DEBUG, "directory_tree_to_printable OPENED dir", i);
            directory_tree_to_printable(*current_struct->open_dir, filesystem_printable_array_voidp, depth + 1);
        }
        else
        {
            //write_log(DEBUG, "directory_tree_to_printable creating filesystem_printable %d", current_struct->dir_state);
            struct filesystem_printable *closed_printable = calloc(1, sizeof(struct filesystem_printable));
            *closed_printable =
                (struct filesystem_printable){string_fast_create_from_string(*current_struct->closed_dir), depth + 1,
                                              DIRECTORY};
            //write_log(DEBUG, "directory_tree_to_printable wait for array_voidp_push");
            array_voidp_push(filesystem_printable_array_voidp, closed_printable);
            //write_log(DEBUG, "directory_tree_to_printable array_voidp_push - ok");
        }
    }
    //write_log(DEBUG, "directory_tree_to_printable end fst loop");

    //write_log(DEBUG, "directory_tree_to_printable start snd loop");
    for (int i = 0; i < dir.files.size; i++)
    {
        struct filesystem_printable *file_printable = calloc(1, sizeof(struct filesystem_printable));
        *file_printable =
            (struct filesystem_printable){string_fast_create_from_string(string_array_get_element(&dir.files, i)),
                                          depth + 1, FILE_DEFAULT};
        array_voidp_push(filesystem_printable_array_voidp, file_printable);
    }
    //write_log(DEBUG, "directory_tree_to_printable end snd loop");
}

uint directory_tree_count(struct directory_tree dir)
{
    struct tree_node *current;
    write_log(DEBUG,
              "count after unbox, %d dirs %d files",
              dir.dirs.size,
              dir.files.size
    );
    uint result = 1;
    for (int i = 0; i < dir.dirs.size; i++)
    {
        current = array_voidp_get_element(&dir.dirs, i);
        if (current->dir_state == OPEN)
        {
            write_log(DEBUG, "iter %d is open", i);
            result += directory_tree_count(*current->open_dir);
        }
        else
        {
            write_log(DEBUG, "iter %d is closed", i);
            result++;
        }
    }
    write_log(DEBUG, "dir+inner count=%d", result);
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
    uint segment_height = params.screen_region.row_end - params.screen_region.row_start;
    //terminal_goto(params.screen_region.row_start, params.screen_region.col_start)//unneeded now
    //if (print_name)
    //{
//    write_log(DEBUG, "print_filesystem_segment creating - ok");
    //for (uint i = 0; i < filesystem_printable_arr.size; i++)
    //{
    //write_log(DEBUG,
    //          "fsystem printable[%d].depth=%d",
    //          i,
    //         ((struct filesystem_printable *)array_voidp_get_element(&filesystem_printable_arr, i))->depth);
    //}
    for (uint i = params.vertical_shift + !params.print_name;
         i < params.vertical_shift + segment_height + !params.print_name;
         i++)
    {
        terminal_goto(params.screen_region.row_start + i - params.vertical_shift - !params.print_name,
                      params.screen_region.col_start)
//        write_log(DEBUG, "print_filesystem_segment start loop - ok");
//        write_log(DEBUG, "type is %d", ((struct filesystem_printable *)filesystem_printable_arr.elements[i])->type);
        color_to_default();
        if (i < filesystem_printable_arr.size)
        {
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
            if (i == params.cursor && params.active)
            {
                color_inverse();
            }
            uint j = segment_width + params.horizontal_shift;
//        write_log(DEBUG,
//                  "print_filesystem_segment loop %s",
//                  ((struct filesystem_printable *)array_voidp_get_element(&filesystem_printable_arr, i))->str
//                      .string_part.line);

            uint d =
                ((struct filesystem_printable *)array_voidp_get_element(&filesystem_printable_arr, i))->depth;
            d -= (d ? !params.print_name : 0);
            for (; d-- && j;)
            {
                //write_log(DEBUG,
                //          "in struct = %ld",
                //          ((struct filesystem_printable *)array_voidp_get_element(&filesystem_printable_arr, i))
                //              ->depth);
                for (uint c = 0; c < prefix_len && j; c++, j--)
                {
                    if (j <= segment_width)
                        putchar(params.prefix[c]);
                }
            }
            char *line = ((struct filesystem_printable *)array_voidp_get_element(&filesystem_printable_arr, i))->str
                .string_part.line;
            while (j)
            {
                if (j <= segment_width)
                {
                    if (line != NULL && *line != '\0')
                        putchar(*line);
                    else
                        putchar(' ');
                }
                if (line != NULL && *line != '\0')
                    line++;
                j--;
            }
        }
        else
        {
            printf("%s", params.color_scheme.default_color.line);
            for (uint j = segment_width; j--;)
                putchar(' ');
        }
//        write_log(DEBUG, "print_filesystem_segment end loop - ok");
    }
    color_to_default();
}

void render_filesystem_segment(void *args)
{
    //write_log(DEBUG, "void unboxing - ok");
    struct filesystem_segment_params *args_struct = args;
    //write_log(DEBUG, "creating - ok");
    array_voidp filesystem_printable_arr = array_voidp_create(free_filesystem_printable_voidp);
    //write_log(DEBUG, "array_voidp_create - ok");
    directory_tree_to_printable(args_struct->dir, &filesystem_printable_arr, 0);
    //write_log(DEBUG, "directory_tree_to_printable - ok");
    print_filesystem_segment(filesystem_printable_arr, *args_struct);
    //write_log(DEBUG, "print_filesystem_segment - ok");
    free_array_voidp(&filesystem_printable_arr);
    //write_log(DEBUG, "free - ok");
}

void process_arrow_filesystem(ARROW arrow, void *args)
{
    struct filesystem_segment_params *args_struct = args;
    if (!args_struct->active)
        return;
    write_log(DEBUG, "process arrow started");
    switch (arrow)
    {
        case UP:
            if (args_struct->cursor > !args_struct->print_name)
            {
                args_struct->cursor--;
                if (args_struct->cursor - 1 < args_struct->vertical_shift)
                {
                    args_struct->vertical_shift = args_struct->cursor - 1;
                }
            }
            break;
        case DOWN:
            if (args_struct->cursor < directory_tree_count(args_struct->dir) - !args_struct->print_name)
            {
                args_struct->cursor++;
                if (args_struct->cursor > args_struct->vertical_shift + args_struct->screen_region.row_end
                    - args_struct->screen_region.row_start)
                {
                    args_struct->vertical_shift =
                        args_struct->cursor - args_struct->screen_region.row_end +
                            args_struct->screen_region.row_start;
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
    write_log(DEBUG, "process arrow pre-render");
    render_filesystem_segment(args);
}

//needed only for enter_cursor_recursive
enum recursive_state
{
    STATE_NONE, STATE_DIR, STATE_FILE
};

struct directory_tree *dir_tree_from_anchor(file_system_anchor anchor)
{
    struct directory_tree *result = calloc(1, sizeof(struct directory_tree));
    result->dirs = array_voidp_create(free_tree_node_voidp);//
    result->files = string_array_create();//
    files_dirs_from_directory content = system_anchor_get_dir_content(anchor);
    for (int i = 0; i < content.dirs.size; i++)
    {
        struct tree_node *node = calloc(1, sizeof(struct tree_node));
        node->dir_state = CLOSED;
        node->closed_dir = calloc(1, sizeof(string));
        *node->closed_dir = string_copy(string_array_get_element(&content.dirs, i));
        array_voidp_push(&result->dirs, node);
    }
    for (int i = 0; i < content.files.size; i++)
    {
        string_array_push(&result->files, string_array_get_element(&content.files, i));
    }
    free_files_dirs_from_directory(content);
    result->name = system_anchor_get_directory_name(&anchor);
    return result;
}

void render_writeable_segment_self_delete(void *args)
{
    //write_log(DEBUG, "render self destruct start");
    render_writeable_segment(args);
    delete_from_after_key_list_global(render_writeable_segment_self_delete, args);
}

void activate_writeable_segment_self_delete(void *args)
{
    //write_log(DEBUG, "activate self destruct start");
    struct write_segment_params *args_struct = args;
    args_struct->active = 1;
    delete_from_after_key_list_global(activate_writeable_segment_self_delete, args);
}

enum recursive_state enter_cursor_recursive(file_system_anchor *anchor_to_change,
                                            struct directory_tree *dir_new,
                                            struct directory_tree *dir_old,
                                            uint cursor,
                                            uint *depth,
                                            char inplace,
                                            struct filesystem_segment_params *args)
{
    if (*depth == cursor && !inplace)
    {
        *dir_new = *dir_tree_from_anchor(*anchor_to_change);
        return STATE_DIR;
    }
    (*depth)++;
    //system_anchor_go_to_dir(anchor_to_change, dir_old.name);
    for (int i = 0; i < dir_old->dirs.size; i++)
    {
        struct tree_node *dir = array_voidp_get_element(&dir_old->dirs, i);
        if (dir->dir_state == OPEN)
        {
            //write_log(DEBUG, "open dir opened in enter_cursor_recursive");
            enum recursive_state state;
            system_anchor_go_to_dir(anchor_to_change, dir->open_dir->name);
            if (cursor == *depth && inplace)
            {
                string content = string_copy(dir->open_dir->name);
                free_directory_tree_voidp(dir->open_dir);
                dir->dir_state = CLOSED;
                dir->closed_dir = calloc(1, sizeof(string));
                *dir->closed_dir = content;
                return STATE_DIR;
            }
            if ((state = enter_cursor_recursive(anchor_to_change, dir_new, dir->open_dir, cursor, depth, inplace, args))
                != STATE_NONE)
            {
                write_log(DEBUG, "ended in recursive(");
                return state;
            }
            system_anchor_go_back_from_dir(anchor_to_change);

        }
        else
        {

            if (*depth == cursor)
            {
                system_anchor_go_to_dir(anchor_to_change, *dir->closed_dir);
                *dir_new = *dir_tree_from_anchor(*anchor_to_change);//TODO free right part
                //write_log(DEBUG, "after tree from anchor dir.dirs.size = %d", dir_new->dirs.size);
                if (inplace)
                {
                    dir->dir_state = OPEN;
                    dir->open_dir = dir_new;
                }
                return STATE_DIR;
            }
            (*depth)++;
        }
    }
    if (dir_old->files.size > cursor - *depth)
    {
        //write_log(DEBUG, "trying open file");
        string filename = string_array_get_element(&dir_old->files, cursor - *depth);

        free_string(*args->write_segment_args->str);
        //write_log(DEBUG, "file enter pre-get str");
        string *result = calloc(1, sizeof(string));
        *result = anchor_read_file(*anchor_to_change, filename.line);
        //write_log(DEBUG, "file enter got str");
        args->write_segment_args->str = result;

        args->write_segment_args->str_col = 0;
        args->write_segment_args->str_row = 0;
        args->write_segment_args->shift_col = 0;
        args->write_segment_args->shift_row = 0;

        //render_writeable_segment(args->write_segment_args); // ->
        process_after_key_list *element = calloc(1, sizeof(process_after_key_list));
        element->args = args->write_segment_args;
        element->process_after_key = render_writeable_segment_self_delete;
        element->free_args = NULL;
        append_processing(process_after_key_list, general_after_key_funcs, element)
        //write_log(DEBUG, "general after arrow %s NULL", general_after_key_funcs == NULL ? "==" : "!=");

        args->file_name_segment_args->str = filename;
        args->file_name_segment_args->anchor = *anchor_to_change;
        render_file_name_segment(args->file_name_segment_args);
        //write_log(DEBUG, "entered file, %d < %d", dir_old->files.size, cursor - *depth + 1);
        return STATE_FILE;
    }
    else
    {
        *depth += dir_old->files.size;//-1
    }
    //system_anchor_go_back_from_dir(anchor_to_change);
    return STATE_NONE;
}

void process_char_filesystem(char c, void *args)
{
    struct filesystem_segment_params *args_struct = args;
    if (!args_struct->active)
        return;
    if (c == '\n')
    {
        file_system_anchor new_anchor = anchor_copy(args_struct->current_outermost_dir);
        struct directory_tree *new_dir = calloc(1, sizeof(struct directory_tree));//TODO free
        uint depth = 0;
        enum recursive_state
            state =
            enter_cursor_recursive(&new_anchor, new_dir, &args_struct->dir, args_struct->cursor, &depth, 0, args);
        switch (state)
        {
            case STATE_DIR:
                //free_file_system_anchor(args_struct->current_outermost_dir);
                args_struct->current_outermost_dir = new_anchor;
                //free_directory_tree_voidp(&args_struct->dir);
                args_struct->dir = *new_dir;
                args_struct->cursor = 1;
                args_struct->vertical_shift = 0;
                break;
            case STATE_FILE:
                break;
            default:
                errno = ERANGE;
        }
    }
    if (c == 'o' || c == 'O')
    {
        file_system_anchor new_anchor = anchor_copy(args_struct->current_outermost_dir);
        struct directory_tree *new_dir = calloc(1, sizeof(struct directory_tree));//TODO free
        uint depth = 0;
        enum recursive_state
            state =
            enter_cursor_recursive(&new_anchor, new_dir, &args_struct->dir, args_struct->cursor, &depth, 1, args);
        switch (state)
        {
            case STATE_DIR:
            case STATE_FILE:
                break;
            default:
                errno = ERANGE;
        }
    }
    if (c == 'm' || c == 'M')
    {
        args_struct->active = 0;
        args_struct->write_segment_args->active = 1;

        //render_writeable_segment(args_struct->write_segment_args);
        process_after_key_list *element = calloc(1, sizeof(process_after_key_list));
        element->args = args_struct->write_segment_args;
        element->process_after_key = render_writeable_segment_self_delete;
        element->free_args = NULL;
        append_processing(process_after_key_list, general_after_key_funcs, element)

        args_struct->instruction_args->str =
            string_create_from_fcharp("ctrl+e - exit  ctrl+f - filesystem  ctrl+x - save");
        render_static_segment(args_struct->instruction_args);
    }
    if (c == 'b' || c == 'B')
    {

    }

    render_filesystem_segment(args);
}

struct filesystem_segment_params *start_filesystem_segment(file_system_anchor anchor,
                                                           urectangle screen_region,
                                                           void (*changer_function)(void *element, struct winsize w),
                                                           filesystem_color_scheme color_scheme,
                                                           char *prefix)
{
    //write_log(DEBUG, "start filesystem segment");
    struct filesystem_segment_params *args = calloc(1, sizeof(struct filesystem_segment_params));
    args->screen_region = screen_region;
    args->color_scheme = color_scheme;
    args->vertical_shift = 0;
    args->horizontal_shift = 0;
    args->cursor = 1;
    args->print_name = 0;
    args->current_outermost_dir = anchor;
    args->dir.dirs = array_voidp_create(free_tree_node_voidp);
    //write_log(DEBUG, "start filesystem array void created");
    args->dir.files = string_array_create();
    args->prefix = prefix;
    files_dirs_from_directory content = system_anchor_get_dir_content(anchor);
    //write_log(DEBUG, "start filesystem got content");

    for (int i = 0; i < content.dirs.size; i++)
    {
        //write_log(DEBUG, "c1 iter %d", i);
        struct tree_node *node = calloc(1, sizeof(struct tree_node));
        node->dir_state = CLOSED;
        node->closed_dir = &content.dirs.elements[i];
        array_voidp_push(&args->dir.dirs, node);
        //write_log(DEBUG, "c1 end iter %d", i);
    }
    for (int i = 0; i < content.files.size; i++)
    {
        //write_log(DEBUG, "c2 iter %d", i);
        string_array_push(&args->dir.files, content.files.elements[i]);
        //write_log(DEBUG, "c2 iter %d", i);
    }
    //write_log(DEBUG, "pre get name, errno %d", errno);
    args->dir.name = system_anchor_get_directory_name(&anchor);//
    //write_log(DEBUG, "start filesystem args done");

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
//    write_log(DEBUG, "append processing - ok");

    registration_for_window_size_update(args, changer_function);
    //write_log(DEBUG, "start filesystem registration done");
//    write_log(DEBUG, "registration for update - ok");
    //write_log(DEBUG, "start filesystem pre-render");
    render_filesystem_segment(args);

    return args;
}

void render_static_segment(void *args)
{
    struct static_params *args_struct = args;
    printf("%s", args_struct->color.line);
    print_segment_plaintext_shifted(args_struct->str.line, args_struct->screen_region, 0);
    color_to_default();
}

struct static_params *start_static_segment(string str,
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
    return args;
}

void render_file_name_segment(void *args)
{
    struct file_name_params *args_struct = args;
    uint width = args_struct->screen_region.col_end - args_struct->screen_region.col_start;
    if (args_struct->screen_region.row_start != args_struct->screen_region.row_end)
    {
        terminal_goto(args_struct->screen_region.row_start, args_struct->screen_region.col_start)
        printf("%s", args_struct->anchor_color.line);
        string full_path = system_anchor_get_full_path(args_struct->anchor);
        char *line_anchor = full_path.line, *line_name = args_struct->str.line;
        uint j = width + args_struct->shift;
        while (*line_anchor != '\0' && j)
        {
            if (j <= width)
                putchar(*line_anchor);
            line_anchor++;
            j--;
        }
        if (j)
        {
            if (j <= width)
                putchar('/');
            j--;
        }
        printf("%s", args_struct->name_color.line);
        while (*line_name != '\0' && j)
        {
            if (j <= width)
                putchar(*line_name);
            line_name++;
            j--;
        }
        while (j)
        {
            if (j <= width)
                putchar(' ');
            j--;
        }

        //free_string(full_path);

        for (uint i = 1; i < args_struct->screen_region.row_end - args_struct->screen_region.row_start; i++)
        {
            terminal_goto(args_struct->screen_region.row_start + i, args_struct->screen_region.col_start)
            for (j = width; j--;)
            {
                putchar(' ');
            }
        }
    }
    color_to_default();
    if (args_struct->active)
    {
        terminal_goto(args_struct->screen_region.row_start,
                      args_struct->screen_region.col_start + args_struct->cursor - args_struct->shift);
    }
}

void process_arrow_file_name(ARROW arrow, void *args)
{
    struct file_name_params *args_struct = args;
    if (!args_struct->active)
    {
        return;
    }
    if (arrow == LEFT)
    {
        if (args_struct->cursor)
        {
            args_struct->cursor--;
            if (args_struct->cursor < args_struct->shift)
                args_struct->shift = args_struct->cursor;
        }
    }
    else if (arrow == RIGHT)
    {
        string full_path = system_anchor_get_full_path(args_struct->anchor);
        uint len = strlen(full_path.line) + strlen(args_struct->str.line) + 1;
        //free_string(full_path);
        if (args_struct->cursor < len)
        {
            args_struct->cursor++;
            if (args_struct->cursor
                > args_struct->shift + args_struct->screen_region.col_end - args_struct->screen_region.col_start - 1)
            {
                args_struct->shift =
                    args_struct->cursor - args_struct->screen_region.col_end + args_struct->screen_region.col_start + 1;
            }
        }
    }
    render_file_name_segment(args);
}

void process_char_file_name(char c, void *args)
{
    struct file_name_params *args_struct = args;
    if (!args_struct->active)
    {
        return;
    }
    if (c == '\n')
    {
        //args_struct->write_args->active = 1;
        //render_writeable_segment(args_struct->write_args);
        process_after_key_list *element = calloc(1, sizeof(process_after_key_list));
        element->args = args_struct->write_args;
        element->process_after_key = render_writeable_segment_self_delete;
        element->free_args = NULL;
        append_processing(process_after_key_list, general_after_key_funcs, element)

        process_after_key_list *element2 = calloc(1, sizeof(process_after_key_list));
        element2->args = args_struct->write_args;
        element2->process_after_key = activate_writeable_segment_self_delete;
        element2->free_args = NULL;
        append_processing(process_after_key_list, general_after_key_funcs, element2)

        args_struct->instruction_args->str =
            string_create_from_fcharp("ctrl+e - exit  ctrl+f - filesystem  ctrl+x - save");
        render_static_segment(args_struct->instruction_args);

        args_struct->active = 0;
        return;
    }
    string full_path = system_anchor_get_full_path(args_struct->anchor);
    uint pre_len = strlen(full_path.line) + 1;
    //free_string(full_path);
    if (args_struct->cursor >= pre_len)
    {
        if (c == DEL)
        {
            if (args_struct->cursor > pre_len)
            {
                args_struct->cursor--;
                if (args_struct->cursor < args_struct->shift)
                    args_struct->shift = args_struct->cursor;
                delete_from_string_multiline(&args_struct->str, 0, args_struct->cursor - pre_len);
            }
        }
        else
        {
            insert_into_string_multiline(&args_struct->str, c, 0, args_struct->cursor - pre_len);
            args_struct->cursor++;

            //not sure:
            if (args_struct->cursor
                > args_struct->shift + args_struct->screen_region.col_end - args_struct->screen_region.col_start - 1)
            {
                args_struct->shift =
                    args_struct->cursor - args_struct->screen_region.col_end + args_struct->screen_region.col_start + 1;
            }
        }
    }

    render_file_name_segment(args);
}

struct file_name_params *start_file_name_segment(file_system_anchor anchor, string str,
                                                 COLOR color_anchor,
                                                 COLOR color_name,
                                                 urectangle screen_region,
                                                 void (*changer_function)(void *element, struct winsize w))
{
    write_log(DEBUG, "start file name started");
    struct file_name_params *args = calloc(1, sizeof(struct file_name_params));
    args->anchor = anchor;
    args->str = str;
    args->anchor_color = color_anchor;
    args->name_color = color_name;
    args->screen_region = screen_region;
    args->cursor = 0;
    args->shift = 0;

    process_arrow_func_list *list_element = calloc(1, sizeof(process_arrow_func_list));
    list_element->next = NULL;
    list_element->args = args;
    list_element->process_arrow = process_arrow_file_name;
    append_processing(process_arrow_func_list, general_arrow_process_funcs, list_element)

    process_char_func_list *list_element_char = calloc(1, sizeof(process_char_func_list));
    list_element_char->args = args;
    list_element_char->next = NULL;
    list_element_char->process_char = process_char_file_name;
    append_processing(process_char_func_list, general_char_process_funcs, list_element_char)

    registration_for_window_size_update(args, changer_function);
    write_log(DEBUG, "start file name pre-render");
    render_file_name_segment(args);
    write_log(DEBUG, "start file name ended");
    return args;
}