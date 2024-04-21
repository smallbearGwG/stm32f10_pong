#include <stdlib.h>
#include <stdio.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/timer.h>

#include "drivers/inc/ili9341.h"
#include "drivers/inc/ili9341_font.h"
#include "drivers/inc/handle.h"

#include "system.h"

int main(void)
{
	// 初始化时钟
	rcc_clock_setup_in_hse_8mhz_out_128mhz();

	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

	il_init();
	il_fill_screen(0x00);

	uint16_t x = 0;
	uint16_t y = 0;

	uint16_t x_add = 1;
	uint16_t y_add = 1;
	while (1)
	{
	}

	return 0;
}

void test_speed(void)
{
	// il_fill_rect(x - x_add, y - y_add, 5, 5, 0x00);
	// il_fill_rect(x, y, 5, 5, 0xffff);
	// x += x_add;
	// y += y_add;
	// if (x >= 240 || y >= 240)
	// {
	// 	il_fill_rect(x - x_add, y - y_add, 5, 5, 0x00);
	// 	x = 0;
	// 	y = 0;
	// }
	// delay_ms(1);
}

void draw_table(void)
{
	il_fill_rect(0, 0, 320, 2, 0xFFFF);
	il_fill_rect(0, 238, 320, 2, 0xFFFF);
	//
	// uint16_t index = 0;
	il_fill_rect((160 - 1), 0 + 5, 2, 19, 0xffff);
	il_fill_rect((160 - 1), 30 + 5, 2, 19, 0xffff);
	il_fill_rect((160 - 1), 60 + 5, 2, 19, 0xffff);
	il_fill_rect((160 - 1), 90 + 5, 2, 19, 0xffff);
	il_fill_rect((160 - 1), 120 + 5, 2, 19, 0xffff);
	il_fill_rect((160 - 1), 150 + 5, 2, 19, 0xffff);
	il_fill_rect((160 - 1), 180 + 5, 2, 19, 0xffff);
	il_fill_rect((160 - 1), 210 + 5, 2, 19, 0xffff);
}