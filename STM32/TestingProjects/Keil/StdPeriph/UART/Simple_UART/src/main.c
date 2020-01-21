#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "uart_buffer.h"

static volatile uint32_t TimingDelay;
static volatile uint32_t SystemTime;

char buf[100];
uint8_t idx = 0;

#define SELECTED_UART   3

#if (SELECTED_UART == 1)
#define UART_PIN_RX     GPIO_Pin_10
#define UART_PIN_TX     GPIO_Pin_9
#define UART_PORT       GPIOA
#define UART_DEVICE     USART1
#define UART_ISR        USART1_IRQn
#define UART_ISR_NAME   USART1_IRQHandler
#endif

#if (SELECTED_UART == 2)
#define UART_PIN_RX     GPIO_Pin_3
#define UART_PIN_TX     GPIO_Pin_2
#define UART_PORT       GPIOA
#define UART_DEVICE     USART2
#define UART_ISR        USART2_IRQn
#define UART_ISR_NAME   USART2_IRQHandler
#endif

#if (SELECTED_UART == 3)
#define UART_PIN_RX     GPIO_Pin_11
#define UART_PIN_TX     GPIO_Pin_10
#define UART_PORT       GPIOB
#define UART_DEVICE     USART3
#define UART_ISR        USART3_IRQn
#define UART_ISR_NAME   USART3_IRQHandler
#endif

uint32_t Timer_GetCurrentTime(void);

int main(void)
{
    uint32_t lastTs, now;
    SysTick_Config(SystemCoreClock / 1000);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    
    /* Enable peripheral clocks for USART1 on GPIOA */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE);
    
    /* TX pin as alternate function push/pull output */
    GPIO_InitStructure.GPIO_Pin = UART_PIN_TX;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(UART_PORT, &GPIO_InitStructure);
    
    /* RX pin as floating input */
    GPIO_InitStructure.GPIO_Pin = UART_PIN_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART_PORT, &GPIO_InitStructure);
    
    /* Configure and initialize usart... */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        
    USART_Init(UART_DEVICE, &USART_InitStructure);
    
    /* Enable USART1 */
    USART_Cmd(UART_DEVICE, ENABLE);   
    
    /* Enable RXNE interrupt */
    USART_ITConfig(UART_DEVICE, USART_IT_RXNE, ENABLE);
    
    /* Enable USART1 global interrupt */
    NVIC_EnableIRQ(UART_ISR);

    while(1) {
        
        /************************************************************
         *  Send '-' char at each 1000 ms as a control data
         *  just to be sure that communication is working
         ************************************************************/
        now = Timer_GetCurrentTime();
        if(now >= (lastTs + 1000)){
            lastTs = now;
            USART_SendData(UART_DEVICE, '-');
            while (USART_GetFlagStatus(UART_DEVICE, USART_FLAG_TXE) == RESET);
        }
        
        /************************************************************
         *  Send back all data received like this:                  
         *    Rx:abc
         *    Tx:[abc]
         ************************************************************/
        if(uart_isNewSeparator()) {
            char data[256] = {0};
            char* ch = data;
            uart_getAll(data);
            
            USART_SendData(UART_DEVICE, '\n');
            while (USART_GetFlagStatus(UART_DEVICE, USART_FLAG_TXE) == RESET);
            
            USART_SendData(UART_DEVICE, '[');
            while (USART_GetFlagStatus(UART_DEVICE, USART_FLAG_TXE) == RESET);
            
            while(*(ch+1) != 0) {
                USART_SendData(UART_DEVICE, *ch);
                while (USART_GetFlagStatus(UART_DEVICE, USART_FLAG_TXE) == RESET);
                ch++;
            }
            
            USART_SendData(UART_DEVICE, ']');
            while (USART_GetFlagStatus(UART_DEVICE, USART_FLAG_TXE) == RESET);
            
            USART_SendData(UART_DEVICE, '\n');
            while (USART_GetFlagStatus(UART_DEVICE, USART_FLAG_TXE) == RESET);
        }
    }
    
    return 0;
}

void UART_ISR_NAME(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(UART_DEVICE, USART_IT_RXNE) != RESET)
    {
        uint16_t data = USART_ReceiveData(UART_DEVICE);
        uart_receive(data);
    }
}

uint32_t Timer_GetCurrentTime(void)
{
    return SystemTime;
}

void SysTick_Handler(void)
{
    if (TimingDelay != 0)
    {
        TimingDelay--;
    }
    SystemTime++;
}