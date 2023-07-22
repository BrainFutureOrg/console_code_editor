//
// Created by maximus on 21.07.23.
//

#include <unistd.h>
#include <stdio.h>
#include "file_system_work.h"
#include <dirent.h>

string_array get_file_names_list(file_system_anchor anchor)
{
    DIR *directory;

    // Open the current directory.
    directory = opendir("./..");

    // Check if the directory was opened successfully.
    if (directory == NULL)
    {
        printf("Could not open directory.\n");
//        return;
    }

    // Declare a file pointer.
    struct dirent *file;

    // Iterate over the files in the directory.
    while ((file = readdir(directory)) != NULL)
    {
        // Check if the file is a regular file.
//        if (file->d_type == DT_REG)
        {
            // Print the file name.
            printf("%s\n", file->d_name);
        }
    }

    // Close the directory.
    closedir(directory);
}

string_array get_directories_names_list(file_system_anchor anchor);