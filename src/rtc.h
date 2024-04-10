#ifndef RTC_H
#define RTC_H

#include <stdint.h>

void rtc_init(void);
void rtc_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds);
void rtc_set_data(uint8_t weekday, uint8_t day, uint8_t mouth, uint8_t year);

#endif