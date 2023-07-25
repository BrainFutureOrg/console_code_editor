//
// Created by maximus on 06.07.23.
//

#include <stdio.h>
#include "user_interface.h"
#include "../colors.h"
#include "../IO/segments.h"
#include "../IO/terminal_io.h"
#include <sys/ioctl.h>
#include "../file_system/file_system_work.h"

void print_logo()
{
    color_to_rgb_foreground(200, 150, 50);
    color_to_rgb_background(0, 0, 0);

    printf("C Code ");
    color_inverse();
    printf("Editor\n");
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
void changer_function_empty(void *element, struct winsize w)
{

}
void start_plaintext_editor_UI_regular(string *str)
{
    terminal_erase_screen;

    COLOR label_color = color_create_background_rgb(15, 15, 70);//TODO: free
    urectangle label_region = {2, 3, 20, 70};//TODO:free
    string label_str = string_create_from_fcharp("[text name will be here]");
    //start_static_segment(label_str, label_color, label_region //TODO: for Maximus: finish

    process_ctrl_func_list *list_element = calloc(1, sizeof(process_ctrl_func_list));
    list_element->next = NULL;
    list_element->args = NULL;
    list_element->process_ctrl_char = ctrl_e_stop_input;
    append_processing(process_ctrl_func_list, general_ctrl_process_funcs, list_element)

    urectangle instructions_region = {20, 22, 5, 70};
    COLOR instructions_color = color_create_background_rgb(15, 15, 50);//TODO free
    string instructions_str = string_create_from_fcharp("ctrl+e - exit");
    //start_static_segment(instructions_str, instructions_color, instructions_region, //TODO: for Maximus: finish

    COLOR writeable_color = color_create_background_rgb(10, 10, 70);
    urectangle writeable_region = {3, 20, 20, 70};
    //start_write_segment(str, writeable_region, changer_function_empty, //TODO: for Maximus: finish

    urectangle filesystem_region = {3, 20, 5, 20};
    //start_filesystem_segment(system_anchor_init(), filesystem_region, //TODO: for Maximus: finish

    read_process_keys(general_arrow_process_funcs,
                      general_char_process_funcs,
                      general_ctrl_process_funcs);
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