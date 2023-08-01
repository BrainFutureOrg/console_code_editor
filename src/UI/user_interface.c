//
// Created by maximus on 06.07.23.
//

#include <stdio.h>
#include "user_interface.h"
#include "../colors.h"
#include "../IO/segments.h"
#include "../IO/terminal_io.h"
#include <sys/ioctl.h>
#include <signal.h>
#include "../file_system/file_system_work.h"
#include "../loging/log.h"

void print_logo()
{
    color_to_rgb_foreground(200, 150, 50);
    color_to_rgb_background(0, 0, 0);

    printf("Text");
    color_inverse();
    printf("Maniac\n");
    color_to_default();
}

void ctrl_e_stop_input(char c, void *args)
{
    if (c == CTRL_('E'))
    {
        finish_terminal_input();
        color_to_default();
        terminal_erase_screen;
        terminal_goto(1, 1)
    }
}

#define changer_filesystem_type   struct filesystem_segment_params
#define changer_file_name_type    struct file_name_params
#define changer_instructions_type struct static_params
#define changer_writeable_type    struct write_segment_params

#define changer_filesystem_render_func   render_filesystem_segment
#define changer_file_name_render_func    render_file_name_segment
#define changer_instructions_render_func render_static_segment
#define changer_writeable_render_func    render_writeable_segment

#define hor_size_percent_filesystem  0.2
#define hor_size_percent_writeable   (1 - size_percent_filesystem)
#define hor_size_percent_instruction 1
#define hor_size_percent_file_name   1

#define vert_size_instruction 3
#define vert_size_file_name   2

#define min_hor_size_filesystem 100

#define min_vert_size_instruction  30
#define min_vert_size_file_name    30

void changer_window_function_filesystem(void *element, struct winsize w)
{
    urectangle new_size;
    if (w.ws_col > min_hor_size_filesystem)
    {
        new_size.col_start = 0 + 1;
        new_size.row_start = 0 + 1;
        new_size.col_end = (uint)(w.ws_col * hor_size_percent_filesystem) + 1;
        if (w.ws_row > min_vert_size_instruction)
        {
            new_size.row_end = w.ws_row - vert_size_instruction + 1;
        }
        else
        {
            new_size.row_end = w.ws_row + 1;
        }

    }
    else
    {
        new_size.row_end = new_size.col_end = new_size.row_start = new_size.col_start = 1;
    }

    ((changer_filesystem_type *)element)->screen_region = new_size;
    changer_filesystem_render_func(element);
}

void changer_window_function_file_name(void *element, struct winsize w)
{
    urectangle new_size;
    if (w.ws_row > min_vert_size_file_name)
    {
        if (w.ws_col > min_hor_size_filesystem)
        {
            new_size.col_start = (uint)(w.ws_col * hor_size_percent_filesystem) + 1;
        }
        else
        {
            new_size.col_start = 0 + 1;
        }
        new_size.row_start = 0 + 1;
        new_size.col_end = w.ws_col + 1;
        new_size.row_end = vert_size_file_name + 1;
    }
    else
    {
        new_size.row_end = new_size.col_end = new_size.row_start = new_size.col_start = 1;
    }

    ((changer_file_name_type *)element)->screen_region = new_size;
    changer_file_name_render_func(element);
}

void changer_window_function_instructions(void *element, struct winsize w)
{
    urectangle new_size;
    if (w.ws_row > min_vert_size_instruction)
    {
        new_size.col_start = 0 + 1;

        new_size.row_start = w.ws_row - vert_size_instruction + 1;
        new_size.col_end = w.ws_col + 1;
        new_size.row_end = w.ws_row + 1;
    }
    else
    {
        new_size.row_end = new_size.col_end = new_size.row_start = new_size.col_start = 1;
    }

    ((changer_instructions_type *)element)->screen_region = new_size;
    changer_instructions_render_func(element);
}

void changer_window_function_writeable(void *element, struct winsize w)
{
    urectangle new_size;

    if (w.ws_col > min_hor_size_filesystem)
    {
        new_size.col_start = (uint)(w.ws_col * hor_size_percent_filesystem) + 1;
    }
    else
    {
        new_size.col_start = 0 + 1;
    }

    if (w.ws_row > min_vert_size_file_name)
    {
        new_size.row_start = vert_size_file_name + 1;
    }
    else
    {
        new_size.row_start = 0 + 1;
    }
    new_size.col_end = w.ws_col + 1;

    if (w.ws_row > min_vert_size_instruction)
    {
        new_size.row_end = w.ws_row - vert_size_instruction + 1;
    }
    else
    {
        new_size.row_end = w.ws_row + 1;
    }

    ((changer_writeable_type *)element)->screen_region = new_size;
    changer_writeable_render_func(element);
}

void changer_function_empty(void *element, struct winsize w)
{

}

struct writeable_filename_connector
{
    struct write_segment_params *writeable_args;
    struct file_name_params *filename_args;
};

