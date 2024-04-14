#include "ili9341.h"
#include <string.h>

#ifndef ILI9341_FONT_H
#define ILI9341_FONT_H

#define FONT_W 5
#define FONT_H 8
#define FONT_S 1

void il_draw_char(uint16_t x, uint16_t y, uint8_t c, uint16_t f_color, uint16_t bg_color);

void il_draw_string(uint16_t x, uint16_t y, char *str, uint16_t f_color, uint16_t bg_color);

#endif