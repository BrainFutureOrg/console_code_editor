//
// Created by maximus on 04.07.23.
//

#ifndef CONSOLE_CODE_EDITOR__PRJ_ERROR_H
#define CONSOLE_CODE_EDITOR__PRJ_ERROR_H

#include <errno.h>
#define errno_to_default errno = 0
#define check_errno_void if(errno)                    \
                            {                         \
                               return;                \
                            }
#define check_errno_void_p if(errno)                  \
                            {                         \
                               print_error();         \
                               return;                \
                            }
#define check_errno(func_return) if(errno)            \
                            {                         \
                               return func_return;    \
                            }
#define check_errno_p(func_return) if(errno)          \
                            {                         \
                               print_error();         \
                               return func_return;    \
                            }

void print_error();

#endif //CONSOLE_CODE_EDITOR__PRJ_ERROR_H
