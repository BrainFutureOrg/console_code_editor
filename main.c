#include <stdio.h>
#include <errno.h>
#include "test/string_tests.h"
#include "test/test_funcs.h"
#include "UI/user_interface.h"
#include "IO/segments.h"
#include "IO/terminal_io.h"
#include "test/test_functionality_of_new_functions.h"

void tests();

void moving_text_prototype()
{
    terminal_erase_screen;
    color_to_rgb_background(0, 0, 50);
    string text = string_create_from_fcharp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa\nbbb\nc\nd\ne\nf\nggggggggggggggggggggggg");
    urectangle region = {5, 8, 5, 20};
    //start_moving_readonly_segment(text, region);
    start_write_segment(text, region);
    free_string(text);
    color_to_default();
}

int main(int argc, char **argv)
{
    print_logo();
    tests();

    //test_cursor_functions();// test that prints strange things
//    test_console_games_functions(2);
//    test_urectangle_region_functionality();
//    test_interesting_picture();
//    read_keys();//no delete

    moving_text_prototype();

    if (errno)
    {
        printf("Fuck!\n");
    }
    return 0;
}

void tests()
{
    apply_string_tests();//no delete
    print_test_success();// no delete
}
