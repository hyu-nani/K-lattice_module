#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    JAN = 1U,
    FEB,
    MAR,
    APR,
    MAY,
    JUN,
    JUL,
    AUG,
    SEP,
    OCT,
    NOV,
    DEC,
} MONTH_LIST;

typedef enum {
    MON = 0U,
    TUE,
    WED,
    THU,
    FRI,
    SAT,
    SUN,
} DAY_LIST;

#ifdef __cplusplus
}
#endif

#endif