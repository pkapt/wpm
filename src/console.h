#ifndef _CONSOLE_H
#define _CONSOLE_H

/*
    Defines an interface to interact with console
*/

#include "color.h"

int ConsoleWriteChar(char c, int x, int y, color_t color);
int ConsoleWriteLine(char * line, int x, int y, color_t color);
void ConsoleHideCursor();
void ConsoleShowCursor();
void ConsoleClearScreen();
void ConsoleSetColor(color_t color);
int ConsoleWriteTwoDigits(int n);
int ConsoleWriteTime(int min, int sec, int x, int y);


#endif
