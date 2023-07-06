//
// Created by kosenko on 06.07.23.
//
#include <stdio.h>
#include "terminal_io.h"
#include <unistd.h>
#include <termios.h>
char read_key(){
    char c;
    if((c=getchar())==27){
        getchar();
        switch (getchar()) {
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
    }else{
        printf("%c", c);
    }
    return c;
}

void read_keys(){
    struct termios old_settings, new_settings;
    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings=old_settings;
    new_settings.c_lflag&=~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
    while(read_key()!='\n');
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
    printf("\nterminal input ended");
}