/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Uart.h"
#include "String.h"
#include "RingBuffer.h"
#include "Utils.h"

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/

#if (ENABLE_DEVICE_UART1 == ON)
    static char U1_RxBuffer[UART_RX_BUFFER_SIZE];
    static RingBuffer_Type RB_U1;
#endif /* ENABLE_DEVICE_UART1 == ON */

#if (ENABLE_DEVICE_UART2 == ON)
    static char U2_RxBuffer[UART_RX_BUFFER_SIZE];
    static RingBuffer_Type RB_U2;
#endif /* ENABLE_DEVICE_UART2 == ON */
    
#if (ENABLE_DEVICE_UART3 == ON)
    static char U3_RxBuffer[UART_RX_BUFFER_SIZE];
    static RingBuffer_Type RB_U3;
#endif /* ENABLE_DEVICE_UART3 == ON */

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/
static U8 Init_LowDriver(void);
static inline U8 Uart_SendOneByte(USART_TypeDef* UARTx, U8 byte);

/*--------------------------------------------------------------------------------
@name       Uart_Init
@brief      
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U8 Uart_Init(void)
{
    U8 initStatus = RETURN_OK;

#if UART_IS_ENABLED
#if (ENABLE_DEVICE_UART1 == ON)
    RB_Init(&RB_U1, U1_RxBuffer, ARRAY_SIZE(U1_RxBuffer));
#endif /* ENABLE_DEVICE_UART1 == ON */

#if (ENABLE_DEVICE_UART2 == ON)
    RB_Init(&RB_U2, U2_RxBuffer, ARRAY_SIZE(U2_RxBuffer));
#endif /* ENABLE_DEVICE_UART2 == ON */
    
#if (ENABLE_DEVICE_UART3 == ON)
    RB_Init(&RB_U3, U3_RxBuffer, ARRAY_SIZE(U3_RxBuffer));
#endif /* ENABLE_DEVICE_UART3 == ON */
    
    initStatus = Init_LowDriver();
    
    /* Mark UART channels initialization */
    Uart_Send_String(USART1, "Hi! This is UART1\n", 18);
    Uart_Send_String(USART2, "Hi! This is UART2\n", 18);
    Uart_Send_String(USART3, "Hi! This is UART3\n", 18);
#endif /* UART_IS_ENABLED */

    return initStatus;
}

/*--------------------------------------------------------------------------------
@name       Uart_Deinit
@brief      
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U8 Uart_Deinit(void)
{
    U8 retCode = RETURN_OK;
    
    return retCode;
}

/*--------------------------------------------------------------------------------
@name       Uart_Init
@brief      
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
static U8 Init_LowDriver(void)
{
    U8 retCode = RETURN_OK;
    
    USART_InitTypeDef USART_InitStructure;
    
    /*
     *    Enable UART peripheral clock
     */
#if (ENABLE_DEVICE_UART1 == ON)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#endif /* ENABLE_DEVICE_UART1 == ON */

#if (ENABLE_DEVICE_UART2 == ON)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
#endif /* ENABLE_DEVICE_UART2 == ON */
    
#if (ENABLE_DEVICE_UART3 == ON)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
#endif /* ENABLE_DEVICE_UART3 == ON */
    
    /*
     *    Configure and initialize usart
     */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
#if (ENABLE_DEVICE_UART1 == ON)
    USART_Init(USART1, &USART_InitStructure);
    /* Enable USART */
    USART_Cmd(USART1, ENABLE);   
    /* Enable RXNE interrupt */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    /* Enable USART global interrupt */
    NVIC_EnableIRQ(USART1_IRQn);
#endif /* ENABLE_DEVICE_UART1 == ON */

#if (ENABLE_DEVICE_UART2 == ON)
    USART_Init(USART2, &USART_InitStructure);
    /* Enable USART */
    USART_Cmd(USART2, ENABLE);   
    /* Enable RXNE interrupt */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    /* Enable USART global interrupt */
    NVIC_EnableIRQ(USART2_IRQn);
#endif /* ENABLE_DEVICE_UART2 == ON */
    
#if (ENABLE_DEVICE_UART3 == ON)
    USART_Init(USART3, &USART_InitStructure);
    /* Enable USART */
    USART_Cmd(USART3, ENABLE);   
    /* Enable RXNE interrupt */
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    /* Enable USART global interrupt */
    NVIC_EnableIRQ(USART3_IRQn);
#endif /* ENABLE_DEVICE_UART3 == ON */
        
    return retCode;
}

/*--------------------------------------------------------------------------------
@brief      Send 1 byte on UART
*--------------------------------------------------------------------------------*/
static inline U8 Uart_SendOneByte(USART_TypeDef* UARTx, U8 byte)
{
    U8 sendStatus = RETURN_NOK;
    
#if (ENABLE_DEVICE_UART1 == ON)
    if(USART1 == UARTx)
    {
        USART_SendData(UARTx, (U16)byte);
        while (USART_GetFlagStatus(UARTx, USART_FLAG_TXE) == RESET);
        sendStatus = RETURN_OK;
    }
#endif /* ENABLE_DEVICE_UART1 == ON */

#if (ENABLE_DEVICE_UART2 == ON)
    if(USART2 == UARTx)
    {
        USART_SendData(UARTx, (U16)byte);
        while (USART_GetFlagStatus(UARTx, USART_FLAG_TXE) == RESET);
        sendStatus = RETURN_OK;
    }
#endif /* ENABLE_DEVICE_UART2 == ON */
    
#if (ENABLE_DEVICE_UART3 == ON)
    if(USART3 == UARTx)
    {
        USART_SendData(UARTx, (U16)byte);
        while (USART_GetFlagStatus(UARTx, USART_FLAG_TXE) == RESET);
        sendStatus = RETURN_OK;
    }
#endif /* ENABLE_DEVICE_UART3 == ON */
    
    return sendStatus;
}

