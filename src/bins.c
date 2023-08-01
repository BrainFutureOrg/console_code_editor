//
// Created by maximus on 06.07.23.
//

#include "bins.h"

int S_to_binary_(const char *s)
{
    int i = 0;
    while (*s)
    {
        i <<= 1;
        i += *s++ - '0';
    }
    return i;
}