//
// Created by maximus on 21.07.23.
//

#include <unistd.h>
#include <stdio.h>
#include "file_system_work.h"
#include "../IO/DAO.h"
#include <dirent.h>

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
                string_array_push_charp(&result.dirs, file->d_name);
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
    string_add_char(&anchor->path, '/');
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
    while (*--end != '/' && *end != '\\');
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
    while (*--end != '/' && *end != '\\');//
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
    string_add_char(&path_to_file, '/');
    string_add_charp(&path_to_file, filename);
    return path_to_file;
}