#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

static __IO uint32_t 	TimingDelay = 0;

void SysTick_Handler(void);
void Delay_MS(__IO uint32_t nTime);

#define LED_ONBOARD	GPIO_Pin_13
#define LED_EXTERN	GPIO_Pin_12

void delay(uint32_t time){
	while(time--) {
		__ASM volatile ("nop");
	}
}

int main()
{
	// This struct describe our pin
	GPIO_InitTypeDef gpio;
    
    SysTick_Config(SystemCoreClock / 1000);

	// Enable Port C clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;	
	gpio.GPIO_Pin = LED_ONBOARD;
	GPIO_Init(GPIOC, &gpio);
	
	while(1){
		// Led on
		GPIO_SetBits(GPIOC, LED_ONBOARD);
		Delay_MS(5000);

		// Led off
		GPIO_ResetBits(GPIOC, LED_ONBOARD);
		Delay_MS(5000);
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