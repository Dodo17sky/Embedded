#include "System.h"

int main(void)
{

    if(RETURN_NOK == System_Init())
    {
        /* This is an error state */
        while(1);
    }
    
    System_Scheduler();
    
    /* this should never be reached */
    System_Deinit();
    
    return 0;
}
