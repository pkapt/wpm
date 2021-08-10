#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define ASCII_BREAK 3
#define ASCII_SPACE 32
#define ASCII_NEWLINE 13
#define ASCII_BACKSPACE 8
#define ASCII_A 97
#define ASCII_Z 122

#define LINE_MAX_LEN 500
#define LEN_WORD_BANK 271
#define WORDS_PER_LINE 1

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

typedef enum {
    COLOR_RED = 4,
    COLOR_YELLOW = 6
} color_t;

typedef struct letter_s 
{
    char letter;
    color_t color;
} letter_t;

typedef struct color_enc_line_s
{
    letter_t letters[LINE_MAX_LEN];
    int len; 
} color_enc_line_t;

int color_enc_line_append(color_enc_line_t * line, letter_t c, int index_offset)
{
    line->letters[line->len + index_offset].letter = c.letter;
    line->letters[line->len + index_offset].color = c.color;
    line->len++;
}

int get_chunk_of_random_words(int num_words, char ** word_bank, char * outbuff)
{
    int chars_written = 0;
    // todo maybe make this safer by checking that you're not overflowing the buffer outbuff
    for (int i = 0; i < num_words; i++)
    {
        int j = rand() % LEN_WORD_BANK - 1;
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

void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

int write_char(char c, int x, int y, color_t color)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%c", c);
}

void clrscr() 
{
    printf("\e[1;1H\e[2J");
}

void write_line(char * line, int pos_x, int pos_y)
{
    COORD pos;
    pos.X = pos_x;
    pos.Y = pos_y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%s", line);
}

void write_color_encoded_line(color_enc_line_t * line, int pos_x, int pos_y)
{
    COORD pos;
    pos.X = pos_x;
    pos.Y = pos_y;

    for (int i = 0; i < line->len; i++)
    {
        write_char(line->letters[i].letter, pos_x, pos_y, line->letters[i].color);
        pos_x++;
    }
}

int write_lines(color_enc_line_t * line_enc, char * line1, char * line2, int num_lines, int pos_x, int pos_y)
{
    write_color_encoded_line(line_enc, pos_x, pos_y);
    write_line(line1, pos_x, pos_y + 1);
    write_line(line2, pos_x, pos_y + 2);
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

int main() {
    clrscr();
    srand(time(NULL)); // initialie random generator for fetching random words
    bool quit = false;
    int pos_x = 0;
    int pos_y = 0;
    int num_wrong_words = 0;
    int num_right_words = 0;

    char line1[LINE_MAX_LEN] = {0};
    char line2[LINE_MAX_LEN] = {0};
    char line3[LINE_MAX_LEN] = {0};
    char * pline1 = line1;
    char * pline2 = line2;
    char * pline3 = line3;
    get_chunk_of_random_words(WORDS_PER_LINE, word_bank, pline1);
    get_chunk_of_random_words(WORDS_PER_LINE, word_bank, pline2);
    get_chunk_of_random_words(WORDS_PER_LINE, word_bank, pline3);
    color_enc_line_t recorded_letters;
    memset(&recorded_letters, 0x00, sizeof(color_enc_line_t));

    write_line(pline1, pos_x, pos_y);
    write_line(pline2, pos_x, pos_y + 1);
    write_line(pline3, pos_x, pos_y + 2);

    char * master_string = pline1;

    hidecursor();

    while(!quit) 
    {
        int keypress = _getch();
        // write_char(keypress, 1, 1, COLOR_RED);

        if (keypress == ASCII_BREAK)
        {
            quit = true;
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
                write_char((char) keypress, pos_x - 1, pos_y, 
                    get_letter_correctness(keypress, master_string[pos_x]));
                letter_t letter = 
                {
                    .color = COLOR_RED,
                    .letter = (char) keypress
                };
                color_enc_line_append(&recorded_letters, letter, 0);
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
                    clrscr();
                    // show timer
                    int size = sizeof(pline1);
                    memcpy(pline2, pline3, sizeof(char) * LINE_MAX_LEN);
                    memset(pline3, 0x00, sizeof(*pline3));
                    get_chunk_of_random_words(WORDS_PER_LINE, word_bank, pline3);
                    write_lines(&recorded_letters, pline2, pline3, 3, pos_x, pos_y); // reset color position to write before you do this
                }
                memset(&recorded_letters, 0x00, sizeof(recorded_letters));
                master_string = pline2;
                // set the cursor position to the right place
            }

        }
        else if (master_string[pos_x] == ASCII_SPACE)
        {
            if (keypress != ASCII_SPACE)
            {
                write_char((char) keypress, pos_x - 1, pos_y, 
                    get_letter_correctness(keypress, master_string[pos_x]));
                letter_t letter = 
                {
                    .color = COLOR_RED,
                    .letter = (char) keypress
                };
                color_enc_line_append(&recorded_letters, letter, -1);
                num_wrong_words++;
            }
            else
            {
                write_char((char) keypress, pos_x, pos_y, 
                    get_letter_correctness(keypress, master_string[pos_x]));
                letter_t letter = 
                {
                    .color = COLOR_YELLOW,
                    .letter = (char) keypress
                };
                color_enc_line_append(&recorded_letters, letter, 0);
                num_right_words++;
                pos_x++;
            }
        }
        else if ((master_string[pos_x] >= ASCII_A) && (master_string[pos_x] <= ASCII_Z))
        {
            if (keypress == ASCII_SPACE)
            {
                // while (1) {
                //     if (master_string[pos_x] == ASCII_SPACE) 
                //     {
                //         pos_x++;
                //         letter_t letter = 
                //         {
                //             .color = COLOR_YELLOW,
                //             .letter = (char) keypress
                //         };
                //         color_enc_line_append(&recorded_letters, letter, 0);
                //         break;
                //     }
                //     else if (master_string[pos_x] == ASCII_NEWLINE)
                //     {
                //         pos_x = 0;
                //         if (pos_y == 0)
                //         {
                //             pos_y++;
                //         }
                //         else
                //         {
                //             clrscr();
                //             // show timer
                //             pline2 = pline3;
                //             memset(pline3, 0x00, sizeof(pline3));
                //             get_chunk_of_random_words(WORDS_PER_LINE, word_bank, pline3);
                //             write_lines(&recorded_letters, pline2, pline3, 3, pos_x, pos_y);
                //         }
                //         memset(&recorded_letters, 0x00, sizeof(recorded_letters));
                //         master_string = pline2;
                //         // set the cursor position to the right place
                //         break;
                //     }
                //     else
                //     {
                //         write_char((char) keypress, pos_x, pos_y, COLOR_RED);
                //         letter_t letter = 
                //         {
                //             .color = COLOR_RED,
                //             .letter = (char) keypress
                //         };
                //         color_enc_line_append(&recorded_letters, letter, 0);
                //         pos_x++;
                //     }
                //     num_wrong_words++;
                // }
            }
            else
            {
                if (keypress == master_string[pos_x])
                {
                    letter_t letter = 
                    {
                        .color = COLOR_YELLOW,
                        .letter = (char) keypress
                    };
                    color_enc_line_append(&recorded_letters, letter, 0);
                }
                else
                {
                    letter_t letter = 
                    {
                        .color = COLOR_RED,
                        .letter = (char) keypress
                    };
                    color_enc_line_append(&recorded_letters, letter, 0);
                }
                color_t letter_color = get_letter_correctness(keypress, master_string[pos_x]);
                write_char((char) keypress, pos_x, pos_y, letter_color);
                pos_x++;
            }
        }
    }
    return 0;
}