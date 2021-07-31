#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>

#define ASCII_BREAK 3
#define ASCII_SPACE 32
#define ASCII_ENTER 13
#define ASCII_BACKSPACE 8
#define ASCII_A 97
#define ASCII_Z 122

#define LINE_MAX_LEN 500

typedef struct letter_s 
{
    unsigned short letter;
    unsigned short color;
} letter_t;

int getch_noblock() {
    if (_kbhit())
        return _getch();
    else
        return -1;
}

void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

int write_char(char c, int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%c", c);
}

int write_lines(char * line) {
    COORD pos;
    pos.X = 10;
    pos.Y = 10;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%s", line);
}

void clrscr() {
    printf("\e[1;1H\e[2J");
}

char * test_str_1 = "hello moto nice to meet you";
char * test_str_2 = "fart time hello moto now go";

int main() {
    // printf("hello, world");

    // start a timer
    // while timer is not timed out
    bool quit = false;
    int pos_x = 0;
    int pos_y = 0;



    char * master_string = "hello moto nice to meet you";
    write_lines(master_string);
    printf("%s", master_string);

    // clear the console
    // clrscr();

    hidecursor();

    while(!quit) 
    {
        int keypress = _getch();
        // printf("%u\n", keypress);

        if (keypress == ASCII_BREAK)
        {
            quit = true;
        }

        else if (keypress == ASCII_ENTER)
        {

        }

        else if (keypress == ASCII_BACKSPACE)
        {

        }

        else if (master_string[pos_x] == ASCII_ENTER)
        {

        }

        else if (master_string[pos_x] == ASCII_SPACE)
        {

        }
        else if ((master_string[pos_x] >= ASCII_A) && (master_string[pos_x] <= ASCII_Z))
        {
            printf("here");
            if (keypress == ASCII_ENTER)
            {

            } 
            else
            {
                if (keypress == master_string[pos_x])
                {
                    // append the letter to the recorded letters buffer in yello
                } 
                else
                {
                    // append the letter to the recorded letter buffer in red
                }
                write_char(keypress, 15, 5);
            }
        }
        printf("%u ASCII_A ASCII_Z\n", master_string[pos_x]);

    }
    return 0;
}