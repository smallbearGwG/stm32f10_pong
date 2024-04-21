#include <libopencm3/stm32/rcc.h>

#include "../inc/usart.h"

static inline void uarst_clock_setup(void)
{
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_USART1);
}

void uarst_init(void)
{
}

char *number_to_str(int32_t number)
{

}