#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>

#define ASCII_BREAK 3
#define ASCII_SPACEBAR 32
#define ASCII_ENTER 13
#define ASCII_BACKSPACE 8
#define ASCII_A 97
#define ASCII_Z 122

int main() {
    // printf("hello, world");

    // start a timer
    // while timer is not timed out
    bool quit = false;
    int pos_x = 0;
    int pos_y = 0;

    char * master_string = "hello moto nice to meet you";
    printf("%s", master_string);

    while(!quit) 
    {
        int keypress = _getch();
        printf("%u\n", keypress);

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

        else if (master_string[pos_x] >= ASCII_ENTER)
        {

        }

        else if (master_string[pos_x] >= ASCII_SPACEBAR)
        {

        }
        else if ((master_string[pos_x] >= ASCII_A) && (master_string[pos_x] >= ASCII_Z))
        {

        }
    }
    return 0;
}