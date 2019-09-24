/*
 *  Define delay milliseconds function, based on a global variable
 *  which is updated from an ISR at 1 ms
 *  This is a blocking function
 */

#include "stm32f10x.h"          // for SysTick_Config()
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

static __IO uint32_t 	TimingDelay = 0;

void SysTick_Handler(void);
void Delay_MS(__IO uint32_t nTime);

int main(void)
{
    // Nomatter what is the freq. of CPU,
    // the System Tick Interrupt will be called 1000 times/second,
    // meaning at 1 millisecond
	SysTick_Config(SystemCoreClock / 1000);

	while(1)
	{
        // do something
        Delay_MS(1000); // delay 1 second
        // do something
        Delay_MS(697);  // delay 697 milliseconds
	}
}

void Delay_MS(__IO uint32_t nTime)
{
  TimingDelay = nTime;
  while(TimingDelay != 0);
}

void SysTick_Handler()
{
  if (TimingDelay != 0) TimingDelay--;
}
