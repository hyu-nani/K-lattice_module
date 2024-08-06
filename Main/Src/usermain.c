#include "../Inc/usermain.h"

UART_QUEUE UART1Send;
UART_QUEUE UART1Recv;
UART_QUEUE UART2Send;
UART_QUEUE UART2Recv;

static void Clear_Q(UART_DIR Q) {
    switch (Q) {
        case UART1_SEND:

            UART1Send.CNT += (UART1Send.CNT < MAXQ) ? 1U : 0U;
            UART1Send.TAIL = (u32)((UART1Send.TAIL + (u32)1U) % MAXQ);
            break;

        case UART1_RECV:

            UART1Recv.CNT += (UART1Recv.CNT < MAXQ) ? 1U : 0U;
            UART1Recv.TAIL = (u32)((UART1Recv.TAIL + (u32)1U) % MAXQ);
            break;

        case UART2_SEND:

            UART2Send.CNT += (UART2Send.CNT < MAXQ) ? 1U : 0U;
            UART2Send.TAIL = (u32)((UART2Send.TAIL + (u32)1U) % MAXQ);
            break;

        case UART2_RECV:

            UART2Recv.CNT += (UART2Recv.CNT < MAXQ) ? 1U : 0U;
            UART2Recv.TAIL = (u32)((UART2Recv.TAIL + (u32)1U) % MAXQ);
            break;

        default:
            break;
    }
}

static void UART1_Recv_Q_Check() {
    if (UART1Recv.HEAD != UART1Recv.TAIL) {
        
        Clear_Q(UART1_RECV);
    }
}

static void UART1_Send_Q_Check() {
    if (UART1Send.HEAD != UART1Send.TAIL) {
        
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
        
        Clear_Q(UART2_SEND);
    }
}

static void Q_Check() {
    UART1_Recv_Q_Check();
    UART1_Send_Q_Check();
    UART2_Recv_Q_Check();
    UART2_Send_Q_Check();
}

void user_init() {

}

void user_main() {
    Q_Check();
}