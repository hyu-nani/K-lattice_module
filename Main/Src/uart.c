/**
 * @file uart.c
 * @author Caldronear (hyu-nani@github.com)
 * @brief uart and protocol
 * @version 0.1
 * @date 2024-08-07
 * 
 * @copyright Copyright (c) 2024
 * @note 
 * protocol is [ STX/CNT/CMD/DAT0/DAT1/DAT2/DAT3/DAT4/ETX ]
 * when the CNT is 0x00, then active device (cascade)
 */
#include <main.h>
#include "../Inc/uart.h"

UART_QUEUE UART1Send;
UART_QUEUE UART1Recv;
UART_QUEUE UART2Send;
UART_QUEUE UART2Recv;

u8 rxd[MAXD];
u8 cmp[MAXD];

#define UART1_HANDLE    &huart1 
#define UART1_IRQn      USART1_IRQn
#define UART1_RX_CNT    (UART1_HANDLE)->RxXferCount
#define UART1_RX_LEN    (UART1_HANDLE)->RxXferSize

#define UART2_HANDLE    &huart2 
#define UART2_IRQn      USART2_IRQn
#define UART2_RX_CNT    (UART2_HANDLE)->RxXferCount
#define UART2_RX_LEN    (UART2_HANDLE)->RxXferSize

#define Protocol_len    9U
#define ACT_CNT         0x01U
#define STX             0xFFU
#define ETX             0xF0U

CSTATUS UART2_SendQ() {
    
}

void Clear_Q(UART_DIR Q) {
    switch (Q) {
        case UART1_SEND:
            memset(UART1Send.BUF[UART1Send.TAIL][0], 0x00U, MAXD);
            UART1Send.CNT -= (UART1Send.CNT > 0U) ? 1U : 0U;
            UART1Send.TAIL = (u32)((UART1Send.TAIL + (u32)1U) % MAXQ);
            break;

        case UART1_RECV:
            memset(UART1Recv.BUF[UART1Recv.TAIL][0], 0x00U, MAXD);
            UART1Recv.CNT -= (UART1Recv.CNT > 0U) ? 1U : 0U;
            UART1Recv.TAIL = (u32)((UART1Recv.TAIL + (u32)1U) % MAXQ);
            break;

        case UART2_SEND:
            memset(UART2Send.BUF[UART2Send.TAIL][0], 0x00U, MAXD);
            UART2Send.CNT -= (UART2Send.CNT > 0U) ? 1U : 0U;
            UART2Send.TAIL = (u32)((UART2Send.TAIL + (u32)1U) % MAXQ);
            break;

        case UART2_RECV:
            memset(UART2Recv.BUF[UART2Recv.TAIL][0], 0x00U, MAXD);
            UART2Recv.CNT -= (UART2Recv.CNT > 0U) ? 1U : 0U;
            UART2Recv.TAIL = (u32)((UART2Recv.TAIL + (u32)1U) % MAXQ);
            break;

        default:
            break;
    }
}
static void CMD_Parsing(UART_DIR uart, CMD_LIST cmd, u32* data) {
    switch(cmd) {
        case CMD_SET_CHAR:
            if (uart == UART1_RECV) {
                
            }
            break;
        case CMD_SET_COLOR:
            if (uart == UART1_RECV) {

            }
            break;
        case CMD_GET_NUM_DEVICE:
            data++;

            break;
        default:
            break;
    }
}

static void UART1_Recv_Q_Check() {
    CSTATUS status = OK;
    u32 dat;
    if (UART1Recv.HEAD != UART1Recv.TAIL) {
        memcpy(cmp, UART1Send.BUF[UART1Send.TAIL][0], MAXD);
        status |= isNOEQ_RET_USER(cmp[0], STX, FAIL);
        status |= isNOEQ_RET_USER(cmp[Protocol_len - 1U], ETX, FAIL);
        if (status == OK) {
            if (cmp[1] == ACT_CNT) {
                for (u8 i = 0U; i < 4U; i++) {
                    dat |= (u32)((u32)cmp[i + 3U] << (8U * i));
                }
                CMD_Parsing(UART1_RECV, cmp[2], &dat);
            }
            else {
                cmp[1]--;
                memcpy(UART2Send.BUF[UART2Send.HEAD][0], cmp, Protocol_len);
                UART2Send.CNT++;
                UART2Send.HEAD = (u32)((UART2Send.HEAD + (u32)1U) & MAXQ);
            }
        }
        Clear_Q(UART1_RECV);
    }
}

static void UART1_Send_Q_Check() {
    if (UART1Send.HEAD != UART1Send.TAIL) {
        HAL_UART_Transmit_IT(UART1_HANDLE, UART1Send.BUF[UART1Send.TAIL][0], Protocol_len);
        Clear_Q(UART1_SEND);
    }
}

static void UART2_Recv_Q_Check() {
    if (UART2Recv.HEAD != UART2Recv.TAIL) {
        
        Clear_Q(UART2_RECV);
    }
}

static void UART2_Send_Q_Check() {
    if (UART2Send.HEAD != UART2Send.TAIL) {
        HAL_UART_Transmit_IT(UART2_HANDLE, UART2Send.BUF[UART2Send.TAIL][0], Protocol_len);
        Clear_Q(UART2_SEND);
    }
}

void Q_Check() {
    UART1_Recv_Q_Check();
    UART1_Send_Q_Check();
    UART2_Recv_Q_Check();
    UART2_Send_Q_Check();
}

void UART1_RxCpltCallback() {
    UART1Recv.LEN = UART1_RX_LEN - UART1_RX_CNT;
    if ((UART1Recv.LEN > 0U) && (UART1Recv.CNT < MAXQ)) {
        memcpy(UART1Recv.BUF[UART1Recv.HEAD][UART1Recv.IDX], rxd, UART1Recv.LEN);
        UART1Recv.IDX += UART1Recv.LEN;
        if (UART1Recv.IDX >= Protocol_len) {
            UART1Recv.IDX = 0U;
            UART1Recv.CNT++;
            UART1Recv.HEAD = (u32)((UART1Recv.HEAD + (u32)1U) & MAXQ);
        }
    }
    if (__HAL_UART_GET_FLAG(UART1_HANDLE, UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(UART1_HANDLE);
        HAL_UART_AbortReceive_IT(UART1_HANDLE);
    }
    HAL_UART_Receive_IT(UART1_HANDLE, rxd, UART1_RX_LEN);
}

void UART2_RxCpltCallback() {
    UART2Recv.LEN = UART2_RX_LEN - UART2_RX_CNT;
    if ((UART2Recv.LEN > 0U) && (UART2Recv.CNT < MAXQ)) {
        memcpy(UART1Recv.BUF[UART1Recv.HEAD][UART1Recv.IDX], rxd, UART1Recv.LEN);
        UART1Recv.IDX += UART1Recv.LEN;
    }
    HAL_UART_Receive_IT(UART2_HANDLE, rxd, UART2_RX_LEN);
}

void UART1_Callback_uartError(void) {
    HAL_UART_AbortReceive_IT(UART1_HANDLE);
    HAL_UART_Receive_IT(UART1_HANDLE, rxd, UART1_RX_LEN);
}
void UART2_Callback_uartError(void) {
    HAL_UART_AbortReceive_IT(UART2_HANDLE);
    HAL_UART_Receive_IT(UART2_HANDLE, rxd, UART2_RX_LEN);
}
