#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAXQ 50
#define MAXD 16

typedef enum uartdir{       /* UART1 : USB or Front device / UART2 : Back device */
    UART1_SEND = 0U,
    UART1_RECV,
    UART2_SEND,
    UART2_RECV,
} UART_DIR;

typedef enum cmdlist{
    CMD_SET_CHAR = 0,
    CMD_SET_COLOR,
    CMD_SET_DX,
    CMD_GET_NUM_DEVICE,
} CMD_LIST;


typedef struct
{
    int IDX;
    int HEAD;
    int TAIL;
	int CNT;
    int LEN;
    unsigned char BUF[MAXQ][MAXD];
} UART_QUEUE;

void Q_Check(void);
void UART1_RxCpltCallback(void);
void UART2_RxCpltCallback(void);
void UART1_Callback_uartError(void);
void UART2_Callback_uartError(void);
#ifdef __cplusplus
}
#endif

#endif