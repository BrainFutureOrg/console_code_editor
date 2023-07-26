//
// Created by maximus on 04.07.23.
//

#ifndef CONSOLE_CODE_EDITOR__COLORS_H
#define CONSOLE_CODE_EDITOR__COLORS_H

#include "prj_types/String_struct.h"
#include "bins.h"

typedef string COLOR;
#define free_color(c) free_string(c)
typedef int COLOR_PARTS;

#define DEFAULT               B(00000000000000000001)
#define BOLD                  B(00000000000000000010)
#define UNDERLINED            B(00000000000000000100)
#define FLASHING              B(00000000000000001000)
#define FOREGROUND_BLACK      B(00000000000000010000)
#define FOREGROUND_RED        B(00000000000000100000)
#define FOREGROUND_GREEN      B(00000000000001000000)
#define FOREGROUND_YELLOW     B(00000000000010000000)
#define FOREGROUND_BLUE       B(00000000000100000000)
#define FOREGROUND_PURPLE     B(00000000001000000000)
#define FOREGROUND_CYAN       B(00000000010000000000)
#define FOREGROUND_WHITE      B(00000000100000000000)
#define BACKGROUND_BLACK      B(00000001000000000000)
#define BACKGROUND_RED        B(00000010000000000000)
#define BACKGROUND_GREEN      B(00000100000000000000)
#define BACKGROUND_YELLOW     B(00001000000000000000)
#define BACKGROUND_BLUE       B(00010000000000000000)
#define BACKGROUND_PURPLE     B(00100000000000000000)
#define BACKGROUND_CYAN       B(01000000000000000000)
#define BACKGROUND_WHITE      B(10000000000000000000)

COLOR create_color(COLOR_PARTS sum_of_parts);

/**
 * @brief Create a foreground color in RGB format
 *
 * This function creates a foreground color in RGB format using the provided
 * red, green, and blue values. The foreground color is represented by the
 * COLOR type.
 *
 * @param r The red component of the color (0-255)
 * @param g The green component of the color (0-255)
 * @param b The blue component of the color (0-255)
 * @return The color in RGB format
 *
 * @see string_create_from_fcharp()
 */
COLOR color_create_foreground_rgb(unsigned char r, unsigned char g, unsigned char b);

/**
 * @brief Creates a background color using RGB values.
 *
 * This function creates a background color using the given RGB values.
 *
 * @param r The red component of the RGB color (0-255).
 * @param g The green component of the RGB color (0-255).
 * @param b The blue component of the RGB color (0-255).
 * @return The created background color.
 */
COLOR color_create_background_rgb(unsigned char r, unsigned char g, unsigned char b);

/**
 * @brief Converts the sum of color parts to a color value and prints it.
 *
 * Given the sum of color parts, this function creates a color value using the `create_color` function.
 * The resulting color is then printed using the `print_free_color` function.
 *
 * @param sum_of_parts The sum of color parts.
 */
void color_to(COLOR_PARTS sum_of_parts);

/**
 * @brief Converts the given RGB values to a foreground color.
 *
 * This function takes three unsigned char values representing the red, green, and blue components of a color,
 * and converts them to a foreground color. The resulting color value is then printed and freed.
 *
 * @param r The red component of the RGB color (0-255).
 * @param g The green component of the RGB color (0-255).
 * @param b The blue component of the RGB color (0-255).
 *
 * @see color_create_foreground_rgb
 * @see print_free_color
 */
void color_to_rgb_foreground(unsigned char r, unsigned char g, unsigned char b);

/**
 * @brief Converts the given RGB color values to a background COLOR object.
 *
 * This function takes three unsigned char values representing the red, green,
 * and blue components of an RGB color and creates a COLOR object using the
 * provided values. The created COLOR object is then used as the background
 * color for further processing.
 *
 * @param r The red component of the RGB color.
 * @param g The green component of the RGB color.
 * @param b The blue component of the RGB color.
 */
void color_to_rgb_background(unsigned char r, unsigned char g, unsigned char b);

/**
 * @brief Sets the color to default.
 *
 * This function sets the color to the default color. It internally calls the
 * `color_to` function with the `DEFAULT` parameter.
 *
 * Example Usage:
 * ```
 * color_to_default();
 * ```
 *
 * @see color_to
 */
void color_to_default();

/**
 * @brief Inverts the color.
 *
 * This function creates a color string that inverse colors and then prints and frees the color string.
 *
 * @note This function assumes the presence of a `string_create_from_fcharp()` function for creating a `COLOR` string and a `print_free_color()` function for printing and freeing the `COLOR` string.
 */
void color_inverse();

/**
 * @brief Prints formatted text in the specified color
 *
 * This function prints formatted text in the specified color using variadic arguments.
 *
 * @param color The color to apply to the printed text
 * @param format The format string for the text to be printed
 * @param ... The optional arguments to be formatted and printed
 *
 * @note The format string and the optional arguments follow the same format as the printf function.
 *
 * @see color_to_default()
 */
void color_printf(COLOR color, char *format, ...);

/**
 * @brief Print formatted output with color using the sum of color parts.
 *
 * This function prints formatted output using the provided format string and
 * optional arguments with color applied. The color is determined by the sum of
 * the color parts provided in the COLOR_PARTS enumeration.
 *
 * @param sum_of_parts The sum of color parts to determine the color.
 * @param format       The format string for the output.
 * @param ...          Optional arguments for the format string.
 *
 * @note This function assumes that the color_to() and color_to_default() functions
 *       are properly set up to handle the color change.
 *
 * @see color_to()
 * @see color_to_default()
 *
 * @example
 *
 * // Print "Hello, World!" in red
 * color_printf(RED, "Hello, %s!", "World");
 *
 * // Print the value of pi in blue
 * double pi = 3.14159;
 * color_printf(BLUE, "The value of pi is %f", pi);
 */
void color_from_parts_printf(COLOR_PARTS sum_of_parts, char *format, ...);

#endif //CONSOLE_CODE_EDITOR__COLORS_H
