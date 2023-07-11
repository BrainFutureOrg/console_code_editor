//
// Created by kosenko on 06.07.23.
//
#include <stdio.h>
#include "terminal_io.h"
#include <unistd.h>
#include <termios.h>
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