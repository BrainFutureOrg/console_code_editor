#include <stdio.h>
#include <errno.h>
#include "prj_types/String_struct.h"
#include <stdlib.h>
#include <bits/time.h>
#include <time.h>
#include "test/string_tests.h"
#include "test/test_funcs.h"

int main(int argc, char **argv)
{

    apply_string_tests();
    print_test_success();
//    string str = string_create_from_fcharp("%s, %d, %.3f, %%",  "Hello, world?", 2, 3.1415926535);
//    printf("%s\n",str.line);
//    free_string(str);
    if (errno)
    {
        printf("Fuck!\n");
    }
/*
    void speed_test();
    speed_test();*/

    return 0;
}

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
