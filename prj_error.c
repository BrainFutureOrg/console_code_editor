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
        case EACCES:
            color_from_parts_printf(BOLD | FOREGROUND_RED, "ERROR: reading response!\n");
            break;
        case EFAULT:
            color_from_parts_printf(BOLD | FOREGROUND_RED, "ERROR: i<2\n");
            break;
        case EKEYEXPIRED:
            color_from_parts_printf(BOLD | FOREGROUND_RED, "ERROR: invalid argument\n");
            break;
        case EBADF:
            color_from_parts_printf(BOLD | FOREGROUND_RED, "ERROR: directory wasn't opened successfully\n");
            break;
        default:
            color_from_parts_printf(BOLD | FOREGROUND_RED, "ERROR\n");
            break;
    }
}