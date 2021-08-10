# Lessons learned
***
## Markdown 
`***` and '___' make a line that looks like this.
***
***
## C
### 1. Print colored text

    #include <windows.h>
    unsigned short color = <some color>;
    SetConsoleTextAttribute(hConsole, color);
    printf("text");

Colors are values between 0 to 255. Each number refers to a forground color and a background color. 

Here is small program to test out this functionality.

    #include <windows.h>

    int main() {
        for(unsigned short i = 0; i < 256; i++)
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, i);
            printf("number: %u\n", i);
        }
        return 0;
    }
