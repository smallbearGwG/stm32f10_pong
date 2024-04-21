#include "../inc/rtc.h"

#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/rtc.h"

void rtc_clock_setup(void)
{
    rcc_periph_clock_enable(RCC_PWR);
    rcc_periph_clock_enable(RCC_BKP);

    pwr_disable_backup_domain_write_protect();
}

void rtc_init(void) {}

void rtc_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds) {}

void rtc_set_data(uint8_t weekday, uint8_t day, uint8_t mouth, uint8_t year) {}