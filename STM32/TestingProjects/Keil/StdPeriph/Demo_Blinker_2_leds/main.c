#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

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

	// Enable Port C clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	gpio.GPIO_Pin = LED_EXTERN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &gpio);
	
	gpio.GPIO_Pin = LED_ONBOARD;
	GPIO_Init(GPIOC, &gpio);
	
	while(1){
		// Led on
		GPIO_SetBits(GPIOB, LED_EXTERN);
		GPIO_SetBits(GPIOC, LED_ONBOARD);
		delay(36000000);

		// Led off
		GPIO_ResetBits(GPIOB, LED_EXTERN);
		GPIO_ResetBits(GPIOC, LED_ONBOARD);
		delay(36000000);
	}
}