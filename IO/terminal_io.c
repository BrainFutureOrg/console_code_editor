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
    else if(c>27)
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
    new_settings.c_lflag &= ~(ICANON | ECHO|ISIG);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
    char c;
    while ((c=read_key()) != CTRL_E)if(c<27)process_ctrl_char(c);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
    printf("\nterminal input ended\n");
}
void process_ctrl_char(char c){
    switch (c) {
        case CTRL_C:
            printf("[CTRL+C]");
            break;
        case CTRL_V:
            printf("[CTRL+V]");
            break;
        case CTRL_O:
            printf("[CTRL+O]");
            break;
        case CTRL_E:
            printf("[CTRL+E]");
            break;
        case CTRL_G:
            printf("[CTRL+G]");
            break;
        case CTRL_H:
            printf("[CTRL+H]");
            break;
        case CTRL_X:
            printf("[CTRL+X]");
            break;
        case CTRL_Z:
            printf("[CTRL+Z]");
            break;
        default:
            printf("[CTRL+WHAT]");
    }
}