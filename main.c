#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "src/line.h"
#include "src/console.h"
#include "src/color.h"

#define ASCII_BREAK 3
#define ASCII_SPACE 32
#define ASCII_NEWLINE 13
#define ASCII_BACKSPACE 8
#define ASCII_A 97
#define ASCII_Z 122

#define LEN_WORD_BANK 271
#define WORDS_PER_LINE 10

char *word_bank[LEN_WORD_BANK] =
{
    "was", "are", "have", "had", "were", "can", "said", "use", "will", "would",
    "make", "like", "has", "look", "write", "see", "could", "been", "call",
    "find", "did", "get", "come", "made", "may", "take", "know", "live",
    "give", "think", "say", "help", "tell", "follow", "came", "want", "show",
    "set", "put", "does", "must", "ask", "went", "read", "need", "move", "try",
    "change", "play", "spell", "found", "study", "learn", "should", "add",
    "keep", "start", "thought", "saw", "turn", "might", "close", "seem",
    "open", "begin", "got", "run", "walk", "began", "grow", "took", "carry",
    "hear", "stop", "miss", "eat", "watch", "let", "cut", "talk", "being", 
    "leave", "word", "time", "number", "way", "people", "water", "day", "part",
    "sound", "work", "place", "year", "back", "thing", "name", "sentence",
    "man", "line", "boy", "farm", "end", "men", "land", "home", "hand",
    "picture", "air", "animal", "house", "page", "letter", "point", "mother",
    "answer", "america", "world", "food", "country", "plant", "school",
    "father", "tree", "city", "earth", "eye", "head", "story", "example", 
    "life", "paper", "group", "children", "side", "feet", "car", "mile", 
    "night", "sea", "river", "state", "book", "idea", "face", "indian", "girl",
    "mountain", "list", "song", "family", "one", "all", "each", "other", "many",
    "some", "two", "more", "long", "new", "little", "most", "good", "great",
    "right", "mean", "old", "any", "same", "three", "small", "another", "large",
    "big", "even", "such", "different", "kind", "still", "high", "every", "own",
    "light", "left", "few", "next", "hard", "both", "important", "white", "four",
    "second", "enough", "above", "young", "not", "when", "there", "how", "out", 
    "then", "first", "now", "only", "very", "just", "where", "much", "before", 
    "too", "also", "around", "well", "here", "why", "again", "off", "away", 
    "near", "below", "last", "never", "always", "together", "often", "once", 
    "later", "far", "really", "almost", "sometimes", "soon", "for", "with", "from", 
    "about", "into", "down", "over", "after", "through", "between", "under", 
    "along", "until", "without", "you", "that", "his", "they", "this", "what", 
    "your", "which", "she", "their", "them", "these", "her", "him", "who", 
    "its", "our", "something", "those", "and", "but", "than", "because", "while"
};

int get_chunk_of_random_words(int num_words, char ** word_bank, char * outbuff)
{
    int chars_written = 0;
    for (int i = 0; i < num_words; i++)
    {
        int j = rand() % (LEN_WORD_BANK - 1);
        char * rand_str = word_bank[j];
        chars_written += snprintf(outbuff + chars_written, strlen(rand_str) + 2, "%s ", rand_str);
    }
    memset(outbuff + strlen(outbuff) - 1, ASCII_NEWLINE, 1);
}

int getch_noblock() {
    if (_kbhit())
        return _getch();
    else
        return -1;
}

int write_lines(color_enc_line_t * line_enc, char * line1, char * line2, int pos_x, int pos_y, color_t cline1, color_t cline2)
{
    ColorEncodedLineWrite(line_enc, pos_x, pos_y);
    ConsoleWriteLine(line1, pos_x, pos_y + 1, COLOR_WHITE);
    ConsoleWriteLine(line2, pos_x, pos_y + 2, COLOR_WHITE);
}

color_t get_letter_correctness(char incoming_letter, char master_letter) 
{
    if (incoming_letter == master_letter) 
    {
        return COLOR_YELLOW;
    }
    else
    {
        return COLOR_RED;
    }
}

void cleanupConsole()
{
    ConsoleSetColor(COLOR_WHITE);
    ConsoleClearScreen();
    ConsoleShowCursor();
}

