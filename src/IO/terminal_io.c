//
// Created by kosenko on 06.07.23.
//
#include <errno.h>
#include "terminal_io.h"
#include <stdlib.h>
#include "../loging/log.h"

char read_key()
{
    char c;
    if ((c = getchar()) == 27)
    {
        getchar();
        switch (getchar())
        {
            case 'A':
                printf("[UP_ARROW]");
                break;
            case 'B':
                printf("[DOWN_ARROW]");
                break;
            case 'C':
                printf("[RIGHT_ARROW]");
                break;
            case 'D':
                printf("[LEFT_ARROW]");
                break;
            default:
                printf("[WHAT]");

        }
    }
    else if (c > 27)
    {
        printf("%c", c);
    }
    return c;
}
void read_process_key(process_arrow_func_list *process_arrow_funcs,
                      process_char_func_list *process_char_funcs,
                      process_ctrl_func_list *process_ctrl_funcs, process_after_key_list *process_after_key_funcs)
{
    char c;
    if ((c = getchar()) > 27 || c == '\n')
    {
        while (process_char_funcs != NULL)
        {
            process_char_funcs->process_char(c, process_char_funcs->args);
            process_char_funcs = process_char_funcs->next;
        }
    }
    else if (c < 27)
    {
        while (process_ctrl_funcs != NULL)
        {
            process_ctrl_funcs->process_ctrl_char(c, process_ctrl_funcs->args);
            process_ctrl_funcs = process_ctrl_funcs->next;
        }
    }
    else
    {
        getchar();
        ARROW arrow = getchar() - 'A';
        while (process_arrow_funcs != NULL)
        {
            process_arrow_funcs->process_arrow(arrow, process_arrow_funcs->args);
            process_arrow_funcs = process_arrow_funcs->next;
        }
    }
    write_log(DEBUG, "process %s general", process_after_key_funcs == general_after_key_funcs ? "==" : "!=");
    write_log(DEBUG, "general after arrow %s NULL", process_after_key_funcs == NULL ? "==" : "!=");
    while (process_after_key_funcs != NULL)
    {
        write_log(DEBUG, "process after key iter");
        process_after_key_funcs->process_after_key(process_after_key_funcs->args);
        process_after_key_funcs = process_after_key_funcs->next;
    }
}
void read_process_key_global()
{
    process_arrow_func_list *process_arrow_funcs = general_arrow_process_funcs;
    process_char_func_list *process_char_funcs = general_char_process_funcs;
    process_ctrl_func_list *process_ctrl_funcs = general_ctrl_process_funcs;
    char c;
    if ((c = getchar()) > 27 || c == '\n')
    {
        while (process_char_funcs != NULL)
        {
            process_char_funcs->process_char(c, process_char_funcs->args);
            process_char_funcs = process_char_funcs->next;
        }
    }
    else if (c < 27)
    {
        while (process_ctrl_funcs != NULL)
        {
            process_ctrl_funcs->process_ctrl_char(c, process_ctrl_funcs->args);
            process_ctrl_funcs = process_ctrl_funcs->next;
        }
    }
    else
    {
        getchar();
        ARROW arrow = getchar() - 'A';
        while (process_arrow_funcs != NULL)
        {
            process_arrow_funcs->process_arrow(arrow, process_arrow_funcs->args);
            process_arrow_funcs = process_arrow_funcs->next;
        }
    }
    process_after_key_list *process_after_key_funcs = general_after_key_funcs;
    write_log(DEBUG, "process %s general", process_after_key_funcs == general_after_key_funcs ? "==" : "!=");
    write_log(DEBUG, "general after arrow %s NULL", process_after_key_funcs == NULL ? "==" : "!=");
    while (process_after_key_funcs != NULL)
    {
        write_log(DEBUG, "process after key iter");
        process_after_key_funcs->process_after_key(process_after_key_funcs->args);
        process_after_key_funcs = process_after_key_funcs->next;
    }
}
void process_ctrl_char(char);
void read_keys()
{
    printf("\nterminal input started, ctrl+e to exit\n");
    struct termios old_settings, new_settings;
    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO | ISIG);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
    char c;
    while ((c = read_key()) != CTRL_('E'))
    {
        if (c < 27)
        {
            process_ctrl_char(c);
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
    printf("\nterminal input ended\n");
}

char end_terminal_input = 0;
process_arrow_func_list *general_arrow_process_funcs = NULL;
process_char_func_list *general_char_process_funcs = NULL;
process_ctrl_func_list *general_ctrl_process_funcs = NULL;
process_after_key_list *general_after_key_funcs = NULL;
/*void ctrl_e_end(char c)
{
    end_terminal_input = c == CTRL_('E');
}*/

void finish_terminal_input()
{
    end_terminal_input = 1;
}
void read_process_keys(process_arrow_func_list *process_arrow_funcs,
                       process_char_func_list *process_char_funcs,
                       process_ctrl_func_list *process_ctrl_funcs, process_after_key_list *process_after_key_funcs)
{
    struct termios old_settings, new_settings;
    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO | ISIG);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
    while (!end_terminal_input)
    {
        read_process_key(process_arrow_funcs, process_char_funcs, process_ctrl_funcs, process_after_key_funcs);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
}
void read_process_keys_global()
{
    struct termios old_settings, new_settings;
    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO | ISIG);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
    while (!end_terminal_input)
    {
        read_process_key_global();
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
}
void process_ctrl_char(char c)
{
    if (IS_CTRL(c))
    {
        printf("[CTRL+%c]", TO_LETTER(c));
    }
    else
    {
        printf("What a hell? %d, %c, %c", c, c, TO_LETTER(c));
    }
}

cursor cursor_get_cursor_position()
{
    cursor result = {0, 0};

    char buf[30] = {0};
    int ret, i, pow;
    char ch;

    struct termios old_settings, new_settings;

    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

    terminal_get_cursor_position;

    for (i = 0, ch = 0; ch != 'R'; i++)
    {
        ret = read(0, &ch, 1);
        if (!ret)
        {
            tcsetattr(0, TCSANOW, &old_settings);
            errno = EACCES;
            return result;
        }
        buf[i] = ch;
//        printf("buf[%d]: \t%c \t%d\n", i, ch, ch);
    }

    if (i < 2)
    {
        tcsetattr(0, TCSANOW, &old_settings);
        errno = EFAULT;
        return result;
    }

    for (i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
    {
        result.x = result.x + (buf[i] - '0') * pow;
    }

    for (i--, pow = 1; buf[i] != '['; i--, pow *= 10)
    {
        result.y = result.y + (buf[i] - '0') * pow;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
    return result;
}

void cursor_printf_at_position(cursor my_cursor, char *format, ...)
{
    cursor start_pos = cursor_get_cursor_position();
    cursor_go_to_position(my_cursor);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    cursor_go_to_position(start_pos);
}

void cursor_print_position(cursor my_cursor)
{
    printf("position is x = %d, y = %d\n", my_cursor.x, my_cursor.y);
}

void delete_from_after_key_list_global(void(*func)(void *), void *args)
{
    process_after_key_list *list = general_after_key_funcs;
    if (list != NULL)
    {
        if (list->args == args && list->process_after_key == func)
        {
            process_after_key_list *temp = list->next;
            if (list->free_args != NULL)
            {
                list->free_args(list->args);
            }
            //free(list);
            general_after_key_funcs = temp;
        }
        else
            while (list->next != NULL)
            {
                if (list->next->args == args && list->next->process_after_key == func)
                {
                    process_after_key_list *temp = list->next->next;
                    if (list->next->free_args != NULL)
                    {
                        list->next->free_args(list->next->args);
                    }
                    //free(list->next);
                    list->next = temp;
                }
            }
    }
}