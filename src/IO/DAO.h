//
// Created by maximus on 03.07.23.
//

#ifndef CONSOLE_CODE_EDITOR__DAO_H
#define CONSOLE_CODE_EDITOR__DAO_H

#include "../prj_types/String_struct.h"

string read_file(char *filename);
void save_file(string containing, char *filename);

#endif //CONSOLE_CODE_EDITOR__DAO_H
