#include <main.h>
#include "../Inc/usermain.h"

USRM_HANDLE_Typedef Uhandle;

void user_init() {
    Uhandle.mode = (Check_ExternPin() == OK ) ? MASTER : SLAVE; 
}

void user_main() {
    Q_Check();
}