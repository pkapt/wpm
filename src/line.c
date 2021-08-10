#include <windows.h>
#include "line.h"

int ColorEncodedLineAppend(color_enc_line_t * line, letter_t c, int index_offset)
{
    line->letters[line->len + index_offset].letter = c.letter;
    line->letters[line->len + index_offset].color = c.color;
    line->len++;
}

int ColorEncodedLineWrite(color_enc_line_t * line, int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;

    for (int i = 0; i < line->len; i++)
    {
        write_char(line->letters[i].letter, x, y, line->letters[i].color);
        x++;
    }
}