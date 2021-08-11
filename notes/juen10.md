# Lessons learned
***

## 1. makefiles

when using `gcc`, you have to specify all your source files. like this:

        @gcc main.c src/line.c src/console.c -o dist/wpm.exe -O0

