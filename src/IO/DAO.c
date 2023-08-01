//
// Created by maximus on 03.07.23.
//

#include <stdio.h>
#include "DAO.h"
#include "../prj_error.h"
#include "../loging/log.h"

FILE *open_file_checking(char *filename, char *type)
{
    FILE *fp;
    if (!(fp = fopen(filename, type)))
    {
        errno = EACCES;
        write_log(FATAL, "File not opened, filename = %s, type = %s", filename, type);

    }
    return fp;

}

string read_file(char *filename)
{
    FILE *fp = open_file_checking(filename, "r");
    if (errno)
    {
        string empty = string_create_new(1);
        return empty;
    }
    char c;
    int filelen = 0;
    while (((char)getc(fp)) != EOF)
    {
        filelen++;
    }
    string_fast result_fast = string_fast_create_new(filelen + 1);
    fseek(fp, SEEK_SET, 0);
    while ((c = (char)getc(fp)) != EOF)
    {
        string_fast_add_char(&result_fast, c);
    }
    fclose(fp);
    return result_fast.string_part;
}
void save_file(string containing, char *filename)
{
    FILE *fp = open_file_checking(filename, "w+");
    check_errno_void
    while (*containing.line != '\0')
    {
        putc(*(containing.line++), fp);
    }
    fclose(fp);

}