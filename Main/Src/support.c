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
    HAL_GPIO_DeInit(EXTEND_GPIO_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = EXTEND_Pin;
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
