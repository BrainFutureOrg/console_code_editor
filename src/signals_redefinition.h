//
// Created by maximus on 24.07.23.
//

#ifndef CONSOLE_CODE_EDITOR__SIGNALS_REDEFINITION_H
#define CONSOLE_CODE_EDITOR__SIGNALS_REDEFINITION_H

void registration_for_window_size_update(void *element, void (*changer_function)(void *element, struct winsize w));
void redefine_signals();
#endif //CONSOLE_CODE_EDITOR__SIGNALS_REDEFINITION_H
