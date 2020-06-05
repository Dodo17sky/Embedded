/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "ComManager.h"

#if (SWC_COMMANAGER_ENABLE == ON)

#include "Uart.h"
#include "Spi.h"
#include "String.h"
#include "Utils.h"

#include "Rte_ComManager.h"

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/
static U8 lastBtnStatus = 0;
/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Functions prototypes declaration
*--------------------------------------------------------------------------------*/
static void Com_Uart_MainFunction(void);
static void Com_SPI_MainFunction(void);
static void ComM_SensorCheck(void);
static void ComM_UartReply(void);

/*--------------------------------------------------------------------------------
*                           Functions prototypes definition
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
@name		Task_MainRunnable
@brief		This task run forever. This will launch all the task in this system
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void ComManager_MainRunnable(void)
{
    Com_Uart_MainFunction();
    
    Com_SPI_MainFunction();
}

/*--------------------------------------------------------------------------------
@name		Com_InitRunnable
@brief		Initialize Com application
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void ComManager_InitRunnable(void)
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
    ComM_UartReply();
    
    ComM_SensorCheck();
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

/*--------------------------------------------------------------------------------
@name		ComM_SensorCheck
@brief		This function check sensors status and notify by a com message
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/
static void ComM_SensorCheck(void)
{
    struct Sensor_StatusType pushBtn;
    Rte_ComManager_ReadSensor(0, &pushBtn);
    
    if(pushBtn.value != lastBtnStatus)
    {
        /* Message for user terminal */
        Uart_Send_String(USART3, "STM32 led = ", 12);
        Uart_Send_UInteger(USART3, (U32)pushBtn.value);
        Uart_Send_Byte(USART3, '\n');
        
        /* Message to ESP8266 */
        Uart_Send_UInteger(USART2, (U32)pushBtn.value);
        
        lastBtnStatus = pushBtn.value;
    }
}

/*--------------------------------------------------------------------------------
@name		ComM_UartReply
@brief		Check messages received on Uart and reply with a message
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/
static void ComM_UartReply(void)
{
    U8 bytesRead = 0;
    UartRxBuffer_Type tmpBuf = {0};

#if (ENABLE_DEVICE_UART1 == ON)
    ZERO_FILL_ARRAY(tmpBuf);
    bytesRead = Uart_RB_Read(USART1, (char*)&tmpBuf, ARRAY_SIZE(tmpBuf));
    if(bytesRead > 0)
    {
        U8 dataLength = GetStrLength((char*)&tmpBuf);
        Uart_Send_String(USART1, "[1]:", 4);
        Uart_Send_String(USART1, (char*)tmpBuf, dataLength);
        Uart_Send_String(USART1, "\n", 1);
    }
#endif /* ENABLE_DEVICE_UART1 == ON */

#if (ENABLE_DEVICE_UART2 == ON && 0)
    ZERO_FILL_ARRAY(tmpBuf);
    bytesRead = Uart_RB_Read(USART2, (char*)&tmpBuf, ARRAY_SIZE(tmpBuf));
    if(bytesRead > 0)
    {
        U8 dataLength = GetStrLength((char*)&tmpBuf);
        Uart_Send_String(USART2, "[2]:", 4);
        Uart_Send_String(USART2, (char*)tmpBuf, dataLength);
        Uart_Send_String(USART2, "\n", 1);
    }
#endif /* ENABLE_DEVICE_UART2 == ON */
    
#if (ENABLE_DEVICE_UART3 == ON)
    ZERO_FILL_ARRAY(tmpBuf);
    bytesRead = Uart_RB_Read(USART3, (char*)&tmpBuf, ARRAY_SIZE(tmpBuf));
    if(bytesRead > 0)
    {
        U8 dataLength = GetStrLength((char*)&tmpBuf);
        Uart_Send_String(USART3, "[3]:", 4);
        Uart_Send_String(USART3, (char*)tmpBuf, dataLength);
        Uart_Send_String(USART3, "\n", 1);
    }
#endif /* ENABLE_DEVICE_UART3 == ON */
}

#else /* (SWC_COMMANAGER_ENABLE == ON) */

void ComManager_InitRunnable(void) {}
void ComManager_MainRunnable(void) {}

#endif /* (SWC_COMMANAGER_ENABLE == ON) */