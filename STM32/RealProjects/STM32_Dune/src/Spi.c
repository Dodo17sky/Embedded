/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Spi.h"

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/
volatile static U32 MasterRxneCounter;
volatile static U32 SlaveRxneCounter;
volatile static U16 LastSlaveRxData;

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Functions prototypes declaration
*--------------------------------------------------------------------------------*/
static U8 SPI_Init_RCC(void);
static U8 SPI_Init_Master(void);
static U8 SPI_Init_Slave(void);
static U8 SPI_Init_NVIC(void);
static void SPI_ISR_Cmd(U8 status);

/*--------------------------------------------------------------------------------
*                           Functions prototypes definition
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
@name       SPI_Init_RCC
@brief      Enable all the peripheral clocks need by SPI devices
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
static U8 SPI_Init_RCC()
{
    U8 retCode = RETURN_OK;
    
    RCC_APB2PeriphClockCmd(SPI_MASTER_GPIO_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(SPI_SLAVE_GPIO_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(SPI_MASTER_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(SPI_SLAVE_RCC, ENABLE);

    return retCode;
}

/*--------------------------------------------------------------------------------
@name       SPI_Init_Master
@brief      Set up the master SPI peripheral
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
static U8 SPI_Init_Master()
{
    U8 retCode = RETURN_OK;
    
    SPI_InitTypeDef spiConfig;
    
    /* configure master SPI */
    spiConfig.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spiConfig.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    spiConfig.SPI_CPOL = SPI_CPOL_Low;
    spiConfig.SPI_CPHA = SPI_CPHA_2Edge;
    spiConfig.SPI_DataSize = SPI_DataSize_8b;
    spiConfig.SPI_FirstBit = SPI_FirstBit_MSB;
    spiConfig.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
    spiConfig.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPI_MASTER, &spiConfig);
    
    /* Clear any pending RX status */
    SPI_I2S_ClearFlag(SPI_MASTER, SPI_I2S_FLAG_RXNE);
    
    Spi_Disable_Slave();

    return retCode;
}

static U8 SPI_Init_Slave()
{
    U8 retCode = RETURN_OK;
    
    SPI_InitTypeDef spiConfig;
    
    /* configure master SPI */
    spiConfig.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    spiConfig.SPI_CPOL = SPI_CPOL_Low;
    spiConfig.SPI_CPHA = SPI_CPHA_2Edge;
    spiConfig.SPI_DataSize = SPI_DataSize_8b;
    spiConfig.SPI_FirstBit = SPI_FirstBit_MSB;
    spiConfig.SPI_NSS = SPI_NSS_Soft;
    spiConfig.SPI_Mode = SPI_Mode_Slave;
    SPI_Init(SPI_SLAVE, &spiConfig);

    return retCode;
}

/*--------------------------------------------------------------------------------
@name       SPI_Init_NVIC
@brief      Set up the interrupts for slave and master SPI devices
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
static U8 SPI_Init_NVIC(void)
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
    U8 retCode = RETURN_OK;
    
    NVIC_InitTypeDef NVIC_InitStructure;
   
    /* 1 bit for pre-emption priority, 3 bits for subpriority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Configure and enable SPI SLAVE interrupt --------------------------------*/
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
        
    return retCode;
}

/*--------------------------------------------------------------------------------
@name       Spi_Init
@brief      
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U8 Spi_Init(void)
{
    U8 retCode = RETURN_OK;
    
    SPI_Init_RCC();
    SPI_Init_Master();
    SPI_Init_Slave();
    SPI_Init_NVIC();
    
    SPI_ISR_Cmd(ENABLE);
    
    SPI_Cmd(SPI_SLAVE, ENABLE);
    SPI_Cmd(SPI_MASTER, ENABLE);
    
    Spi_Enable_Slave();

    return retCode;
}

/*--------------------------------------------------------------------------------
@name       Spi_Deinit
@brief      
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U8 Spi_Deinit(void)
{
    U8 retCode = RETURN_OK;
    
    Spi_Disable_Slave();

    return retCode;
}

/*--------------------------------------------------------------------------------
@name       Spi_Transfer
@brief      
@paramIn    Data to be transmitted
@paramOut   Data received
*--------------------------------------------------------------------------------*/
U16 Spi_Master_Transfer(U16 data)
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
    uint16_t dataFromSlave = SPI_INVALID_DATA;
        
    /* Wait for SPIm Tx buffer empty */
    while (SPI_I2S_GetFlagStatus(SPI_MASTER, SPI_I2S_FLAG_TXE) == RESET);
    /* Write data to TX buffer */
    SPI_I2S_SendData(SPI_MASTER, data);
    
    /* Starting from now, data is shifted out (from master) and shifted in (from slave) */
    
    /* When the transmission is done, the RXNE flag is set */
    while (SPI_I2S_GetFlagStatus(SPI_MASTER, SPI_I2S_FLAG_RXNE) == RESET);
    /* Transmision done. Get data received from slave */
    dataFromSlave = SPI_I2S_ReceiveData(SPI_MASTER);
    
    return dataFromSlave;
}

/*--------------------------------------------------------------------------------
@name       Enable_Slave
@brief      Start communication with slave SPI device
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
void Spi_Enable_Slave(void)
{
    // Set slave SS pin low
	SPI_MASTER_GPIO->BRR = SPI_MASTER_PIN_SS;
}

/*--------------------------------------------------------------------------------
@name       Disable_Slave
@brief      Stop communication with slave SPI device
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
void Spi_Disable_Slave(void)
{
    // Set slave SS pin high
	SPI_MASTER_GPIO->BSRR = SPI_MASTER_PIN_SS;
}

/*--------------------------------------------------------------------------------
@name       Spi_ReadData
@brief      Get last data received by SPI
@paramIn    Pointer to output variable
@paramOut   Read status
*--------------------------------------------------------------------------------*/
U8 Spi_ReadData(SpiData* data)
{
    U8 retCode = RETURN_NOK;
    
    return retCode;
}

static void SPI_ISR_Cmd(U8 status)
{
    if(ENABLE != status && DISABLE != status)
        return;
    
    /* enable slave Rx interrupt */
    SPI_I2S_ITConfig(SPI_SLAVE, SPI_I2S_IT_RXNE, status);
}

void Spi_GetIsrCounters(U32* MasterRxne, U32* SlaveRxne)
{
    *MasterRxne = MasterRxneCounter;
    *SlaveRxne = SlaveRxneCounter;
}

void SPI2_IRQHandler(void)
{
    /* THIS IS SLAVE INTERRUPT */
    
    U16 slaveTxData;
    
    /* You have to read SPI DR otherwise interrupt will be continuously triggered */
    LastSlaveRxData = SPI_I2S_ReceiveData(SPI_SLAVE);
    
    /* Respond to master */
    slaveTxData = LastSlaveRxData + 10;
    SPI_I2S_SendData(SPI_SLAVE, slaveTxData);
}

