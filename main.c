#include <stdio.h>
#include <errno.h>
#include "prj_types/String_struct.h"
#include <stdlib.h>
#include <bits/time.h>
#include <time.h>
#include "test/string_tests.h"
#include "test/test_funcs.h"
#include "IO/terminal_io.h"
#include "colors.h"
#define program_logo printf("\033[48;2;0;0;0m\033[38;2;200;150;50mC Code \033[48;2;200;150;50m\033[38;2;0;0;0mEditor\n" DEFAULT_COLOR)

int main(int argc, char **argv)
{
    program_logo;
    apply_string_tests();//no delete
    print_test_success();// no delete

    //system("/bin/stty cooked");
//    string str = string_create_from_fcharp("%s, %d, %.3f, %%",  "Hello, world?", 2, 3.1415926535);
//    printf("%s\n",str.line);
//    free_string(str);

    read_keys();//no delete
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