/*--------------------------------------------------------------------------------
@name       Uart_Send_Byte
@brief      Send one byte
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U8 Uart_Send_Byte(USART_TypeDef* UARTx, U8 data)
{
    U8 sendStatus = RETURN_NOK;
    
#if UART_IS_ENABLED    
    sendStatus = Uart_SendOneByte(UARTx, data);
#endif /* UART_IS_ENABLED */

    return sendStatus;
}

/*--------------------------------------------------------------------------------
@name       Uart_Send_ByteArray
@brief      Send multiple bytes
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U8 Uart_Send_ByteArray(USART_TypeDef* UARTx, U8* data, U8 size)
{
    U8 sendStatus = RETURN_OK;
    U8 index = 0;

#if UART_IS_ENABLED
    for(index=0; (index<size) && (RETURN_OK==sendStatus); index++)
    {
        sendStatus = Uart_SendOneByte(UARTx, data[index]);
    }
#endif /* UART_IS_ENABLED */

    return sendStatus;
}

/*--------------------------------------------------------------------------------
@name       Uart_Send_String
@brief      Send multiple bytes
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U8 Uart_Send_String(USART_TypeDef* UARTx, char* data, U8 size)
{
    U8 sendStatus = RETURN_OK;
    U8 index = 0;

#if UART_IS_ENABLED
    for(index=0; (index<size) && (RETURN_OK==sendStatus); index++)
    {
        sendStatus = Uart_SendOneByte(UARTx, data[index]);
    }
#endif /* UART_IS_ENABLED */
    
    return sendStatus;
}

/*--------------------------------------------------------------------------------
@name       Uart_Send_Integer
@brief      Send positivs and negativs numbers
            '+' sign is never sent
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U8 Uart_Send_SInteger(USART_TypeDef* UARTx, S32 number)
{
    U8 sendStatus = RETURN_NOK;
    
#if UART_IS_ENABLED
    if(number < 0L)
    {
        sendStatus = Uart_Send_Byte(UARTx, '-');
        number *= (-1);
    }
    sendStatus = Uart_Send_UInteger(UARTx, (U32)number);
#endif /* UART_IS_ENABLED */
    
    return sendStatus;
}

/*--------------------------------------------------------------------------------
@name       Uart_Send_Integer
@brief      Send positivs and negativs numbers
            '+' sign is never sent
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U8 Uart_Send_UInteger(USART_TypeDef* UARTx, U32 number)
{
    U8 sendStatus = RETURN_NOK;
    U8 strLen = 0;
    ArraySize11 strBuf;
    
#if UART_IS_ENABLED
    strLen = IntToString(number, &strBuf);
    sendStatus = Uart_Send_String(UARTx, (char*)strBuf, strLen);
#endif /* UART_IS_ENABLED */
    
    return sendStatus;
}

/*--------------------------------------------------------------------------------
@name       Uart_RB_Read
@brief      TBD
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U16 Uart_RB_Read(USART_TypeDef* UARTx, char* buffer, U16 bufSize)
{
    U16 dataRead = 0;
    RingBuffer_Type* tmpRB = nullptr;
    
#if UART_IS_ENABLED
    if(USART1 == UARTx)
    {
#if (ENABLE_DEVICE_UART1 == ON)
        tmpRB = &RB_U1;
#endif /* ENABLE_DEVICE_UART1 == ON */
    }
    else if(USART2 == UARTx)
    {
#if (ENABLE_DEVICE_UART2 == ON)
        tmpRB = &RB_U2;
#endif /* ENABLE_DEVICE_UART2 == ON */
    }
    else if(USART3 == UARTx)
    {
#if (ENABLE_DEVICE_UART3 == ON)
        tmpRB = &RB_U3;
#endif /* ENABLE_DEVICE_UART3 == ON */
    }

    if(tmpRB != nullptr)
    {
        RB_Read(tmpRB, buffer, bufSize, dataRead);
    }
#endif /* UART_IS_ENABLED */
    
    return dataRead;
}

/*--------------------------------------------------------------------------------
@name       USART1_IRQHandler
@brief      Callback function related to UART1 intterupts
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
#if (ENABLE_DEVICE_UART1 == ON)
void USART1_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint16_t newByte = USART_ReceiveData(USART1);
        RB_Write(&RB_U1, (char)newByte);
    }
}
#endif /* ENABLE_DEVICE_UART1 == ON */

/*--------------------------------------------------------------------------------
@name       USART2_IRQHandler
@brief      Callback function related to UART2 intterupts
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
#if (ENABLE_DEVICE_UART2 == ON)
void USART2_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        uint16_t newByte = USART_ReceiveData(USART2);
        RB_Write(&RB_U2, (char)newByte);
    }
}
#endif /* ENABLE_DEVICE_UART2 == ON */
    
/*--------------------------------------------------------------------------------
@name       USART3_IRQHandler
@brief      Callback function related to UART3 intterupts
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
#if (ENABLE_DEVICE_UART3 == ON)
void USART3_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        uint16_t newByte = USART_ReceiveData(USART3);
        RB_Write(&RB_U3, (char)newByte);
    }
}
#endif /* ENABLE_DEVICE_UART3 == ON */

