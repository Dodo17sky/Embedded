#ifndef USER_UART_H
#define USER_UART_H

#include "stm32f10x.h"

void uUSART1_Init();
void uGPIO_Init();
uint8_t uartRxIndication();
void getUartInput(char* buf, uint8_t len);
void user_UART_Send(char* data, int length);

// Macro and constants

#define 	isUartRx		(uartRxIndication())


#endif // USER_UART_H
