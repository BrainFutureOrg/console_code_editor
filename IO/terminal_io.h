//
// Created by kosenko on 06.07.23.
//

#ifndef CONSOLE_CODE_EDITOR_TERMINAL_IO_H
#define CONSOLE_CODE_EDITOR_TERMINAL_IO_H

typedef struct
{
    char *position;
} cursor;
// CTRL controls
#define preA ('A' - 1)
#define IS_CTRL(l)   !(l > 'Z' - preA || l < 1)
#define CTRL_(l)     (l - preA)
#define TO_LETTER(l) (l + preA)
typedef enum{
    UP,DOWN,RIGHT,LEFT
}ARROW;
typedef struct process_arrow_func_list{
    void (*process_arrow)(ARROW arrow);
    struct process_arrow_func_list* next;
}process_arrow_func_list;
typedef struct process_char_func_list{
    void (*process_char)(char c);
    struct process_char_func_list* next;
}process_char_func_list;
typedef struct process_ctrl_func_list{
    void (*process_ctrl_char)(char c);
    struct process_ctrl_func_list* next;
}process_ctrl_func_list;

//some comment about functions
char read_key();
void read_keys();

//ESCAPE in decimal format
#define ESC "\033"

//---------------------+
//   Cursor controls   |
//---------------------+

/* moves cursor to home position (0, 0) */
#define terminal_go_home                                   printf(ESC"[H")

/* moves cursor to line x, column y */
#define terminal_goto_xy(x, y)                             printf(ESC"[%d;%dH",x,y)

/* moves cursor up x lines */
#define terminal_go_up_x(x)                                printf(ESC"[%dA",x)

/* moves cursor down x lines */
#define terminal_go_down_x(x)                              printf(ESC"[%dB",x)

/* moves cursor right x columns */
#define terminal_go_right_x(x)                             printf(ESC"[%dC",x)

/* moves cursor left # columns */
#define terminal_go_left_x(x)                              printf(ESC"[%dD",x)

/* moves cursor to beginning of next line, x lines down */
#define terminal_go_to_beginning_of_next_x_line(x)         printf(ESC"[%dE",x)

/* moves cursor to beginning of previous line, x lines up */
#define terminal_go_to_beginning_of_previous_x_line(x)     printf(ESC"[%dF",x)

/* moves cursor to column x */
#define terminal_go_to_x_column(x)                         printf(ESC"[%dG",x)

/* request cursor position (reports as ESC[#;#R) */
#define terminal_get_cursor_position                       printf(ESC"[6n")

cursor cursor_get_cursor_position();

void cursor_print_position(cursor my_cursor);

#define cursor_go_to_position(my_cursor)                   printf(ESC"[%sH",my_cursor.position)

void cursor_printf_at_position(cursor my_cursor, char *format, ...);

void free_cursor(cursor my_cursor);

/* moves cursor one line up, scrolling if needed */
#define terminal_go_one_up_with_scroll                     printf(ESC" M")

/* save cursor position (DEC) */
#define terminal_save_cursor_position                      printf(ESC" 7")

/* restores the cursor to the last saved position (DEC) */
#define terminal_go_to_last_saved_position                 printf(ESC" 8")

//---------------------+
//   Erase Functions   |
//---------------------+

/* erase in display */
#define terminal_erase_display                             printf(ESC"[J")

/* erase from cursor until end of screen */
#define terminal_erase_from_cursor_to_end_screen           printf(ESC"[0J")

/* erase from cursor to beginning of screen */
#define terminal_erase_from_cursor_to_beginning_screen     printf(ESC"[1J")

/* erase entire screen */
#define terminal_erase_screen                              printf(ESC"[2J")

/* erase saved lines */
#define terminal_erase_saved lines                         printf(ESC"[3J")

/* erase in line */
#define terminal_erase_line                                printf(ESC"[K")

/* erase from cursor to end of line */
#define terminal_erase_from_cursor_to_end_line             printf(ESC"[0K")

/* erase start of line to the cursor */
#define terminal_erase_from_cursor_to_start_line           printf(ESC"[1K")

/* erase the entire line */
#define terminal_erase_entire_line                         printf(ESC"[2K")

//--------------------------+
//   Common Private Modes   |
//--------------------------+

/* make cursor invisible */
#define terminal_invisible_cursor                          printf(ESC"[?25l")

/* make cursor visible */
#define terminal_visible_cursor                            printf(ESC"[?25h")

/* restore screen */
#define terminal_restore_screen                            printf(ESC"[?47l")

/* save screen */
#define terminal_save_screen                               printf(ESC"[?47h")

/* enables the alternative buffer */
#define terminal_enable_alternative_buffer                 printf(ESC"[?1049h")

/* disables the alternative buffer */
#define terminal_disable_alternative_buffer                printf(ESC"[?1049l")

/*  */

#endif //CONSOLE_CODE_EDITOR_TERMINAL_IO_H
