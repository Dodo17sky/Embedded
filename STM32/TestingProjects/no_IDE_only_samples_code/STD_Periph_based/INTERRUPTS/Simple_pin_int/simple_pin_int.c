#include "simple_pin_int.h"
#include "stm32f10x.h"

void Configure_PB12(void);

void Interrupt_normal_pin()
{
	/* Bit configuration structure for GPIOA PIN8 */
	GPIO_InitTypeDef gpio = { GPIO_Pin_13, GPIO_Speed_50MHz, GPIO_Mode_Out_PP };
	/* Enable PORT A clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* Initialize GPIOA: 50MHz, PIN8, Push-pull Output */
	GPIO_Init(GPIOB, &gpio);
	/* Turn off LED to start with */
	GPIO_SetBits(GPIOB, GPIO_Pin_13);

	/* Configure PB12 as interrupt */
	Configure_PB12();

	while (1) {

	}
}

void Configure_PB12(void)
{
	/* Set variables used */
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	/* Enable clock for GPIOB */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* Set pin as input */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Tell system that you will use PB12 for EXTI_Line12 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);

	EXTI_InitStruct.EXTI_Line = EXTI_Line12;		/* PB12 is connected to EXTI_Line12 */
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;			/* Enable interrupt */
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;/* Interrupt mode */
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling; /* Triggers on rising and falling edge */
	EXTI_Init(&EXTI_InitStruct); /* Add to EXTI */

	/* Add IRQ vector to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn; /* PB12 is connected to EXTI_Line12, which has EXTI15_10_IRQn vector */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00; /* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01; /* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; /* Enable interrupt */
	NVIC_Init(&NVIC_InitStruct); /* Add to NVIC */
}

/* Handle PB12 interrupt */
void EXTI15_10_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line12) != RESET) {
        /* Do your stuff when PB12 is changed */
    	uint8_t led_bit = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_13);

    	//===============================================
		/* If LED output set, clear it */
		if(led_bit == (uint8_t)Bit_SET)
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		}
		/* If LED output clear, set it */
		else
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_13);
		}
		//===============================================

        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
}
