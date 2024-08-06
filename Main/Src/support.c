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

CSTATUS Change_Port() {
    CSTATUS ret = OK;


    return ret;
}