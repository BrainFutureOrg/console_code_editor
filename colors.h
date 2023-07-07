//
// Created by maximus on 04.07.23.
//

#ifndef CONSOLE_CODE_EDITOR__COLORS_H
#define CONSOLE_CODE_EDITOR__COLORS_H

#include "prj_types/String_struct.h"
#include "bins.h"

typedef string COLOR;
#define free_color(c) free_string(c)
typedef int COLOR_PARTS;

#define DEFAULT               B(00000000000000000001)
#define BOLD                  B(00000000000000000010)
#define UNDERLINED            B(00000000000000000100)
#define FLASHING              B(00000000000000001000)
#define FOREGROUND_BLACK      B(00000000000000010000)
#define FOREGROUND_RED        B(00000000000000100000)
#define FOREGROUND_GREEN      B(00000000000001000000)
#define FOREGROUND_YELLOW     B(00000000000010000000)
#define FOREGROUND_BLUE       B(00000000000100000000)
#define FOREGROUND_PURPLE     B(00000000001000000000)
#define FOREGROUND_CYAN       B(00000000010000000000)
#define FOREGROUND_WHITE      B(00000000100000000000)
#define BACKGROUND_BLACK      B(00000001000000000000)
#define BACKGROUND_RED        B(00000010000000000000)
#define BACKGROUND_GREEN      B(00000100000000000000)
#define BACKGROUND_YELLOW     B(00001000000000000000)
#define BACKGROUND_BLUE       B(00010000000000000000)
#define BACKGROUND_PURPLE     B(00100000000000000000)
#define BACKGROUND_CYAN       B(01000000000000000000)
#define BACKGROUND_WHITE      B(10000000000000000000)

COLOR create_color(COLOR_PARTS sum_of_parts);

COLOR color_create_foreground_rgb(unsigned char r, unsigned char g, unsigned char b);
COLOR color_create_background_rgb(unsigned char r, unsigned char g, unsigned char b);

void color_to(COLOR_PARTS sum_of_parts);
void color_to_rgb_foreground(unsigned char r, unsigned char g, unsigned char b);
void color_to_rgb_background(unsigned char r, unsigned char g, unsigned char b);
void color_to_default();
void color_inverse();

void color_printf(COLOR color, char *format, ...);
void color_from_parts_printf(COLOR_PARTS sum_of_parts, char *format, ...);

#endif //CONSOLE_CODE_EDITOR__COLORS_H
