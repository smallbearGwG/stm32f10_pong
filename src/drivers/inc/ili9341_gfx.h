#include "ili9341.h"

#ifndef ILI9341_H
#define ILI9341_H

/**
 * @brief 绘制像素
 */
void il_draw_pixel(uint16_t x, uint8_t y, uint16_t color);

/**
 * @brief 绘制垂直线条
 */
void il_draw_fast_h_line(uint16_t x, uint16_t y, uint16_t width, uint16_t color);

/**
 * @brief 绘制水平线条
 */
void il_draw_fast_v_line(uint16_t x, uint16_t y, uint16_t width, uint16_t color);

/**
 * @brief 绘制线条
 */
void il_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

/**
 * @brief 绘制圆
 */
void il_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);

#endif