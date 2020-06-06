/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Serial.h"

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/
static String SerialData;
static bool dataReceived;
/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Functions prototypes declaration
*--------------------------------------------------------------------------------*/
void SerialEvent(void);

/*--------------------------------------------------------------------------------
*                           Functions prototypes definition
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
@name		Serial_InitRunnable
@brief		This function shall initialize the UART and all data involved in
			Rx and Tx process
*--------------------------------------------------------------------------------*/
void Serial_InitRunnable(void)
{
	SerialData.reserve(150);
	dataReceived = false;

	Serial.begin(115200);
}

/*--------------------------------------------------------------------------------
@name		Serial_MainRunnable
@brief		Main UART task where RX and TX operations are done
*--------------------------------------------------------------------------------*/
void Serial_MainRunnable(void)
{
	SerialEvent();
}

/*--------------------------------------------------------------------------------
@name		Serial_Write
@brief		Send data over UART
*--------------------------------------------------------------------------------*/
void Serial_Write(String data)
{
	Serial.print(data);
}

/*--------------------------------------------------------------------------------
@name		Serial_Read
@brief		Receive data from UART
*--------------------------------------------------------------------------------*/
String Serial_Read()
{
	String outData = "";
	if(dataReceived)
	{
		outData = SerialData;
		dataReceived = false;
		SerialData.clear();
	}
	return outData;
}

/*--------------------------------------------------------------------------------
@name		SerialEvent
@brief		UART receives data in polling mode
*--------------------------------------------------------------------------------*/
inline void SerialEvent(void)
{
	while (Serial.available()) {
		char inChar = (char)Serial.read();

		if (inChar == '\n') {
			dataReceived = true;
		}
		else {
			SerialData += inChar;
		}
	}
}
