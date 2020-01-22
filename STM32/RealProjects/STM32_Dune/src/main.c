#include "System.h"

/* TMP CODE START */
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "Timer.h"
/* TMP CODE STOP */


int main(void)
{     
    if(RETURN_NOK == System_Init())
    {
        while(1);
    }
    
    System_Scheduler();
    
    /* this should never be reached */
    System_Deinit();
    
    return 0;
}
