/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "ComManager.h"
#include <Arduino.h>
#include "Serial.h"
#include "Controller.h"

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/
static String InMsg_UART;
/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/
#define INVALID_MSG_ID				0xFFFF
#define USER_LED					0x1

/*--------------------------------------------------------------------------------
*                           Functions prototypes declaration
*--------------------------------------------------------------------------------*/
static void Uart_MainFunction(void);
static void MessageInterpreter(void);
/*--------------------------------------------------------------------------------
*                           Functions prototypes definition
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
@name		ComM_InitRunnable
@brief		Initialize communication setups
*--------------------------------------------------------------------------------*/
void ComM_InitRunnable(void)
{
	InMsg_UART.reserve(100);
}

/*--------------------------------------------------------------------------------
@name		ComM_MainRunnable
@brief		Main communication task
*--------------------------------------------------------------------------------*/
void ComM_MainRunnable(void)
{
	Uart_MainFunction();

	MessageInterpreter();
}

/*--------------------------------------------------------------------------------
@name		Uart_MainFunction
@brief		Main UART function
*--------------------------------------------------------------------------------*/
static void Uart_MainFunction()
{
	InMsg_UART = Serial_Read();
}

/*--------------------------------------------------------------------------------
@name		MessageInterpreter
@brief		Analyze an UART message and
*--------------------------------------------------------------------------------*/
static void MessageInterpreter()
{
	if(InMsg_UART.startsWith("hey"))
	{
		Serial_Write("Hey to you :)\n");
	}
	else if(InMsg_UART.startsWith("ledon"))
	{
		Controller_SetOutput(USER_LED, HIGH);
	}
	else if(InMsg_UART.startsWith("ledoff"))
	{
		Controller_SetOutput(USER_LED, LOW);
	}
	else if(InMsg_UART != "")
	{
		Serial_Write("Reply [");
		Serial_Write(InMsg_UART);
		Serial_Write("]\n");
	}
}
