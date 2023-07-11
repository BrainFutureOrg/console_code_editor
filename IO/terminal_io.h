//
// Created by kosenko on 06.07.23.
//

#ifndef CONSOLE_CODE_EDITOR_TERMINAL_IO_H
#define CONSOLE_CODE_EDITOR_TERMINAL_IO_H
#define preA ('A' - 1)
#define IS_CTRL(l)   !(l > 'Z' - preA || l < 1)
#define CTRL_(l)     (l - preA)
#define TO_LETTER(l) (l + preA)

char read_key();
void read_keys();
#endif //CONSOLE_CODE_EDITOR_TERMINAL_IO_H
