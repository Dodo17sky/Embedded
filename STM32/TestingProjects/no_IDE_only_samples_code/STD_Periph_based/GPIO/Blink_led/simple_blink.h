#include "stm32f10x.h"

void delay(uint32_t time){
	while(time--)
		asm volatile ("nop");
}

void Simple_blink()
{
	// This struct describe our pin
	GPIO_InitTypeDef gpio;

	// Enable Port B clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_13;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &gpio);

	while(1) {
		// Led on
		GPIO_SetBits(GPIOB, GPIO_Pin_13);
		delay(3600000);

		// Led off
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		delay(3600000);
	}
}
