#include <main.h>
#include "../Inc/support.h"

CSTATUS Check_ExternPin() {
    CSTATUS ret;
    if (HAL_GPIO_ReadPin(EXTEND_GPIO_Port, EXTEND_Pin) == GPIO_PIN_SET) {
        ret = OK;
    } else {
        ret = FAIL;
    }
    return ret;
}

CSTATUS Change_ExtendPort(u8 output) {
    CSTATUS ret = OK;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = EXTEND_Pin;
    HAL_GPIO_DeInit(EXTEND_GPIO_Port, EXTEND_Pin);
    if (output == 1U) {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    } else {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    HAL_GPIO_Init(EXTEND_GPIO_Port, &GPIO_InitStruct);

    return ret;
}

void combineBytes(u8 *byte, u32 *combined) {
    *combined = 0U;
    *combined |= (u32)byte[0] << 24U;
    *combined |= (u32)byte[1] << 16U;
    *combined |= (u32)byte[2] << 8U;
    *combined |= (u32)byte[3];
}

void splitBytes(u32 combined, u8 *byte) {
    byte[0] = (u8)((combined >> 24U) & 0xFFU);
    byte[1] = (u8)((combined >> 16U) & 0xFFU);
    byte[2] = (u8)((combined >> 8U) & 0xFFU);
    byte[3] = (u8)(combined & 0xFFU);
}