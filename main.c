#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "test/string_tests.h"
#include "test/test_funcs.h"
#include "IO/terminal_io.h"
#include "UI/user_interface.h"
#include "colors.h"
#include "IO/DAO.h"
#include "IO/segments.h"
//#define program_logo printf("\033[48;2;0;0;0m\033[38;2;200;150;50mC Code \033[48;2;200;150;50m\033[38;2;0;0;0mEditor\n" DEFAULT_COLOR)

int main(int argc, char **argv)
{
    print_logo();
//    apply_string_tests();//no delete
//    print_test_success();// no delete

    string file_text = read_file("example.txt");
    save_file(file_text, "smth.txt");
    printf("%s", file_text.line);

    sleep(2);
    terminal_go_right_x(10);
    color_from_parts_printf(FLASHING | FOREGROUND_WHITE, "10 to right\n");
    sleep(2);
    terminal_go_down_x(3);
    color_from_parts_printf(UNDERLINED | FOREGROUND_BLUE, "3 to down\n");
    sleep(2);
    terminal_goto_xy(15, 10);
    color_from_parts_printf(BOLD | FOREGROUND_CYAN, "I'm in 15 10\n");
    sleep(2);
    terminal_go_home;
    color_from_parts_printf(UNDERLINED | FOREGROUND_BLUE, "I`m in home\n");
    sleep(2);

    terminal_erase_display;
    color_from_parts_printf(FOREGROUND_PURPLE, "ERASE ALL\n");
    sleep(2);

    printf("%s", file_text.line);
    sleep(2);

    terminal_go_up_x(5);
    color_from_parts_printf(UNDERLINED | BOLD | FOREGROUND_YELLOW, "5 to up\n");
    sleep(2);

    terminal_erase_line;
    color_from_parts_printf(BOLD | FOREGROUND_GREEN, "Erase line\n");
    sleep(2);

    terminal_erase_from_cursor_to_end_screen;
    color_from_parts_printf(BOLD | UNDERLINED | FOREGROUND_RED, "Попіл до попілу, прах до праху\n");
    sleep(2);

    terminal_erase_from_cursor_to_beginning_screen;
    color_from_parts_printf(BOLD | FOREGROUND_RED, "Щезне все\n");
    sleep(2);
    //new
//    printf("\033[H\033[J");//clear
//    //printf("%s", file_text.line);
//    //string str= string_create_from_fcharp("aaa\nbbb\nnnnnnnnnnnnnnnnnnnn");
//    urectangle region={4,12,4,40};
//    COLOR color= color_create_background_rgb(0,0,50);
//    print_string_segment_primitive(file_text, color, region);
//    //free_string(str);
//    free_string(color);
//
//    urectangle region2={13,20,4,40};
//    COLOR color2= color_create_background_rgb(0,0,80);
//    string instructions_string= string_create_from_fcharp("ctrl+c copy\nctrl+v paste\nctrl+z cancel\nctrl+o open");
//    print_string_segment_primitive(instructions_string, color2, region2);
//    free_string(instructions_string);
//    free_string(color);

    //end new



    free_string(file_text);
    //printf("\e[2J");

    //printf("\033[H\033[J");//clear

    /*printf("\033[%d;%dHa", 8, 8);//y;x
    color_to_rgb_foreground(255,0,0);
    color_to_rgb_background(0,255,0);
    printf("\033[%d;%dHb", 8, 16);//y;x
    //color_to_rgb_foreground(255,0,0);
    printf("\033[%d;%dHc", 16, 8);//y;x
    printf("\033[%d;%dHd", 1, 1);
    printf("\033[%d;%dHe", 2, 2);*/
    /*for(int i=0; i<50; i++){
        for(int j=0; j<170; j++){
            int color=(i-20)*(i-20)/5-(j-20)*(j-20)/10;
            color=(color>0?color:0)<255?color:255;
            color_to_rgb_background(0, color,0);
            printf(" ");
        }
        printf("\n");
    }*/

    //read_keys();//no delete
    if (errno)
    {
        printf("Fuck!\n");
    }
/*
    void speed_test();
    speed_test();*/

    return 0;
}


