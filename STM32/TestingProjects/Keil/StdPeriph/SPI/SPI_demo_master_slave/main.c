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
uint16_t SPI_Master_Transfer(uint16_t data);
void SPIm_EnableSlave(void);
void SPIm_DisableSlave(void);

void LedInit();
void LedToggle();

void SPI_MainRunnable();

volatile uint8_t newData = 1;
volatile uint8_t spiData = 7; // a random byte

int main(void)
{    
    LedInit();
    DelayInit();
    SPIx_Init();
    
    SPIm_EnableSlave();
    while (1)
    {        
        SPI_MainRunnable();
        
        LedToggle();
    }
    SPIm_DisableSlave();
}

void SPI_MainRunnable()
{
    uint64_t ts = 0;
    static uint64_t last = 0;
    static uint8_t masterTxData = 0;

    /* delay master transmission */
    ts = GetTimestamp();
    
    if( ts >= (last+500) )
    {
        last = ts;
        masterTxData++;
        
        
        SPI_Master_Transfer(masterTxData);
    }
}

void LedInit()
{
    GPIO_InitTypeDef gpioConfig;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    gpioConfig.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    gpioConfig.GPIO_Mode = GPIO_Mode_Out_PP;
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
    spiConfig.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spiConfig.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    spiConfig.SPI_CPOL = SPI_CPOL_Low;
    spiConfig.SPI_CPHA = SPI_CPHA_2Edge;
    spiConfig.SPI_DataSize = SPI_DataSize_8b;
    spiConfig.SPI_FirstBit = SPI_FirstBit_MSB;
    spiConfig.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
    spiConfig.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPIm, &spiConfig);
    
    SPI_Cmd(SPIm, ENABLE);
    SPIm_DisableSlave();
}

void SPI_NVIC_Init()
{
    /*  MASTER SPI device doesn't need RXNE or TXE interrupts,
     *  because MASTER SPI device controls communication and
     *  data transfer on master is done on-demand,
     *  so master knows when data is transmitted and received.
     *
     *  SLAVE SPI device doesn't control communication and 
     *  he doesn't know when data will be transferred.
     *  Thus, SLAVE SPI device needs the RXNE interrupt activated
     *  to be informed when new data was received.
    */
    
    NVIC_InitTypeDef NVIC_InitStructure;
   
    /* 1 bit for pre-emption priority, 3 bits for subpriority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Configure and enable SPI SLAVE interrupt --------------------------------*/
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    /* enable SLAVE Rx interrupt */
    SPI_I2S_ITConfig(SPIs, SPI_I2S_IT_RXNE, ENABLE);
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

uint16_t SPI_Master_Transfer(uint16_t data)
{
    /*  An SPI device always perform 2 operations in parallel
     *      - A transmission (data from master to slave)
     *      - A reception (data from slave to master)
     *  
     *  There is NO NEED FOR INTERRUPT on master for Rx indication or Tx confirmation !!
     *  How so ??
     *      - Because the transmission is done on demand (when is needed)
     *      - The reception is always done right after transmission (Tx and Rx are simultaneous operations)
     *      - So any Tx involves also an Rx
     */
    uint16_t dataFromSlave = 0;
        
    /* Wait for SPIm Tx buffer empty */
    while (SPI_I2S_GetFlagStatus(SPIm, SPI_I2S_FLAG_TXE) == RESET);
    /* Write data to TX buffer */
    SPI_I2S_SendData(SPIm, data);
    
    /* Starting from now, data is shifted out (from master) and shifted in (from slave) */
    
    /* When the transmission is done, the RXNE flag is set */
    while (SPI_I2S_GetFlagStatus(SPIm, SPI_I2S_FLAG_RXNE) == RESET);
    /* Transmision done. Get data received from slave */
    dataFromSlave = SPI_I2S_ReceiveData(SPIm);
    
    return dataFromSlave;
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
    
    /* Clear any pending RX status */
    SPI_I2S_ClearFlag(SPIm, SPI_I2S_FLAG_RXNE);
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
    if(GPIOC->ODR & GPIO_Pin_14)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_14);
    }
    else
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_14);
    }
    
    uint8_t data = SPI_I2S_ReceiveData(SPIs);
    /* Write in the DR register the data to be sent to master*/
    SPIs->DR = data;
}