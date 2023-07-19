//
// Created by kosenko on 06.07.23.
//
#include <stdio.h>
#include "terminal_io.h"
#include <unistd.h>
#include <termios.h>
#include <malloc.h>
#include <stdarg.h>
process_arrow_func_list* general_arrow_process_funcs=NULL;
process_char_func_list* general_char_process_funcs=NULL;
process_ctrl_func_list* general_ctrl_process_funcs=NULL;
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
void read_process_key(process_arrow_func_list* process_arrow_funcs, process_char_func_list* process_char_funcs, process_ctrl_func_list* process_ctrl_funcs){
    char c;
    if((c=getchar())>27){
        while(process_char_funcs!=NULL){
            process_char_funcs->process_char(c);
            process_char_funcs=process_char_funcs->next;
        }
    }else if(c<27){
        while(process_ctrl_funcs!=NULL){
            process_ctrl_funcs->process_ctrl_char(c);
            process_ctrl_funcs=process_ctrl_funcs->next;
        }
    }else{
        getchar();
        ARROW arrow=getchar()-'A';
        while(process_arrow_funcs!=NULL){
            process_arrow_funcs->process_arrow(arrow);
            process_arrow_funcs=process_arrow_funcs->next;
        }
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
char end_terminal_input=0;
void ctrl_e_end(char c){
    end_terminal_input=c==CTRL_('E');
}
void read_process_keys(process_arrow_func_list* process_arrow_funcs, process_char_func_list* process_char_funcs, process_ctrl_func_list* process_ctrl_funcs){
    struct termios old_settings, new_settings;
    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO | ISIG);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
    while (!end_terminal_input)
    {
        read_process_key(process_arrow_funcs, process_char_funcs, process_ctrl_funcs);
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
    cursor result;
    result.position = malloc(2);

    struct termios old_settings, new_settings;
    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO | ISIG);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

//    terminal_get_cursor_position;
//    read(0, result.position, 2);
    for (int i = 0; i < 2; i++)
    {
        result.position[i] = getc(stdout);//getc(STDIN_FILENO);
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
    free_cursor(start_pos);
}

void cursor_print_position(cursor my_cursor)
{
    printf("position is %s\n", my_cursor.position);
}

void free_cursor(cursor my_cursor)
{
    free(my_cursor.position);
}