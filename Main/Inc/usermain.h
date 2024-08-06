#ifndef __USERMAIN_H
#define __USERMAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#include "../Inc/led.h"
#include "../Inc/uart.h"

u8 mode; /* 0 = MASTER, 1 = SLAVE */
enum {
    MASTER = 0,
    SLAVE,
};

typedef enum
{
    MODE_OFF = 0x00,
    
    MODE_PRINT,
    MODE_PRINT_INV,
    MODE_EFFECT,
    SET_DX,
    SET_COLOR,
    SET_BG_COLOR,
    COMMAND_MAX,
} USRM_Mode_TypeDef;


void user_init(void);
void user_main(void);

#ifdef __cplusplus
}
#endif

#endif