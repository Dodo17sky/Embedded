#ifndef UART_HEADER
#define UART_HEADER

/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "GeneralTypes.h" 
#include "stm32f10x.h"

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/
#define ENABLE_DEVICE_UART1         ON
#define ENABLE_DEVICE_UART2         OFF
#define ENABLE_DEVICE_UART3         ON
#define UART_IS_ENABLED             (0 < (ENABLE_DEVICE_UART1 + ENABLE_DEVICE_UART2 + ENABLE_DEVICE_UART3))

#define UART_RX_BUFFER_SIZE         200
#define UART_RX_END_CHAR            '\n'

#define RXBUF_STATUS_IDLE           0
#define RXBUF_STATUS_WDONE          1
#define RXBUF_STATUS_RDONE          2
#define RXBUF_STATUS_WRITING        3

/*--------------------------------------------------------------------------------
*                           Custom data types
*--------------------------------------------------------------------------------*/
typedef U8 UartRxBuffer_Type[UART_RX_BUFFER_SIZE];

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/
U8 Uart_Init(void);
U8 Uart_Deinit(void);
U8 Uart_Send_Byte(USART_TypeDef* UARTx, U8 data);
U8 Uart_Send_ByteArray(USART_TypeDef* UARTx, U8* data, U8 size);
U8 Uart_Send_String(USART_TypeDef* UARTx, char* data, U8 size);
U8 Uart_Send_SInteger(USART_TypeDef* UARTx, S32 number);
U8 Uart_Send_UInteger(USART_TypeDef* UARTx, U32 number);
U16 Uart_RB_Read(USART_TypeDef* UARTx, char* buffer, U16 bufSize);

#endif /* UART_HEADER */

