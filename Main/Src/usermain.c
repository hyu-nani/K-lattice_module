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

#define MAINLOOP_TIME  10   /*ms*/

void user_init() {
    Uhandle.mode = (Check_ExternPin() == OK ) ? MASTER : SLAVE; 
    Uhandle.tick_main = HAL_GetTick();
    LED_Init();
}

void user_main() {
    /* per main time */
    
    /* end */
    while((HAL_GetTick() - Uhandle.tick_main) < MAINLOOP_TIME) {
        Q_Check();
    }
    Uhandle.tick_main = HAL_GetTick();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
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

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
    resetLedSendFlag();
}