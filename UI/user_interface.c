//
// Created by maximus on 06.07.23.
//

#include <stdio.h>
#include "user_interface.h"
#include "../colors.h"
#include "../IO/segments.h"
#include "../IO/terminal_io.h"

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

void start_plaintext_editor_UI_regular(string *str)
{
    terminal_erase_screen;

    color_to_rgb_background(15, 15, 70);

    urectangle label_region = {2, 3, 5, 70};
    print_segment_plaintext_shifted("           [text name will be here]", label_region, 0);

    process_ctrl_func_list list_element;
    list_element.next = NULL;
    list_element.args = NULL;
    list_element.process_ctrl_char = ctrl_e_stop_input;
    append_processing(process_ctrl_func_list, general_ctrl_process_funcs, &list_element)

    urectangle instructions_region = {20, 22, 5, 70};
    print_segment_plaintext_shifted("ctrl+e - exit", instructions_region, 0);

    color_to_rgb_background(10, 10, 50);
    urectangle writeable_region = {3, 20, 5, 70};
    start_write_segment(str, writeable_region);
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