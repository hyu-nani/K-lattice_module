#ifndef __USERMAIN_H
#define __USERMAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "../Inc/support.h"

#include "../Inc/led.h"



#define MAXQ 50
#define MAXD 10

typedef enum uartdir{       /* UART1 : USB or Front device / UART2 : Back device */
    UART1_SEND = 0U,
    UART1_RECV,
    UART2_SEND,
    UART2_RECV,
} UART_DIR;

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

typedef struct
{
    int IDX;
    int HEAD;
    int TAIL;
	int CNT;
    unsigned char BUF[MAXQ][MAXD];
} UART_QUEUE;


void user_init(void);
void user_main(void);

#ifdef __cplusplus
}
#endif

#endif