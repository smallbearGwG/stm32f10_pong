#include "handle.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>

uint8_t button_cout = 0;

void handle_init(void)
{
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_AFIO);

    nvic_enable_irq(NVIC_EXTI0_IRQ);

    gpio_set_mode(HANDLE_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, HANDLE_B1);
    gpio_get(HANDLE_PORT, HANDLE_B1);

    exti_select_source(EXTI0, HANDLE_PORT);
    exti_set_trigger(EXTI0, EXTI_TRIGGER_FALLING);
    exti_enable_request(EXTI0);
}

void exti0_isr(void)
{
    button_cout++;
    exti_reset_request(EXTI0);
}