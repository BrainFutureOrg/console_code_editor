//
// Created by maximus on 24.07.23.
//

#include <sys/ioctl.h>
#include <unistd.h>
#include "signals_redefinition.h"
#include "IO/terminal_io.h"
#include "loging/log.h"
#include <signal.h>
#include <stdlib.h>

struct change_window_size_registrator
{
    void *element;
    void
    (*changer_function)(void *element, struct winsize w);
    struct change_window_size_registrator *next;
} *main_change_window_size_registrator;

void free_change_window_size_registrator()
{
    struct change_window_size_registrator *cwsr = main_change_window_size_registrator;
    while (cwsr != NULL)
    {
        free(cwsr);
        cwsr = cwsr->next;
    }
}

void is_window_changed(int signal)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

//    write_log(DEBUG, "window_changed, rows: %d, columns: %d", w.ws_row, w.ws_col);

    struct change_window_size_registrator *cwsr = main_change_window_size_registrator;
    while (cwsr != NULL)
    {
//        write_log(DEBUG, "window_changed, registrator caused");
        cwsr->changer_function(cwsr->element, w);
        cwsr = cwsr->next;
    }
}
#define free_process_functions(type, func)                   \
        {                                                    \
            type *func_pointer = func;                       \
            while(func_pointer != NULL){                     \
                func_pointer->free_args(func_pointer->args); \
                type *a = func_pointer;                      \
                func_pointer = func_pointer->next;           \
                free(a);                                     \
            }                                                \
        }
void end_program(int signal)
{
    free_process_functions(process_arrow_func_list, general_arrow_process_funcs)
    free_process_functions(process_char_func_list, general_char_process_funcs)
    free_process_functions(process_ctrl_func_list, general_ctrl_process_funcs)
    exit(0);
}

void registration_for_window_size_update(void *element, void (*changer_function)(void *element, struct winsize w))
{
    struct change_window_size_registrator *cwsr = calloc(1, sizeof(struct change_window_size_registrator));
    cwsr->element = element;
    cwsr->changer_function = changer_function;
    cwsr->next = main_change_window_size_registrator;
    main_change_window_size_registrator = cwsr;
}

int catch_signal(int sig, void (*handler)(int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}

void redefine_signals()
{

    catch_signal(SIGINT, end_program);
    catch_signal(SIGWINCH, is_window_changed);
}