void ctrl_save(char c, void *args)
{
    struct writeable_filename_connector *args_struct = args;
    if (!args_struct->writeable_args->active)
        return;
    if (c == CTRL_('X'))
    {
        //write_log(DEBUG, "ctrl+x");
        anchor_save_file(args_struct->filename_args->anchor,
                         args_struct->filename_args->str.line,
                         *args_struct->writeable_args->str);
    }
}
void start_plaintext_editor_UI_regular(string *str)
{
    terminal_erase_screen;

    string bg_label_anchor = color_create_background_rgb(20, 20, 82);
    string fg_label_anchor = color_create_foreground_rgb(170, 170, 170);
    string_add_string(&bg_label_anchor, fg_label_anchor);
    string bg_label_name = color_create_background_rgb(20, 20, 82);
    string fg_label_name = color_create_foreground_rgb(255, 255, 255);
    string_add_string(&bg_label_name, fg_label_name);
    urectangle label_region = {2, 3, 20, 70};//TODO:free
    string label_str = string_create_from_fcharp("");
    //struct static_params *file_name_args = start_static_segment(label_str,
    //                                                            label_color,
    //                                                            label_region,
    //                                                            changer_window_function_file_name);
    file_system_anchor anchor = system_anchor_init();//TODO:free
    struct file_name_params *file_name_args = start_file_name_segment(anchor, label_str,
                                                                      bg_label_anchor,
                                                                      bg_label_name,
                                                                      label_region,
                                                                      changer_window_function_file_name);

    process_ctrl_func_list *list_element = calloc(1, sizeof(process_ctrl_func_list));
    list_element->next = NULL;
    list_element->args = NULL;
    list_element->process_ctrl_char = ctrl_e_stop_input;
    append_processing(process_ctrl_func_list, general_ctrl_process_funcs, list_element)

    urectangle instructions_region = {20, 22, 5, 70};
    COLOR instructions_color = color_create_background_rgb(20, 20, 82);//TODO free
    string instructions_str = string_create_from_fcharp("ctrl+e - exit   ctrl+f - filesystem  ctrl+x - save");
    struct static_params *instruction_args = start_static_segment(instructions_str,
                                                                  instructions_color,
                                                                  instructions_region,
                                                                  changer_window_function_instructions);

    COLOR writeable_color = color_create_background_rgb(7, 7, 35);
    COLOR writeable_color_highlight = color_create_background_rgb(13, 13, 55);
    urectangle writeable_region = {3, 20, 20, 70};
    struct write_segment_params *write_args = start_write_segment(str,
                                                                  writeable_region,
                                                                  changer_window_function_writeable,
                                                                  writeable_color, writeable_color_highlight);

    urectangle filesystem_region = {2, 20, 5, 20};
    string bg_dir = color_create_background_rgb(12, 12, 60);
    string fg_dir = color_create_foreground_rgb(255, 100, 100);
    string_add_string(&bg_dir, fg_dir);
    string bg_file = color_create_background_rgb(12, 12, 60);
    string fg_file = color_create_foreground_rgb(100, 100, 255);
    string_add_string(&bg_file, fg_file);
    filesystem_color_scheme filesystem_colors = {bg_dir, bg_file, bg_file};
    struct filesystem_segment_params *filesystem_args = start_filesystem_segment(system_anchor_init(),
                                                                                 filesystem_region,
                                                                                 changer_window_function_filesystem,
                                                                                 filesystem_colors,
                                                                                 "  ");

    write_args->active = 1;//
    write_args->filesystem_segment_args = filesystem_args;
    filesystem_args->active = 0;//
    filesystem_args->write_segment_args = write_args;
    filesystem_args->file_name_segment_args = file_name_args;
    file_name_args->write_args = write_args;
    write_args->file_name_args = file_name_args;

    process_ctrl_func_list *list_element_save = calloc(1, sizeof(process_ctrl_func_list));
    list_element_save->next = NULL;
    struct writeable_filename_connector *save_args = calloc(1, sizeof(struct writeable_filename_connector));
    save_args->writeable_args = write_args;
    save_args->filename_args = file_name_args;
    list_element_save->args = save_args;
    list_element_save->process_ctrl_char = ctrl_save;
    append_processing(process_ctrl_func_list, general_ctrl_process_funcs, list_element_save)

    write_args->instruction_args = instruction_args;
    file_name_args->instruction_args = instruction_args;
    filesystem_args->instruction_args = instruction_args;

    write_log(DEBUG, "ui pre-render");
    raise(SIGWINCH);
    write_log(DEBUG, "ui post-render");
    /*read_process_keys(general_arrow_process_funcs,
                      general_char_process_funcs,
                      general_ctrl_process_funcs, general_after_key_funcs);*/
    read_process_keys_global();
    color_to_default();
}
//prototype
/*
#include <sys/ioctl.h>
#include <time.h>
struct screen_size
{
    uint rows, columns;
} old_size;
char check_size = 1;

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do
    {
        res = nanosleep(&ts, &ts);
    }
    while (res && errno == EINTR);

    return res;
}

struct callback
{
    void
    (*callback_func)(void *args);
    void *callback_args;
};
void *check_screen_size(void *func_n_args)
{
    struct screen_size new_size;
    struct winsize w;

    struct callback *my_callback = func_n_args;

    while (check_size)
    {

        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        new_size.rows = w.ws_row;
        new_size.columns = w.ws_col;
        if (not_equals(old_size, new_size))
        {
            render_screens(new_size);
        }
        msleep(100);
    }
    return NULL;
}*/