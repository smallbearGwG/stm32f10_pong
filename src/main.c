#include <stdlib.h>
#include <stdio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/dma.h>

#include "ili9341.h"

// uint64_t systick_count = 0;

// void sys_tick_handler()
// {
// 	systick_count++;
// }

void delay(uint32_t milliseconds)
{
	// 每个循环延时大约 3 个 CPU 周期
	// 基于实际情况调整这个值
	const uint32_t cycles_per_loop = 3;

	// 根据 CPU 频率计算每个周期的耗时
	const uint32_t cycles_per_millisecond = 128000;

	// 计算需要循环的次数
	uint32_t loops = milliseconds * cycles_per_millisecond / cycles_per_loop;

	// 执行延时
	for (uint32_t i = 0; i < loops; i++)
	{
		__asm__("nop");
	}
}

/**
 * Overclock stm32 to 128MHz. HSE 8MHz multiplied by PLL 16
 * Performance may become unstable
 */
void rcc_clock_setup_in_hse_8mhz_out_128mhz(void);

int main(void)
{
	// 初始化时钟
	rcc_clock_setup_in_hse_8mhz_out_128mhz();

	// // rtc设置
	// systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
	// systick_set_reload(15999);
	// systick_interrupt_enable();
	// systick_counter_enable();

	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

	il_init();

	while (1)
	{
		il_fill_screen(0x0000);
		delay(10);
		il_fill_screen(0xFFFF);
		delay(10);
	}
	return 0;
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