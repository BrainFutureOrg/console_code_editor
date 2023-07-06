//
// Created by maximus on 04.07.23.
//
#include "colors.h"
#include <stdio.h>
#include "prj_error.h"
void print_error()
{
    switch errno
    {
        case 0:
            break;
        case E2BIG:
            color_from_parts_printf(BOLD | FOREGROUND_RED, "ERROR: E2BIG\n");
            break;
        case EINVAL:
            color_from_parts_printf(BOLD | FOREGROUND_RED, "ERROR: EINVAL\n");
            break;
        default:
            color_from_parts_printf(BOLD | FOREGROUND_RED, "ERROR\n");
            break;
    }
}