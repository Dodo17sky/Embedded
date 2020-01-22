/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Uart.h"
#include "String.h"


/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/
#if (ENABLE_DEVICE_UART1 == ON)
    UartRx_Type RxBuf1 = {0};
#endif /* ENABLE_DEVICE_UART1 == ON */

#if (ENABLE_DEVICE_UART2 == ON)
    UartRx_Type RxBuf2 = {0};
#endif /* ENABLE_DEVICE_UART2 == ON */
    
#if (ENABLE_DEVICE_UART3 == ON)
    UartRx_Type RxBuf3 = {0};
#endif /* ENABLE_DEVICE_UART3 == ON */
    
UartPeripheralState_Type PeripheralState = { FALSE, FALSE, FALSE };
/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/
void Init_UART1(void);
static inline U8 Uart_SendOneByte(USART_TypeDef* UARTx, U8 byte);
static void Uart_AppendRxData(USART_TypeDef const* UARTx, U8 newByte);

/*--------------------------------------------------------------------------------
@name       Uart_Init
@brief      
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U8 Uart_Init(void)
{
    U8 retCode = RETURN_OK;
    
    USART_InitTypeDef USART_InitStructure;
    
    /*
     *    Enable UART peripheral clock
     */
#if (ENABLE_DEVICE_UART1 == ON)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    PeripheralState.isEnabled_UART1 = TRUE;
#endif /* ENABLE_DEVICE_UART1 == ON */

#if (ENABLE_DEVICE_UART2 == ON)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    PeripheralState.isEnabled_UART2 = TRUE;
#endif /* ENABLE_DEVICE_UART2 == ON */
    
#if (ENABLE_DEVICE_UART3 == ON)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    PeripheralState.isEnabled_UART3 = TRUE;
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
@name       Uart_Deinit
@brief      
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U8 Uart_Deinit(void)
{
    U8 retCode = RETURN_OK;
    
    PeripheralState.isEnabled_UART1 = FALSE;
    PeripheralState.isEnabled_UART2 = FALSE;
    PeripheralState.isEnabled_UART3 = FALSE;
    
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
    
    sendStatus = Uart_SendOneByte(UARTx, data);
    
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
    
    for(index=0; (index<size) && (RETURN_OK==sendStatus); index++)
    {
        sendStatus = Uart_SendOneByte(UARTx, data[index]);
    }
    
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
    
    for(index=0; (index<size) && (RETURN_OK==sendStatus); index++)
    {
        sendStatus = Uart_SendOneByte(UARTx, data[index]);
    }
    
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
    
    if(number < 0L)
    {
        sendStatus = Uart_Send_Byte(UARTx, '-');
        number *= (-1);
    }
    sendStatus = Uart_Send_UInteger(UARTx, (U32)number);
    
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
    
    strLen = IntToString(number, &strBuf);
    sendStatus = Uart_Send_String(UARTx, (char*)strBuf, strLen);
    
    return sendStatus;
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
        Uart_AppendRxData(USART1, newByte);
    }
}
#endif /* ENABLE_DEVICE_UART1 == ON */
#if (ENABLE_DEVICE_UART2 == ON)
void USART2_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        uint16_t newByte = USART_ReceiveData(USART2);
        Uart_AppendRxData(USART2, newByte);
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
        Uart_AppendRxData(USART3, newByte);
    }
}
#endif /* ENABLE_DEVICE_UART3 == ON */

/*--------------------------------------------------------------------------------
@brief      Add a new byte to RX internal buffer
*--------------------------------------------------------------------------------*/
static void Uart_AppendRxData(USART_TypeDef const* UARTx, U8 newByte)
{
    UartRx_Type* RxBuf = 0;
    
#if (ENABLE_DEVICE_UART1 == ON)
    if(UARTx == USART1)
    {
        RxBuf = &RxBuf1;
    }
#endif /* ENABLE_DEVICE_UART1 == ON */

#if (ENABLE_DEVICE_UART2 == ON)
    if(UARTx == USART2)
    {
        RxBuf = &RxBuf2;
    }
#endif /* ENABLE_DEVICE_UART2 == ON */
    
#if (ENABLE_DEVICE_UART3 == ON)
    if(UARTx == USART3)
    {
        RxBuf = &RxBuf3;
    }
#endif /* ENABLE_DEVICE_UART3 == ON */
    
    if(RxBuf->Status == RXBUF_STATUS_WDONE)
    {
        /* Data Lost, because was not read since last update */
        RxBuf->error.DataLostCounter++;
    }
    
    if(RxBuf->DataLength == (UART_RX_BUFFER_SIZE-1))
    {
        /* Buffer is full, so data will be lost */
        RxBuf->error.RxBufOverflow++;
        RxBuf->Buffer[RxBuf->DataLength] = 0;
        RxBuf->Status = RXBUF_STATUS_WDONE;
    }
    else if(UART_RX_END_CHAR == newByte)
    {
        /* end of transmision */
        RxBuf->Status = RXBUF_STATUS_WDONE;
    }
    else
    {
        /* new byte received */
        RxBuf->Buffer[RxBuf->DataLength++] = newByte;
    }
}

/*--------------------------------------------------------------------------------
@brief      Read all data from RX buffer
*--------------------------------------------------------------------------------*/
U8 Uart_Read_All(USART_TypeDef* UARTx, UartRxBuffer_Type* outBuffer)
{
    U8 retCode = RETURN_NOK;
    U8 index = 0;
    UartRx_Type* RxBuf = 0;
    
#if (ENABLE_DEVICE_UART1 == ON)
    if(UARTx == USART1)
    {
        RxBuf = &RxBuf1;
    }
#endif /* ENABLE_DEVICE_UART1 == ON */

#if (ENABLE_DEVICE_UART2 == ON)
    if(UARTx == USART2)
    {
        RxBuf = &RxBuf2;
    }
#endif /* ENABLE_DEVICE_UART2 == ON */
    
#if (ENABLE_DEVICE_UART3 == ON)
    if(UARTx == USART3)
    {
        RxBuf = &RxBuf3;
    }
#endif /* ENABLE_DEVICE_UART3 == ON */
    
    if(RxBuf->Status == RXBUF_STATUS_WDONE)
    {
        for(index=0; index<RxBuf->DataLength; index++)
        {
            (*outBuffer)[index] = RxBuf->Buffer[index];
        }
        RxBuf->DataLength = 0;
        RxBuf->Status = RXBUF_STATUS_RDONE;
        retCode = RETURN_OK;
    }
    
    return retCode;
}