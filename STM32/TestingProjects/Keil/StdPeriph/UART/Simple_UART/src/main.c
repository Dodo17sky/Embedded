#include "stm32f10x.h"
#include "stm32f10x_conf.h"

uint16_t rxbuf[64];
int rxbuf_pos = 0;
static uint16_t counter = 0;

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    
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
    
    /* Enable RXNE interrupt */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    /* Enable USART1 global interrupt */
    NVIC_EnableIRQ(USART1_IRQn);

    while(1) {
        // everything happens in interrupt
    }
    
    return 0;
}

void USART1_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        /* data received */
    	char c = (char)USART_ReceiveData(USART1);
        rxbuf[rxbuf_pos++] = c;
        counter ++;
        if (c == '\n'){
            USART_SendData(USART1, '0' + counter);           
            /* Wait until the byte has been transmitted */
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
            counter = 0;
        }
    }
}
