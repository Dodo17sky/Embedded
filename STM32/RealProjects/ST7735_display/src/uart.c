#include "uart.h"

// Function prototype
static void SendData(char* buffer, uint16_t length);

//---------------------------------------------------------
#define userLED_Port		GPIOB
#define userLED_Pin			GPIO_Pin_1
#define userLED				userLED_Port, userLED_Pin
//---------------------------------------------------------
#define boardLED_Port		GPIOC
#define boardLED_Pin		GPIO_Pin_13
#define boardLED			boardLED_Port, boardLED_Pin
//---------------------------------------------------------

//---------------------------------------------------------
char leds_off[] = "LED's are off";
char leds_on[]  = "LED's are on";
char led1_on[]  = "LED 1 is on";
char led2_on[]  = "LED 2 is on";
//---------------------------------------------------------

#define IN_BUF_LEN		128
char inBuf[IN_BUF_LEN];
uint8_t inBufPos = 0;
uint8_t inReady = 0;

void user_UART_Send(char* data, int length)
{
	SendData(data, length);
}

void uGPIO_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = userLED_Pin;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(userLED_Port, &gpio);

	gpio.GPIO_Pin = boardLED_Pin;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(boardLED_Port, &gpio);
}

void uUSART1_Init()
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef uart;

	// Enable clock for GPIO, AFIO, USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO  , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* GPIOA PIN9 alternative function Tx */
	gpio.GPIO_Pin	= GPIO_Pin_9;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio);

	/* GPIOA PIN10 alternative function Rx */
	gpio.GPIO_Pin	= GPIO_Pin_10;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);

	// Enable USART1
	USART_Cmd(USART1, ENABLE);

	// Configure UART
	uart.USART_BaudRate		= 9600;
	uart.USART_WordLength	= USART_WordLength_8b;
	uart.USART_StopBits		= USART_StopBits_1;
	uart.USART_Parity		= USART_Parity_No;
	uart.USART_Mode			= USART_Mode_Rx | USART_Mode_Tx;
	uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &uart);

	// Enable Rx interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	// Enable USART1 global interrupt
	NVIC_EnableIRQ(USART1_IRQn);
}

static void SendData(char* buffer, uint16_t length)
{
	uint16_t idx = 0;
	for(idx = 0; idx < length; idx++) {
		while(!(USART1->SR & USART_SR_TXE));
		USART_SendData(USART1, (uint16_t)buffer[idx]);
	}
}

uint8_t uartRxIndication()
{
	return inReady;
}

void getUartInput(char* buf, uint8_t len){
	for(int i=0; i<len && i<IN_BUF_LEN && inBuf[i]!=0; i++)
	{
		buf[i] = inBuf[i];
	}
	for(int i=0; i<IN_BUF_LEN; i++)
		inBuf[i] = 0;
	inReady = 0;
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		char data = (char)USART_ReceiveData(USART1);
		if(data == '\n'){
			inBuf[inBufPos] = 0;
			inBufPos = 0;
			inReady = 1;
		}
		else {
			inBuf[inBufPos] = data;
			++inBufPos;
			inReady = 0;
		}
	}
}
