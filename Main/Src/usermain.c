#include <main.h>
#include "../Inc/usermain.h"

void user_init() {
    
    mode = (Check_ExternPin() == OK ) ? MASTER : SLAVE; 
}

void user_main() {
    Q_Check();
}