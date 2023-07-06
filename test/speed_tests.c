//
// Created by maximus on 06.07.23.
//

#include "speed_tests.h"

void speed_test()
{
    int size = 100000;
    char *c = calloc(size, sizeof(char)), *original = c;



/* here, do your time-consuming job */



    clock_t begin = clock();
    for (int i = 0; i < size; i++, c++);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n%lf", time_spent);

    c = original;

    begin = clock();
    for (int i = 0; i++ < size;);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n%lf", time_spent);

    c = original;

    begin = clock();
    for (; (c++) - original < size;);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n%lf", time_spent);

    c = original;

    begin = clock();
    while ((c++) - original < size);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n%lf", time_spent);
}