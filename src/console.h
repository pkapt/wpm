#ifndef _CONSOLE_H
#define _CONSOLE_H

/*
    Defines an interface to interact with console
*/

#include "color.h"

int ConsoleWriteChar(char c, int x, int y, color_t color);
int ConsoleWriteLine(char * line, int x, int x, color_t color);
void ConsoleHideCursor();
void ConsoleClearScreen();


#endif
