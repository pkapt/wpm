#include <windows.h>
#include <stdio.h>

int main() {
    for(unsigned short i = 0; i < 256; i++)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, i);
        printf("number: %u\n", i);
    }
    return 0;
}