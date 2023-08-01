//
// Created by maximus on 21.07.23.
//

#include <unistd.h>
#include <stdio.h>
#include "file_system_work.h"
#include "../IO/DAO.h"
#include <dirent.h>

#define dir_slash '/'

/*
 * Get all content without . and ..
 */
files_dirs_from_directory system_anchor_get_dir_content(file_system_anchor anchor)
{
    files_dirs_from_directory result = {string_array_create(), string_array_create()};

    DIR *directory;
    directory = opendir(anchor.path.line);

    // Check if the directory was opened successfully.
    if (directory == NULL)
    {
        errno = EBADF;
        return result;
    }

    // Declare a file pointer.
    struct dirent *file;

    // Iterate over the files in the directory.
    while ((file = readdir(directory)) != NULL)
    {
        switch (file->d_type)
        {
            case DT_REG:
                string_array_push_charp(&result.files, file->d_name);
                break;
            case DT_DIR:
                if (strcmp(file->d_name, "..") != 0 && strcmp(file->d_name, ".") != 0)
                {
                    string_array_push_charp(&result.dirs, file->d_name);
                }
                break;
            default:
                string_array_push_charp(&result.else_files, file->d_name);
                break;
        }
    }
    // Close the directory.
    closedir(directory);
    return result;
}

file_system_anchor system_anchor_init()
{
    file_system_anchor result;
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        errno = EXFULL;
        result.path = string_create_from_fcharp("");
        return result;
    }
    result.path = string_create_from_fcharp(cwd);
    return result;
}

string system_anchor_get_full_path(file_system_anchor anchor)
{
    return anchor.path;
}

void system_anchor_go_to_dir(file_system_anchor *anchor, string directory_name)
{
    string_add_char(&anchor->path, dir_slash);
    string_add_string(&anchor->path, directory_name);
}

void system_anchor_go_back_from_dir(file_system_anchor *anchor)
{
    char *end = anchor->path.line;
    if (*end == '\0' || *(end + 1) == '\0')
    {
        return;
    }
    while (*++end != '\0');
    //while (*--end != '\\');
    while (*--end != dir_slash);
    *end = '\0';
}
#include "../loging/log.h"
string system_anchor_get_directory_name(file_system_anchor *anchor)
{
    //write_log(DEBUG, "entered ...get_dir_name");
    //write_log(DEBUG, "path.line = %s", anchor->path.line);
    char *end = anchor->path.line;
    if (*end == '\0' || *(end + 1) == '\0')
    {
        return string_create_new(1);
    }
    //write_log(DEBUG, "after \\0 check");
    while (*++end != '\0');
    //write_log(DEBUG, "c1 end");
    while (*--end != dir_slash);
    //write_log(DEBUG, "c2 now = %c", *end);
    //write_log(DEBUG, "c2 end");
    return string_create_from_fcharp(end + 1);
}

void free_file_system_anchor(file_system_anchor anchor)
{
    free_string(anchor.path);
}
void free_files_dirs_from_directory(files_dirs_from_directory dirs)
{
    free_string_array(&dirs.dirs);
    free_string_array(&dirs.files);
    free_string_array(&dirs.else_files);
}

string anchor_read_file(file_system_anchor anchor, char *filename)//anchor_open_file
{
    string path_to_file = path_join(anchor.path, filename);
    string result = read_file(path_to_file.line);
    free_string(path_to_file);
    return result;
}

file_system_anchor anchor_copy(file_system_anchor anchor)
{
    file_system_anchor new;
    new.path = string_copy(anchor.path);
    return new;
}

void anchor_save_file(file_system_anchor anchor, char *filename, string contains)
{
    string path_to_file = path_join(anchor.path, filename);
    save_file(contains, path_to_file.line);
    free_string(path_to_file);
}

string path_join(string path, char *filename)
{
    string path_to_file = string_copy(path);
    string_add_char(&path_to_file, dir_slash);
    string_add_charp(&path_to_file, filename);
    return path_to_file;
}

void anchor_create_dir(file_system_anchor anchor, char *dir_name)
{
    string path_to_dir = path_join(anchor.path, dir_name);
    pid_t pid = fork();
    if (pid == -1)
    {
        write_log(ERROR, "Can`t clone process, error is: %s", strerror(errno));
        return;
    }
    if (pid == 0)
    {
        if (execlp("mkdir", "mkdir", path_to_dir.line, NULL) == -1)
        {
            write_log(ERROR, "Can`t create dir with mkdir: %s", strerror(errno));
        }
    }
}

char anchor_is_file_exist(file_system_anchor anchor, char *file_name)
{
    files_dirs_from_directory files_dirs = system_anchor_get_dir_content(anchor);
    char result = string_array_is_charp_in(&files_dirs.files, file_name) ||
        string_array_is_charp_in(&files_dirs.dirs, file_name) ||
        string_array_is_charp_in(&files_dirs.else_files, file_name);
    free_files_dirs_from_directory(files_dirs);
    return result;
}

char is_file_symbol_valid(char c)
{
    return c == dir_slash;
}

char is_file_name_valid(char *file_name)
{
    while (*file_name != '\0')
    {
        if (is_file_symbol_valid(*file_name++))
        {
            return 0;
        }
    }
    return 1;
}