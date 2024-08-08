#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
extern "C" {
#endif

void dateSet(u8 year, u8 month, u8 date, u8 day);
void timeSet(u8 hour, u8 min, u8 sec);

#ifdef __cplusplus
}
#endif

#endif