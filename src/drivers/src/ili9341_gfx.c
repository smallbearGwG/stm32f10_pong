#include "../inc/ili9341_gfx.h"

void il_draw_pixel(uint16_t x, uint8_t y, uint16_t color)
{
    il_set_address_window(x, y, x, y);
    il_fill_color_array(color, 1);
}

void il_draw_fast_h_line(uint16_t x, uint16_t y, uint16_t width, uint16_t color)
{
    il_fill_rect(x, x + y - 1, 1, y, color);
}

void il_draw_fast_v_line(uint16_t x, uint16_t y, uint16_t width, uint16_t color)
{
}