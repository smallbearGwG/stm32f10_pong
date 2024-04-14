#include <stdlib.h>
#include <stdio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/dma.h>

#include "ili9341.h"
#include "ili9341_font.h"
#include "handle.h"

uint64_t systick_count = 0;

extern uint8_t button_cout;

void sys_tick_handler()
{
	systick_count++;
}

void delay(uint32_t milliseconds)
{
	const uint32_t cycles_per_loop = 3;
	const uint32_t cycles_per_millisecond = 128000;
	uint32_t loops = milliseconds * cycles_per_millisecond / cycles_per_loop;
	for (uint32_t i = 0; i < loops; i++)
	{
		__asm__("nop");
	}
}

void rcc_clock_setup_in_hse_8mhz_out_128mhz(void)
{
	rcc_osc_on(RCC_HSI);
	rcc_wait_for_osc_ready(RCC_HSI);
	rcc_set_sysclk_source(RCC_CFGR_SW_SYSCLKSEL_HSICLK);
	rcc_osc_on(RCC_HSE);
	rcc_wait_for_osc_ready(RCC_HSE);
	rcc_set_sysclk_source(RCC_CFGR_SW_SYSCLKSEL_HSECLK);
	rcc_set_hpre(RCC_CFGR_HPRE_SYSCLK_NODIV);
	rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV8);
	rcc_set_ppre1(RCC_CFGR_PPRE1_HCLK_NODIV);
	rcc_set_ppre2(RCC_CFGR_PPRE2_HCLK_NODIV);
	flash_set_ws(2);
	rcc_set_pll_multiplication_factor(RCC_CFGR_PLLMUL_PLL_CLK_MUL16);
	rcc_set_pll_source(RCC_CFGR_PLLSRC_HSE_CLK);
	rcc_set_pllxtpre(RCC_CFGR_PLLXTPRE_HSE_CLK);
	rcc_osc_on(RCC_PLL);
	rcc_wait_for_osc_ready(RCC_PLL);
	rcc_set_sysclk_source(RCC_CFGR_SW_SYSCLKSEL_PLLCLK);
	rcc_ahb_frequency = 128000000;
	rcc_apb1_frequency = 128000000;
	rcc_apb2_frequency = 128000000;
}

int main(void)
{
	// 初始化时钟
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

	il_init();
	il_fill_screen(0x00);
	il_draw_string(10, 200, "TINY MONITOR V1.0", 0xffff, 0x00);
	il_draw_string(10, 1, "CONNECT USB....", 0xffff, 0x00);

	// rtc设置
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
	systick_set_reload(15999);
	systick_interrupt_enable();
	systick_counter_enable();

	while (1)
	{
		char result[50];
		sprintf(result, "%ld", systick_count);
		il_draw_string(100, 100, result, 0xffff, 0x00);
		sprintf(result, "BT:%d", button_cout);
		il_draw_string(120, 20, result, 0xffff, 0x00);
	}

	return 0;
}

void test_function()
{
	// while (1)
	// {
	uint64_t time_start = systick_count;
	il_fill_screen(0x00);
	// il_draw_string(100, 100, "HELLO WORDL");

	uint64_t time_end = systick_count;
	uint64_t time_taken = time_end - time_start;

	char result[50];
	sprintf(result, "240 FRAME: %ldMS", time_taken);
	il_draw_string(10, 10, result, 0XFFFF, 0X00);
	sprintf(result, "FPS: %ld", (uint16_t)((240.0 / (double)time_taken) * 1000.0));
	il_draw_string(10, 40, result, 0XFFFF, 0X00);
	sprintf(result, "CLOCK (AHB): %ldMHZ", (uint16_t)(rcc_ahb_frequency / 1000000.0));
	il_draw_string(10, 70, result, 0XFFFF, 0X00);
	delay(50);
}