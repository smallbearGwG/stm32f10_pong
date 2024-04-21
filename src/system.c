#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>

#include "stdint.h"
#include "system.h"

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

void delay_ms(uint32_t ms)
{
}