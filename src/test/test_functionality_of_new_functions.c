//
// Created by maximus on 14.07.23.
//

#include <unistd.h>
#include <stdio.h>
#include "test_functionality_of_new_functions.h"
#include "../IO/DAO.h"
#include "../IO/terminal_io.h"
#include "../colors.h"
#include "../IO/segments.h"

void test_console_games_functions(int sleep_time)
{
    string file_text = read_file("example.txt");
    printf("%s", file_text.line);

    sleep(sleep_time);
    terminal_go_right_x(10);
    color_from_parts_printf(FLASHING | FOREGROUND_WHITE, "10 to right\n");
    sleep(sleep_time);
    terminal_go_down_x(3);
    color_from_parts_printf(UNDERLINED | FOREGROUND_BLUE, "3 to down\n");
    sleep(sleep_time);
    terminal_goto_xy(15, 10);
    color_from_parts_printf(BOLD | FOREGROUND_CYAN, "I'm in 15 10\n");
    sleep(sleep_time);
    terminal_go_home;
    color_from_parts_printf(UNDERLINED | FOREGROUND_BLUE, "I`m in home\n");
    sleep(sleep_time);

    terminal_erase_display;
    color_from_parts_printf(FOREGROUND_PURPLE, "ERASE ALL\n");
    sleep(sleep_time);

    printf("%s", file_text.line);
    sleep(sleep_time);

    terminal_go_up_x(5);
    color_from_parts_printf(UNDERLINED | BOLD | FOREGROUND_YELLOW, "5 to up\n");
    sleep(sleep_time);

    terminal_erase_line;
    color_from_parts_printf(BOLD | FOREGROUND_GREEN, "Erase line\n");
    sleep(sleep_time);

    terminal_erase_from_cursor_to_end_screen;
    color_from_parts_printf(BOLD | UNDERLINED | FOREGROUND_RED, "Попіл до попілу, прах до праху\n");
    sleep(sleep_time);

    terminal_erase_from_cursor_to_beginning_screen;
    color_from_parts_printf(BOLD | FOREGROUND_RED, "Щезне все\n");
    sleep(sleep_time);

    free_string(file_text);
}

void test_interesting_picture()
{
    terminal_erase_display;
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 170; j++)
        {
            int color = (i - 20) * (i - 20) / 5 - (j - 20) * (j - 20) / 10;
            color = (color > 0 ? color : 0) < 255 ? color : 255;
            color_to_rgb_background(0, color, 0);
            printf(" ");
        }
        printf("\n");
    }
    color_to(DEFAULT);
}

void test_cursor_functions()
{
    string file_text = read_file("example.txt");
    cursor start_pos = cursor_get_cursor_position();
    printf("%s", file_text.line);

    cursor_printf_at_position(start_pos, "some_text");
    sleep(2);
    printf("text in end\n");
    sleep(2);
    cursor_printf_at_position(start_pos, "another_text");
    sleep(2);
    printf("one more text in end\n");
    sleep(2);

    free_string(file_text);
}

void test_global()
{
    static char entered = 0;
    if (!entered)
    {
        process_arrow_func_list dummy_list;
        general_arrow_process_funcs = &dummy_list;
    }
    printf("global list is %s null, iteration %d", general_arrow_process_funcs != NULL ? "not" : "", entered);
    if (!entered)
    {
        entered++;
        test_global();
    }
}

