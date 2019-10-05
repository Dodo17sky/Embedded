#include "stm32f10x.h"
#include "stm32f10x_conf.h"

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
    uint16_t rxbuf[64];
    int rxbuf_pos = 0;
    int i;
    
    /* Enable peripheral clocks for USART1 on GPIOA */
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_USART1 |
        RCC_APB2Periph_GPIOA |
        RCC_APB2Periph_AFIO, ENABLE);
        
    /* Configure PA9 and PA10 as USART1 TX/RX */
    
    /* PA9 = alternate function push/pull output */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* PA10 = floating input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Configure and initialize usart... */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        
    USART_Init(USART1, &USART_InitStructure);
    
    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);   

    while (1)
    {
        /* Wait until there's data in the receive data register */
        while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
        
        /* Read a byte */
        rxbuf[rxbuf_pos++] = USART_ReceiveData(USART1);

        /* Check if the previous byte was a newline */
        if ((rxbuf[rxbuf_pos-1] == '\n' || rxbuf[rxbuf_pos-1] == '\r') && rxbuf_pos != 0) {
            
            /* Send the line back */
            for (i = 0; i < rxbuf_pos; i++) {
                USART_SendData(USART1, rxbuf[i]);
                
                /* Wait until the byte has been transmitted */
                while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
            }
            
            rxbuf_pos = 0;
        }
    }
    return 0;
}

