/**
  ******************************************************************************
  * @file    main.c
  * @author  Dorin
  * @version 1.0
  * @date    05-June-2018
  * @brief   Default main function.
  * @description
  * 	Simple blink app. Led is toggles on/off at 1 second
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"
			
void simple_delay(uint32_t us)
{
	/* simple delay loop */
	while (us--)
	{
		asm volatile ("nop");
	}
}

/* system entry point */
int main(void)
{
	/* gpio init struct */
	GPIO_InitTypeDef gpio;


	//	Clk_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	/* initialize gpio structure */
	gpio.GPIO_Pin = GPIO_Pin_13;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;

	/* apply configuration */
	GPIO_Init(GPIOC, &gpio);

	/* main program loop */
	for (;;)
	{
		/* set led on */
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		/* delay */
		simple_delay(900000);
		/* clear led */
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		/* delay */
		simple_delay(900000);
	}

	/* never reached */
	return 0;
}
