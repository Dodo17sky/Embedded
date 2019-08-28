
/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	CMSIS Multiple Threads Example

*----------------------------------------------------------------------------*/
#include "STM32F10x.h"
#include "cmsis_os.h"
#include "Board_LED.h"

/*----------------------------------------------------------------------------
  Simple delay loop 
 *---------------------------------------------------------------------------*/

void delay (unsigned int count)
{
unsigned int index;

	for(index =0;index<count;index++)
	{
		;
	}
}

/*----------------------------------------------------------------------------
  led Flasher thread
 *---------------------------------------------------------------------------*/
void ledSwitcher (void const *argument) 
{
	if ( (uint8_t)argument == 1 ) {
		osDelay(400);
	}
	for (;;) 
	{
		LED_On((uint32_t)argument);                          
		osDelay(1000);
		LED_Off((uint32_t)argument);
		osDelay(1000);
	}
}

//define the threads allow two instances of ledSwitcher
osThreadId main_ID,led_ID1,led_ID2;	
osThreadDef(ledSwitcher, osPriorityAboveNormal, 2, 0);

/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) 
{
	osKernelInitialize ();                    // initialize CMSIS-RTOS
	
	LED_Initialize();
	led_ID1 = osThreadCreate(osThread(ledSwitcher),(void *) 1UL);		//Create first instance of ledswitcher
	led_ID2 = osThreadCreate(osThread(ledSwitcher),(void *) 2UL);		//Create a second instance of ledSwitcher
	
	osKernelStart ();                         // start thread execution 
}
