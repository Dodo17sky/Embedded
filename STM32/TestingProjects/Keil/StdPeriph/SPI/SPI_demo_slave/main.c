/*--------------------------------------------------------------------------------
*                           General info
* 
*  STM32F103C8 max bit rate is 18 Mbits/s
Pin configuration:
    SPI1:   NSS     -   PA4
            SCK     -   PA5
            MISO    -   PA6
            MOSI    -   PA7
            
    SPI2:   NSS     -   PB12
            SCK     -   PB13
            MISO    -   PB14
            MOSI    -   PB15
*--------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "delay.h"

#define SPI_TX_DELAY        100

#define SPIm_RCC			RCC_APB2Periph_SPI1
#define SPIm				SPI1
#define SPIm_GPIO_RCC		RCC_APB2Periph_GPIOA
#define SPIm_GPIO			GPIOA
#define SPIm_PIN_MOSI		GPIO_Pin_7
#define SPIm_PIN_MISO		GPIO_Pin_6
#define SPIm_PIN_SCK		GPIO_Pin_5
#define SPIm_PIN_SS			GPIO_Pin_4

#define SPIs_RCC			RCC_APB1Periph_SPI2
#define SPIs				SPI2
#define SPIs_GPIO_RCC		RCC_APB2Periph_GPIOB
#define SPIs_GPIO			GPIOB
#define SPIs_PIN_MOSI		GPIO_Pin_15
#define SPIs_PIN_MISO		GPIO_Pin_14
#define SPIs_PIN_SCK		GPIO_Pin_13
#define SPIs_PIN_SS			GPIO_Pin_12

void SPIx_Init(void);
uint8_t SPIm_Transfer(uint8_t data);
void SPIm_EnableSlave(void);
void SPIm_DisableSlave(void);

void LedInit();
void LedToggle();

volatile uint8_t newData = 1;
volatile uint8_t spiData = 7; // a random byte

int main(void)
{    
    LedInit();
    DelayInit();
    SPIx_Init();

    /* enable RXNEIE interrupt */
    SPI_I2S_ITConfig(SPIs, SPI_I2S_IT_RXNE, ENABLE);
    
    while (1)
    {
        SPIm_EnableSlave();
        if(1 == newData)
        {
            newData = 0;
            spiData += 3;
            
            /* Wait for SPIm Tx buffer empty */
            while (SPI_I2S_GetFlagStatus(SPIm, SPI_I2S_FLAG_TXE) == RESET);
            /* Send SPIz data */
            SPI_I2S_SendData(SPIm, spiData);
            
            DelayUs(SPI_TX_DELAY);
        }
        SPIm_DisableSlave();
        
        LedToggle();
    }
}

void LedInit()
{
    GPIO_InitTypeDef gpioConfig;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    gpioConfig.GPIO_Pin = GPIO_Pin_13;
    gpioConfig.GPIO_Mode = GPIO_Mode_Out_OD;
    gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpioConfig);
}

void LedToggle()
{
    uint64_t ts = 0;
    static uint64_t last = 0;
    static uint8_t ledState = 0;
    
    ts = GetTimestamp();
    if( ts >= (last+500000) )
    {
        if(0 == ledState)
        {
            ledState = 1;
            GPIO_SetBits(GPIOC, GPIO_Pin_13);
        }
        else
        {
            ledState = 0;
            GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        }
        last = ts;
    }
}

void SPI_Init_Master()
{
    SPI_InitTypeDef spiConfig;
    GPIO_InitTypeDef gpioConfig;
    
    /* configure GPIO for SPI master */
    gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
    
    gpioConfig.GPIO_Pin = (SPIm_PIN_SCK | SPIm_PIN_MOSI);
    gpioConfig.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPIm_GPIO, &gpioConfig);
    
    gpioConfig.GPIO_Pin = SPIm_PIN_MISO;
    gpioConfig.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SPIm_GPIO, &gpioConfig);
    
    gpioConfig.GPIO_Pin = SPIm_PIN_SS;
    gpioConfig.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPIm_GPIO, &gpioConfig);
    
    /* configure master SPI */
    spiConfig.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    spiConfig.SPI_CPOL = SPI_CPOL_Low;
    spiConfig.SPI_CPHA = SPI_CPHA_2Edge;
    spiConfig.SPI_DataSize = SPI_DataSize_8b;
    spiConfig.SPI_FirstBit = SPI_FirstBit_MSB;
    spiConfig.SPI_NSS = SPI_NSS_Soft;
    spiConfig.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPIm, &spiConfig);
    
    SPI_Cmd(SPIm, ENABLE);
    SPIm_DisableSlave();
}

void SPI_NVIC_Init()
{
    NVIC_InitTypeDef NVIC_InitStructure;
   
    /* 1 bit for pre-emption priority, 3 bits for subpriority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Configure and enable SPI_SLAVE interrupt --------------------------------*/
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

void SPI_Init_Slave()
{
    SPI_InitTypeDef spiConfig;
    GPIO_InitTypeDef gpioConfig;
    
    /* configure GPIO for SPI master */
    gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
    
    gpioConfig.GPIO_Pin = (SPIs_PIN_SCK | SPIs_PIN_MOSI);
    gpioConfig.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SPIs_GPIO, &gpioConfig);
    
    gpioConfig.GPIO_Pin = SPIs_PIN_MISO;
    gpioConfig.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPIs_GPIO, &gpioConfig);
    
    /* configure master SPI */
    spiConfig.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    spiConfig.SPI_CPOL = SPI_CPOL_Low;
    spiConfig.SPI_CPHA = SPI_CPHA_2Edge;
    spiConfig.SPI_DataSize = SPI_DataSize_8b;
    spiConfig.SPI_FirstBit = SPI_FirstBit_MSB;
    spiConfig.SPI_NSS = SPI_NSS_Soft;
    spiConfig.SPI_Mode = SPI_Mode_Slave;
    SPI_Init(SPIs, &spiConfig);
    
    SPI_NVIC_Init();
        
    SPI_Cmd(SPIs, ENABLE);
}

void SPI_Init_RCC()
{
    RCC_APB2PeriphClockCmd(SPIm_GPIO_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(SPIs_GPIO_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(SPIm_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(SPIs_RCC, ENABLE);
}

void SPIx_Init()
{
	SPI_Init_RCC();
    SPI_Init_Master();
    SPI_Init_Slave();
}

void SPIm_EnableSlave()
{
	// Set slave SS pin low
	SPIm_GPIO->BRR = SPIm_PIN_SS;
}

void SPIm_DisableSlave()
{
	// Set slave SS pin high
	SPIm_GPIO->BSRR = SPIm_PIN_SS;
}

void SPI2_IRQHandler(void)
{
    spiData = SPI_I2S_ReceiveData(SPIs);
    newData = 1;
    /* Write in the DR register the data to be sent to master*/
    SPIs->DR = spiData;
}

