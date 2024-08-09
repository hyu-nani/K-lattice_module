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
#include <string.h>
#include "../Inc/uart.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

UART_HANDLE_Typedef huart;
UART_QUEUE UART1Send;
UART_QUEUE UART1Recv;
UART_QUEUE UART2Send;
UART_QUEUE UART2Recv;

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

#define UART_TIMEOUT_TX 10      /*ms*/
#define UART1_TIME_TX   30      /*ms*/
#define UART2_TIME_TX   30      /*ms*/

static void CMD_Parsing(UART_DIR uart, CMD_LIST cmd, u8* data) {
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
            break;
        default:
            break;
    }
}

static void UART1_EnQueue(u8* data, u8 len) {
    if (UART1Send.CNT < MAXQ) {
        memcpy(&UART1Send.BUF[UART1Send.HEAD][0], &data, len);
        UART1Send.CNT++;
        UART1Send.HEAD = (u32)((UART1Send.HEAD + (u32)1U) & MAXQ);
    }
}

static void UART2_EnQueue(u8* data, u8 len) {
    if (UART2Send.CNT < MAXQ) {
        memcpy(&UART2Send.BUF[UART2Send.HEAD][0], &data, len);
        UART2Send.CNT++;
        UART2Send.HEAD = (u32)((UART2Send.HEAD + (u32)1U) & MAXQ);
    }
}

void Clear_Q(UART_DIR Q) {
    switch (Q) {
        case UART1_SEND:
            memset(&UART1Send.BUF[UART1Send.TAIL][0], 0x00U, MAXD);
            UART1Send.CNT -= (UART1Send.CNT > 0U) ? 1U : 0U;
            UART1Send.TAIL = (u32)((UART1Send.TAIL + (u32)1U) % MAXQ);
            break;
        case UART1_RECV:
            memset(&UART1Recv.BUF[UART1Recv.TAIL][0], 0x00U, MAXD);
            UART1Recv.CNT -= (UART1Recv.CNT > 0U) ? 1U : 0U;
            UART1Recv.TAIL = (u32)((UART1Recv.TAIL + (u32)1U) % MAXQ);
            break;
        case UART2_SEND:
            memset(&UART2Send.BUF[UART2Send.TAIL][0], 0x00U, MAXD);
            UART2Send.CNT -= (UART2Send.CNT > 0U) ? 1U : 0U;
            UART2Send.TAIL = (u32)((UART2Send.TAIL + (u32)1U) % MAXQ);
            break;
        case UART2_RECV:
            memset(&UART2Recv.BUF[UART2Recv.TAIL][0], 0x00U, MAXD);
            UART2Recv.CNT -= (UART2Recv.CNT > 0U) ? 1U : 0U;
            UART2Recv.TAIL = (u32)((UART2Recv.TAIL + (u32)1U) % MAXQ);
            break;
        default:
            break;
    }
}


static void UART1_Recv_Q_Check(void) {
    CSTATUS status = OK;
    u32 dat;
    if (UART1Recv.HEAD != UART1Recv.TAIL) {
        memcpy(huart.cmp, &UART1Send.BUF[UART1Send.TAIL][0], MAXD);
        status = (huart.cmp[0] != STX) ? FAIL : OK;
        status = (huart.cmp[Protocol_len - 1U] != ETX) ? FAIL : OK;

        if (status == OK) {
            huart.cnt = huart.cmp[1];
            huart.cmd = huart.cmp[2];
            if (huart.cnt == ACT_CNT) {
                combineBytes(&huart.cmp[4], &dat);
                if (huart.cmd == CMD_GET_NUM_DEVICE) {
                    dat++;
                    splitBytes(dat, &huart.cmp[3]);
                    UART2_EnQueue(&huart.cmd, Protocol_len);
                } 
                else {
                    CMD_Parsing(UART1_RECV, (CMD_LIST)huart.cmd, &huart.cmp[3]);
                }
            }
            else {
                huart.cmp[1]--;
                UART2_EnQueue(&huart.cmp[0], Protocol_len);
            }
        }
        Clear_Q(UART1_RECV);
    }
}

static void UART1_Send_Q_Check(u32 time) {
    if ((UART1Send.HEAD != UART1Send.TAIL) && ((time-huart.delayTx1) >= UART1_TIME_TX)) {
        huart.delayTx1 = time;
        HAL_UART_Transmit(UART1_HANDLE, &UART1Send.BUF[UART1Send.TAIL][0], Protocol_len, UART_TIMEOUT_TX);
        Clear_Q(UART1_SEND);
    }
}

static void UART2_Recv_Q_Check(void) {
    if (UART2Recv.HEAD != UART2Recv.TAIL) {
        
        Clear_Q(UART2_RECV);
    }
}

static void UART2_Send_Q_Check(u32 time) {
    if ((UART2Send.HEAD != UART2Send.TAIL) && ((time-huart.delayTx2) >= UART2_TIME_TX)) {
        huart.delayTx2 = time;
        HAL_UART_Transmit(UART2_HANDLE, &UART2Send.BUF[UART2Send.TAIL][0], Protocol_len, UART_TIMEOUT_TX);
        Clear_Q(UART2_SEND);
    }
}

void UART_INIT(void) {
    huart.delayTx1 = 0U;
    huart.delayTx2 = 0U;
}

void UART_PROC(void) {
    UART1_Recv_Q_Check();
    UART1_Send_Q_Check(HAL_GetTick());
    UART2_Recv_Q_Check();
    UART2_Send_Q_Check(HAL_GetTick());
}

void UART1_RxCpltCallback(void) {
    huart.len = UART1_RX_LEN - UART1_RX_CNT;
    if ((huart.len > 0U) && (UART1Recv.CNT < MAXQ)) {
        memcpy(&UART1Recv.BUF[UART1Recv.HEAD][UART1Recv.IDX], huart.rxd, huart.len);
        UART1Recv.IDX += huart.len;
        if (UART1Recv.IDX >= Protocol_len) {
            UART1Recv.IDX = 0U;
            UART1Recv.CNT++;
            UART1Recv.HEAD = (u32)((UART1Recv.HEAD + (u32)1U) & MAXQ);
        }
    }
    if (__HAL_UART_GET_FLAG(UART1_HANDLE, UART_FLAG_IDLE) != RESET) {
        __HAL_UART_CLEAR_IDLEFLAG(UART1_HANDLE);
        HAL_UART_AbortReceive_IT(UART1_HANDLE);
    }
    HAL_UART_Receive_IT(UART1_HANDLE, huart.rxd, UART1_RX_LEN);
}

void UART2_RxCpltCallback(void) {
    huart.len = UART2_RX_LEN - UART2_RX_CNT;
    if ((huart.len > 0U) && (UART2Recv.CNT < MAXQ)) {
        memcpy(&UART1Recv.BUF[UART1Recv.HEAD][UART1Recv.IDX], huart.rxd, huart.len);
        UART1Recv.IDX += huart.len;
    }
    HAL_UART_Receive_IT(UART2_HANDLE, huart.rxd, UART2_RX_LEN);
}

void UART1_Callback_uartError(void) {
    HAL_UART_AbortReceive_IT(UART1_HANDLE);
    HAL_UART_Receive_IT(UART1_HANDLE, huart.rxd, UART1_RX_LEN);
}
void UART2_Callback_uartError(void) {
    HAL_UART_AbortReceive_IT(UART2_HANDLE);
    HAL_UART_Receive_IT(UART2_HANDLE, huart.rxd, UART2_RX_LEN);
}