bool quit_flag = false;

void exit_program()
{
    quit_flag = true;
    cleanupConsole();
}

int main( int argc, char *argv[] ) {
    int pos_x = 0;
    int pos_y = 0;
    int timeout = 0;
    int num_wrong_words = 0;
    int num_right_words = 0;
    char line1[LINE_MAX_LEN] = {0};
    char line2[LINE_MAX_LEN] = {0};
    char line3[LINE_MAX_LEN] = {0};
    char * pline1 = line1;
    char * pline2 = line2;
    char * pline3 = line3;
    bool quit = false;

    // initialize random generator for fetching random words
    srand(time(NULL)); 

    // initialize command line parameters
    if (argc < 2)
    {
        fprintf( stderr, "Must supply positional argument timeout: wpm.exe <timeout>\nExample: wpm.exe 15", 30);
        return 0;
    }
    else 
    {
        char *a = argv[1];
        timeout = atoi(a) * 1000;
    }

    // initialize data structures
    get_chunk_of_random_words(WORDS_PER_LINE, word_bank, pline1);
    get_chunk_of_random_words(WORDS_PER_LINE, word_bank, pline2);
    get_chunk_of_random_words(WORDS_PER_LINE, word_bank, pline3);

    color_enc_line_t recorded_letters;
    memset(&recorded_letters, 0x00, sizeof(color_enc_line_t));
    char * master_string = pline1;

    // intialize timer
    LARGE_INTEGER frequency;
    LARGE_INTEGER startTimeTicks, currentTimeTicks;
    unsigned long int elapsedTime_ms, elapsedTime_s, elapsedTime_m;
    QueryPerformanceFrequency(&frequency); // get ticks per second
    QueryPerformanceCounter(&startTimeTicks); // start timer

    // intialize console
    ConsoleHideCursor();
    ConsoleClearScreen();
    printf("\n\n\n\nBegin typing to start test...");
    ConsoleWriteLine(pline1, pos_x, pos_y, COLOR_WHITE);
    ConsoleWriteLine(pline2, pos_x, pos_y + 1, COLOR_WHITE);
    ConsoleWriteLine(pline3, pos_x, pos_y + 2, COLOR_WHITE);

    int firstKeyPress = _getch();
    ConsoleClearScreen();
    ConsoleWriteLine(pline1, pos_x, pos_y, COLOR_WHITE); // am lazy :()
    ConsoleWriteLine(pline2, pos_x, pos_y + 1, COLOR_WHITE);
    ConsoleWriteLine(pline3, pos_x, pos_y + 2, COLOR_WHITE);
    int keypress;


    while(!quit_flag) 
    {
        if (firstKeyPress != -1)
        {
            keypress = firstKeyPress;
            firstKeyPress = -1;
        }
        else
        {
            keypress = getch_noblock();
        }
        

        if (keypress != -1)
        {
            if (keypress == ASCII_BREAK)
            {
                exit_program();
            }
            else if (keypress == ASCII_NEWLINE)
            {

            }
            else if (keypress == ASCII_BACKSPACE)
            {

            }
            else if (master_string[pos_x] == ASCII_NEWLINE)
            {
                if (keypress != ASCII_SPACE)
                {
                    ConsoleWriteChar((char) keypress, pos_x - 1, pos_y, get_letter_correctness(keypress, master_string[pos_x]));
                    letter_t letter = {.color = COLOR_RED, .letter = (char)keypress};
                    ColorEncodedLineAppend(&recorded_letters, letter, 0);
                    num_wrong_words++;
                }
                else
                {
                    pos_x = 0;
                    if (pos_y == 0)
                    {
                        pos_y++;
                    }
                    else
                    {
                        ConsoleClearScreen();
                        int size = sizeof(pline1);
                        memcpy(pline2, pline3, sizeof(char) * LINE_MAX_LEN);
                        memset(pline3, 0x00, sizeof(*pline3));
                        get_chunk_of_random_words(WORDS_PER_LINE, word_bank, pline3);
                        write_lines(&recorded_letters, pline2, pline3, 0, 0, COLOR_WHITE, COLOR_WHITE);
                    }
                    memset(&recorded_letters, 0x00, sizeof(recorded_letters));
                    master_string = pline2;
                }

            }
            else if (master_string[pos_x] == ASCII_SPACE)
            {
                if (keypress != ASCII_SPACE)
                {
                    ConsoleWriteChar((char) keypress, pos_x - 1, pos_y, get_letter_correctness(keypress, master_string[pos_x]));
                    letter_t letter = {.color = COLOR_RED,.letter = (char) keypress};
                    ColorEncodedLineAppend(&recorded_letters, letter, -1);
                    num_wrong_words++;
                }
                else
                {
                    ConsoleWriteChar((char) keypress, pos_x, pos_y, get_letter_correctness(keypress, master_string[pos_x]));
                    letter_t letter = {.color = COLOR_YELLOW,.letter = (char) keypress};
                    ColorEncodedLineAppend(&recorded_letters, letter, 0);
                    num_right_words++;
                    pos_x++;
                }
            }
            else if ((master_string[pos_x] >= ASCII_A) && (master_string[pos_x] <= ASCII_Z))
            {
                if (keypress == ASCII_SPACE)
                {
                    while (1) {
                        if (master_string[pos_x] == ASCII_SPACE) 
                        {
                            pos_x++;
                            letter_t letter = {.color = COLOR_YELLOW,.letter = (char) keypress};
                            ColorEncodedLineAppend(&recorded_letters, letter, 0);
                            break;
                        }
                        else if (master_string[pos_x] == ASCII_NEWLINE)
                        {
                            pos_x = 0;
                            if (pos_y == 0)
                            {
                                pos_y++;
                            }
                            else
                            {
                                ConsoleClearScreen();
                                // show timer
                                int size = sizeof(pline1);
                                memcpy(pline2, pline3, sizeof(char) * LINE_MAX_LEN);
                                memset(pline3, 0x00, sizeof(*pline3));
                                get_chunk_of_random_words(WORDS_PER_LINE, word_bank, pline3);
                                write_lines(&recorded_letters, pline2, pline3, 0, 0, COLOR_WHITE, COLOR_WHITE);
                            }
                            memset(&recorded_letters, 0x00, sizeof(recorded_letters));
                            master_string = pline2;
                            break;
                        }
                        else
                        {
                            letter_t letter = {.color = COLOR_RED,.letter = master_string[pos_x]};
                            ColorEncodedLineAppend(&recorded_letters, letter, 0);
                            ConsoleWriteChar(master_string[pos_x], pos_x, pos_y, COLOR_RED);
                            pos_x++;
                        }
                        num_wrong_words++;
                    }
                }
                else
                {
                    if (keypress == master_string[pos_x])
                    {
                        letter_t letter = {.color = COLOR_YELLOW,.letter = (char) keypress};
                        ColorEncodedLineAppend(&recorded_letters, letter, 0);
                    }
                    else
                    {
                        letter_t letter = {.color = COLOR_RED,.letter = (char) keypress};
                        ColorEncodedLineAppend(&recorded_letters, letter, 0);
                    }
                    color_t letter_color = get_letter_correctness(keypress, master_string[pos_x]);
                    ConsoleWriteChar((char) keypress, pos_x, pos_y, letter_color);
                    pos_x++;
                }
            }
        }

        QueryPerformanceCounter(&currentTimeTicks);
        elapsedTime_ms = (currentTimeTicks.QuadPart - startTimeTicks.QuadPart) * 1000.0 / frequency.QuadPart;
        double elapsed_ms = timeout - elapsedTime_ms;
        elapsedTime_s = elapsed_ms / 1000;
        elapsedTime_m = elapsedTime_s / 60;
        int seconds_left = elapsedTime_s - (elapsedTime_m * 60);
        ConsoleWriteTime(elapsedTime_m, seconds_left, 0, 5);
        
        if (elapsedTime_ms >= timeout)
        {
            exit_program();
        }
    }

    float total_minutes = ((float)elapsedTime_ms / 1000.0 / 60.0);
    float wpm = num_right_words / total_minutes;
    printf("Your words per minute is %0.2f\n", wpm);
    printf("You got %u words wrong", num_wrong_words);

    return 0;
}

