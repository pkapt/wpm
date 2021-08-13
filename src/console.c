#include <windows.h>
#include <stdio.h>
#include "console.h"

int ConsoleWriteChar(char c, int x, int y, color_t color)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    SetConsoleCursorPosition(hConsole, pos);
    printf("%c", c);
}

int ConsoleWriteLine(char * line, int x, int y, color_t color)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    SetConsoleCursorPosition(hConsole, pos);
    printf("%s", line);
}

int ConsoleWriteTwoDigits(int n)
{
    int temp;
	if (n <= 9)
		printf("0%d", n);
	else {
        printf("%d", n);
	}
}

int ConsoleWriteTime(int min, int sec, int x, int y)
{
    if (min > 60)
    {
        min = 60;
    }
    else if (min < 0)
    {
        min = 0;
    }

    if (sec > 60)
    {
        sec = 60;
    }
    else if (sec < 0)
    {
        sec = 0;
    }

    
    COORD pos;
    pos.X = x;
    pos.Y = y;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, COLOR_WHITE);
    SetConsoleCursorPosition(hConsole, pos);
    ConsoleWriteTwoDigits(min);
    printf(":");
    ConsoleWriteTwoDigits(sec);
}

void ConsoleHideCursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void ConsoleShowCursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = TRUE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void ConsoleClearScreen() 
{
    printf("\e[1;1H\e[2J");
}

void ConsoleSetColor(color_t color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}