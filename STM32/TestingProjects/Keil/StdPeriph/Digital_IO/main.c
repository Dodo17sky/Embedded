#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;	
	gpio.GPIO_Pin = LED_ONBOARD;
	GPIO_Init(GPIOC, &gpio);
	
	while(1){
		// Led on
		GPIO_SetBits(GPIOC, LED_ONBOARD);
		delay(10000000);

		// Led off
		GPIO_ResetBits(GPIOC, LED_ONBOARD);
		delay(2000000);
	}
}