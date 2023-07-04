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
            printf(RED_B_COLOR "ERROR: E2BIG\n" DEFAULT_COLOR);
            break;
        case EINVAL:
            printf(RED_B_COLOR "ERROR: EINVAL\n" DEFAULT_COLOR);
            break;
        default:
            printf(RED_B_COLOR "ERROR\n" DEFAULT_COLOR);
            break;
    }
}