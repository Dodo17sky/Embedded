/*
 * STM32 driver for SPI ST7735
 *
 * Example usage with STM32F103C8T6
 *
 * ST7735 wiring example:
 * 1 VCC    - 3.3V Vcc
 * 2 GND    - GND
 * 3 CS     - PB5
 * 4 RESET  - PA12
 * 5 A0     - PA11
 * 6 SDA    - PB15 (SPI2 MOSI)
 * 7 SCK    - PB13 (SPI2 SCK)
 * 8 LED    - via resistor 68 Ohm to 3.3V Vcc
 *
 * Using hardware SPI2
 */

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "st7735.h"
#include "uart.h"

#include <stdio.h>

static __IO uint32_t 	TimingDelay = 0;
static __IO uint32_t    TimeElapsed = 0;
static void				AnalyzeContent(char* data, int len);
static int  			str_cmp(char* s1, char* s2, uint16_t nmb);
static void 			uartCmd_Led(char* data, int len);
static void 			uartCmd_Msg(char* data, int len);
static void 			uartComTest(char* data, int len);
static void 			uartComChat(char* data, int len);

void SysTick_Handler(void);
void Delay_US(__IO uint32_t nTime);

void Config_GPIO();
void Config_LEDs_GPIO();
void LedSwitch(uint8_t mask);

// =============== Macro and constants ===============

#define LED_YELLOW		GPIO_Pin_7
#define LED_RED			GPIO_Pin_1
#define LED_GREEN		GPIO_Pin_12

#define ZERO_FILL(x)		for(int i=0;i<(sizeof(x)/sizeof(x[0]));i++) x[i] = 0
#define COMPARE(s1,s2,n)	(str_cmp(s1,s2,n) == 0)

// =============== Global variables ===============
uint16_t 	msgNmb 			= 0;
uint8_t		startComTesting = 0;
uint8_t		startComChat	= 0;

int main(void)
{
	char buf[128] = {0};
	SysTick_Config(SystemCoreClock / 1000);

	// Configure all unused GPIO port pins in Analog Input mode
	Config_GPIO();
	Config_LEDs_GPIO();

	// UART init
	uGPIO_Init();
	uUSART1_Init();

	ST7735_Init();
	ST7735_AddrSet(0,0,159,127);
	ST7735_Clear(COLOR565_BLACK);
	ST7735_Orientation(scr_180);

	ST7735_Rect(0, 0, 127, 30, COLOR565_BLUE);
	ST7735_PutStr5x7(2, 20, 10, "C H A T", COLOR565_GREEN, COLOR565_BLACK);

	while(1)
	{
		if( isUartRx ) {
			getUartInput(buf, 128);
			AnalyzeContent(buf, 128);
		}

		if(startComTesting)
			uartComTest(buf, 128);

		if(startComChat)
			uartComChat(buf, 128);

		ZERO_FILL(buf);
	}
}

static void uartComTest(char* data, int len) {
	static uint32_t iter = 0;
	static uint32_t lastRun = 0;
	uint32_t		now = TimeElapsed;
	char			buf[128] = {0};

	if(now >= (lastRun+1000)) {
		lastRun = now;
		iter++;
		sprintf(buf, "Test com msg %d\n", (int)iter);
		user_UART_Send(buf, strlen(buf));
	}
}

static void uartComChat(char* data, int len) {
	char			buf[256] = {0};

	if(! data[0])
		return;

	sprintf(buf, "You said \"%s\"\n", data);
	user_UART_Send(buf, strlen(buf));
}

static void AnalyzeContent(char* data, int len) {
	if( COMPARE(data, "led:", 4) )
		uartCmd_Led(&data[4], len-4);

	if( COMPARE(data, "msg:", 4) )
		uartCmd_Msg(&data[4], len-4);

	if( COMPARE(data, "comtest", 7) ) {
		if(startComTesting == 1)
			startComTesting = 0;
		else
			startComTesting = 1;
		startComChat = 0;
	}

	if( COMPARE(data, "comchat", 7) ) {
		if(startComChat == 1)
			startComChat = 0;
		else
			startComChat = 1;
		startComTesting = 0;
	}
}

static void uartCmd_Led(char* data, int len){
	uint8_t mask = (uint8_t)(data[0] - '0');
	LedSwitch(mask);
	char txbuf[32] = {0};
	sprintf(txbuf, "Led %d on", mask);
	user_UART_Send(txbuf, strlen(txbuf));
}

static void uartCmd_Msg(char* data, int len){
	char txbuf[32] = {0};
	ST7735_FillRect(0, 65, 127, 159, COLOR565_BLACK);
	ST7735_PutStr5x7(1, 0, 65, data, COLOR565_WHITE, COLOR565_BLACK);
	msgNmb++;
	sprintf(txbuf, "Msg received %d", msgNmb);
	user_UART_Send(txbuf, strlen(txbuf));
}

void Config_GPIO()
{
	GPIO_InitTypeDef  gpio;
	// Configure all unused GPIO port pins in Analog Input mode

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
	gpio.GPIO_Pin = GPIO_Pin_All; gpio.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &gpio); GPIO_Init(GPIOB, &gpio); GPIO_Init(GPIOC, &gpio); GPIO_Init(GPIOD, &gpio); GPIO_Init(GPIOE, &gpio);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, DISABLE);
}

static int str_cmp(char* s1, char* s2, uint16_t nmb)
{
	for(int i=0; i<nmb; i++){
		if(s1[i] > s2[i])
			return 1;
		if(s1[i] < s2[i])
			return -1;
	}
	return 0;
}

void Config_LEDs_GPIO()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = LED_RED | LED_GREEN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);

	gpio.GPIO_Pin = LED_YELLOW;
	GPIO_Init(GPIOA, &gpio);

	GPIO_ResetBits(GPIOB, LED_RED | LED_GREEN);
	GPIO_ResetBits(GPIOA, LED_YELLOW);
}

void LedSwitch(uint8_t mask)
{
	uint8_t x = 17, y = 37;

	if(mask > 7)
		return;

	// Clear LEDs rectangles zone
	ST7735_FillRect(x-1, y-1, scr_w-1, y+21, COLOR565_BLACK);

	if(mask & 0x4) {
		GPIO_SetBits(GPIOB, LED_GREEN);
		ST7735_FillRect(x, y, x+20, y+20, COLOR565_GREEN);
	}
	else{
		GPIO_ResetBits(GPIOB, LED_GREEN);
		ST7735_Rect(x, y, x+20, y+20, COLOR565_GREEN);
	}
	x+=40;

	if(mask & 0x2) {
		GPIO_SetBits(GPIOB, LED_RED);
		ST7735_FillRect(x, y, x+20, y+20, COLOR565_RED);
	}
	else{
		GPIO_ResetBits(GPIOB, LED_RED);
		ST7735_Rect(x, y, x+20, y+20, COLOR565_RED);
	}
	x+=40;

	if(mask & 0x1) {
		GPIO_SetBits(GPIOA, LED_YELLOW);
		ST7735_FillRect(x, y, x+20, y+20, COLOR565_YELLOW);
	}
	else{
		GPIO_ResetBits(GPIOA, LED_YELLOW);
		ST7735_Rect(x, y, x+20, y+20, COLOR565_YELLOW);
	}

	Delay_US(2000);
}

void Delay_US(__IO uint32_t nTime)
{
  TimingDelay = nTime;
  while(TimingDelay != 0);
}

void SysTick_Handler()
{
  if (TimingDelay != 0) TimingDelay--;
  TimeElapsed++;
}
