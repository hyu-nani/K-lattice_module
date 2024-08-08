#include <main.h>
#include "../Inc/rtc.h"

RTC_HandleTypeDef hrtc;

RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};

void timeSet() {
    sTime.Hours = 0x12;     // 12시
    sTime.Minutes = 0x34;   // 34분
    sTime.Seconds = 0x56;   // 56초
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
        Error_Handler();
    }

    sDate.WeekDay = RTC_WEEKDAY_FRIDAY; // 금요일
    sDate.Month = RTC_MONTH_AUGUST;     // 8월
    sDate.Date = 0x07;                  // 7일
    sDate.Year = 0x24;                  // 2024년
    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
        Error_Handler();
    }
}