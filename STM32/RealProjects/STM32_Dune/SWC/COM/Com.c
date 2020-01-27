/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Com.h"
#include "Uart.h"
#include "Spi.h"
#include "String.h"
#include "Utils.h"
#include "Wifi.h"

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/
#define UART_TO_PC1         USART1
#define UART_TO_PC2         USART2
#define UART_TO_PC3         USART3

/*--------------------------------------------------------------------------------
*                           Functions prototypes declaration
*--------------------------------------------------------------------------------*/
static void Com_Uart_MainFunction(void);
static void Com_SPI_MainFunction(void);

/*--------------------------------------------------------------------------------
*                           Functions prototypes definition
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
@name		Task_MainRunnable
@brief		This task run forever. This will launch all the task in this system
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void Com_MainRunnable(void)
{
    Com_Uart_MainFunction();
    
    Com_SPI_MainFunction();
    
    Wifi_MainFunction();
}

/*--------------------------------------------------------------------------------
@name		Com_InitRunnable
@brief		Initialize Com application
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void Com_InitRunnable(void)
{
    
}

/*--------------------------------------------------------------------------------
@name		Com_Uart_MainFunction
@brief		Main UART function that handles the Rx and Tx communication
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/
static void Com_Uart_MainFunction(void)
{
#if 0
    U8 readStatus = RETURN_NOK;
    UartRxBuffer_Type tmpBuf = {0};

#if (ENABLE_DEVICE_UART1 == ON)
    ZERO_FILL_ARRAY(tmpBuf);
    readStatus = Uart_Read_All(UART_TO_PC1, &tmpBuf);
    if(RETURN_OK == readStatus)
    {
        U8 dataLength = GetStrLength((char*)&tmpBuf);
        Uart_Send_String(UART_TO_PC1, "[1]:", 4);
        Uart_Send_String(UART_TO_PC1, (char*)tmpBuf, dataLength);
        Uart_Send_String(UART_TO_PC1, "\n", 1);
    }
#endif /* ENABLE_DEVICE_UART1 == ON */

#if (ENABLE_DEVICE_UART2 == ON)
    ZERO_FILL_ARRAY(tmpBuf);
    readStatus = Uart_Read_All(UART_TO_PC2, &tmpBuf);
    if(RETURN_OK == readStatus)
    {
        U8 dataLength = GetStrLength((char*)&tmpBuf);
        Uart_Send_String(UART_TO_PC2, "[2]:", 4);
        Uart_Send_String(UART_TO_PC2, (char*)tmpBuf, dataLength);
        Uart_Send_String(UART_TO_PC2, "\n", 1);
    }
#endif /* ENABLE_DEVICE_UART2 == ON */
    
#if (ENABLE_DEVICE_UART3 == ON)
    ZERO_FILL_ARRAY(tmpBuf);
    readStatus = Uart_Read_All(UART_TO_PC3, &tmpBuf);
    if(RETURN_OK == readStatus)
    {
        U8 dataLength = GetStrLength((char*)&tmpBuf);
        Uart_Send_String(UART_TO_PC3, "[3]:", 4);
        Uart_Send_String(UART_TO_PC3, (char*)tmpBuf, dataLength);
        Uart_Send_String(UART_TO_PC3, "\n", 1);
    }
#endif /* ENABLE_DEVICE_UART3 == ON */
#endif /* 0 */
}

/*--------------------------------------------------------------------------------
@name		Com_InitRunnable
@brief		Main SPI function that handles the Rx and Tx communication
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/
static void Com_SPI_MainFunction(void)
{
    static U8 TxData = 50;
    U8 RxData;

    RxData = Spi_Master_Transfer(TxData);
    TxData = RxData + 3;
}