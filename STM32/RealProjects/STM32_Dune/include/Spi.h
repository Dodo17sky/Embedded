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

#ifndef SPI_HEADER
#define SPI_HEADER

/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "GeneralTypes.h" 
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/* Master device */
#define SPI_MASTER_RCC			RCC_APB2Periph_SPI1
#define SPI_MASTER		        SPI1
#define SPI_MASTER_GPIO_RCC		RCC_APB2Periph_GPIOA
#define SPI_MASTER_GPIO			GPIOA
#define SPI_MASTER_PIN_MOSI		GPIO_Pin_7
#define SPI_MASTER_PIN_MISO		GPIO_Pin_6
#define SPI_MASTER_PIN_SCK		GPIO_Pin_5
#define SPI_MASTER_PIN_SS		GPIO_Pin_4
/* Slave device */
#define SPI_SLAVE_RCC			RCC_APB1Periph_SPI2
#define SPI_SLAVE				SPI2
#define SPI_SLAVE_GPIO_RCC		RCC_APB2Periph_GPIOB
#define SPI_SLAVE_GPIO			GPIOB
#define SPI_SLAVE_PIN_MOSI		GPIO_Pin_15
#define SPI_SLAVE_PIN_MISO		GPIO_Pin_14
#define SPI_SLAVE_PIN_SCK		GPIO_Pin_13
#define SPI_SLAVE_PIN_SS		GPIO_Pin_12

/* define a EOT/EOF character */
#define SPI_INVALID_DATA (0xFF)

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Custom data types
*--------------------------------------------------------------------------------*/
typedef U16 SpiData;

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/
U8 Spi_Init(void);
U8 Spi_Deinit(void);
void Spi_Enable_Slave(void);
void Spi_Disable_Slave(void);
U16 Spi_Master_Transfer(U16 data);
U8 Spi_ReadData(SpiData* data);
void Spi_GetIsrCounters(U32* MasterRxne, U32* SlaveRxne);

#endif /* SPI_HEADER */

