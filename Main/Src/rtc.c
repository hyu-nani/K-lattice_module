#include <main.h>
#include "../Inc/rtc.h"

RTC_HandleTypeDef hrtc;

RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};

void dateSet(u8 year, u8 month, u8 date, u8 day) {
    sDate.WeekDay   = day;      /* 요일 */
    sDate.Month     = month;    /* 월 */
    sDate.Date      = date;     /* 일 */
    sDate.Year      = year;     /* 년 */
    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
        Error_Handler();
    }
}

void timeSet(u8 hour, u8 min, u8 sec) {
    sTime.Hours     = hour;   // 12시
    sTime.Minutes   = min;   // 34분
    sTime.Seconds   = sec;   // 56초
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
        Error_Handler();
    }   
}