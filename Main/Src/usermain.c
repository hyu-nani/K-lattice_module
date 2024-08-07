/**
 * @file usermain.c
 * @author Caldronear (hyu-nani@github.com)
 * @brief Korea Traditional Lattice Segment module
 * @version 0.1
 * @date 2024-08-07
 * 
 * @copyright Copyright (c) 2024 by Caldronear
 * @note WS2812 led
 */
#include <main.h>
#include "../Inc/usermain.h"

USRM_HANDLE_Typedef Uhandle;

void user_init() {
    Uhandle.mode = (Check_ExternPin() == OK ) ? MASTER : SLAVE; 
}

void user_main() {
    Q_Check();
}

HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if      (huart->Instance == USART1) {UART1_RxCpltCallback();}
    else if (huart->Instance == USART2) {UART2_RxCpltCallback();}
    else    {CNOP;}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    __HAL_UART_CLEAR_OREFLAG(huart);
    __HAL_UART_CLEAR_NEFLAG(huart);
    __HAL_UART_CLEAR_PEFLAG(huart);
    __HAL_UART_CLEAR_FEFLAG(huart);

    if      (huart->Instance == USART1) {UART1_Callback_uartError();}
    else if (huart->Instance == USART2) {UART2_Callback_uartError();}
    else    {CNOP;}
}