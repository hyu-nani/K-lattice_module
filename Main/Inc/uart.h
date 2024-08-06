#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAXQ 50
#define MAXD 10

typedef enum uartdir{       /* UART1 : USB or Front device / UART2 : Back device */
    UART1_SEND = 0U,
    UART1_RECV,
    UART2_SEND,
    UART2_RECV,
} UART_DIR;

typedef struct
{
    int IDX;
    int HEAD;
    int TAIL;
	int CNT;
    unsigned char BUF[MAXQ][MAXD];
} UART_QUEUE;

void Q_Check(void);

#ifdef __cplusplus
}
#endif

#endif