#ifndef UARST_H
#define UARST_H

#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>

#define USARTx USART1
#define USART_PORT GPIOB
#define USART_TX GPIO6
#define USART_RX GPIO7

void uarst_init(void);

#endif