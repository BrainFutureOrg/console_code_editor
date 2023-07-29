//
// Created by maximus on 21.07.23.
//

#ifndef CONSOLE_CODE_EDITOR_FILE_SYSTEM_FILE_SYSTEM_WORK_H
#define CONSOLE_CODE_EDITOR_FILE_SYSTEM_FILE_SYSTEM_WORK_H
#include "../prj_types/String_struct.h"
typedef struct
{
    string path;
} file_system_anchor;

typedef struct
{
    string_array files;
    string_array dirs;
    string_array else_files;
} files_dirs_from_directory;

file_system_anchor system_anchor_init();
string system_anchor_get_full_path(file_system_anchor anchor);

files_dirs_from_directory system_anchor_get_dir_content(file_system_anchor anchor);

void system_anchor_go_to_dir(file_system_anchor *anchor, string directory_name);
void system_anchor_go_back_from_dir(file_system_anchor *anchor);
string system_anchor_get_directory_name(file_system_anchor *anchor);

void free_file_system_anchor(file_system_anchor anchor);
void free_files_dirs_from_directory(files_dirs_from_directory dirs);

string anchor_read_file(file_system_anchor anchor, char *filename);
void anchor_save_file(file_system_anchor anchor, char *filename, string contains);

file_system_anchor anchor_copy(file_system_anchor anchor);

#endif //CONSOLE_CODE_EDITOR_FILE_SYSTEM_FILE_SYSTEM_WORK_H
