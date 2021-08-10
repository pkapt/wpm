#ifndef _LINE_H
#define _LINE_H

/*
    Defines an interface for line data types
*/

#include "color.h"

#define LINE_MAX_LEN 500

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

int ColorEncodedLineAppend(color_enc_line_t * line, letter_t c, int index_offset);
int ColorEncodedLineWrite(color_enc_line_t * line, int x, int y);

#endif
