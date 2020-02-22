/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Port.h"
#include "stm32f10x.h"

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/
static const PinConfig_Type PinList[] = 
{
    {   PIN_PORT_A,     PIN_NUMBER_0 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_A,     PIN_NUMBER_1 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_A,     PIN_NUMBER_2 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* UART2 TX              */
    {   PIN_PORT_A,     PIN_NUMBER_3 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* UART2 RX              */
    {   PIN_PORT_A,     PIN_NUMBER_4 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* SPI MASTER SS         */
    {   PIN_PORT_A,     PIN_NUMBER_5 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* SPI MASTER CLK        */
    {   PIN_PORT_A,     PIN_NUMBER_6 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* SPI MASTER MISO       */
    {   PIN_PORT_A,     PIN_NUMBER_7 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* SPI MASTER MOSI       */
    {   PIN_PORT_A,     PIN_NUMBER_8 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_A,     PIN_NUMBER_9 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* UART1 TX              */
    {   PIN_PORT_A,     PIN_NUMBER_10,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* UART1 RX              */
    {   PIN_PORT_A,     PIN_NUMBER_11,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_A,     PIN_NUMBER_12,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_A,     PIN_NUMBER_13,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_A,     PIN_NUMBER_14,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_A,     PIN_NUMBER_15,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */

    {   PIN_PORT_B,     PIN_NUMBER_0 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_B,     PIN_NUMBER_1 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_B,     PIN_NUMBER_2 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_B,     PIN_NUMBER_3 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_B,     PIN_NUMBER_4 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_B,     PIN_NUMBER_5 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_B,     PIN_NUMBER_6 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_B,     PIN_NUMBER_7 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_B,     PIN_NUMBER_8 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_B,     PIN_NUMBER_9 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_B,     PIN_NUMBER_10,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* UART3 TX              */
    {   PIN_PORT_B,     PIN_NUMBER_11,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* UART3 RX              */
    {   PIN_PORT_B,     PIN_NUMBER_12,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* SPI SLAVE SS          */
    {   PIN_PORT_B,     PIN_NUMBER_13,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* SPI SLAVE CLK         */
    {   PIN_PORT_B,     PIN_NUMBER_14,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* SPI SLAVE MISO        */
    {   PIN_PORT_B,     PIN_NUMBER_15,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* SPI SLAVE MOSI        */

    {   PIN_PORT_C,     PIN_NUMBER_13,  PIN_MODE_OUT_PP     ,   GPIO_Speed_50MHz    }, /* user LED */
    {   PIN_PORT_C,     PIN_NUMBER_14,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_C,     PIN_NUMBER_15,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
                                                                                                   
    {   PIN_PORT_D,     PIN_NUMBER_0 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }, /* usage */
    {   PIN_PORT_D,     PIN_NUMBER_1 ,  PIN_MODE_NOT_USED   ,   GPIO_Speed_50MHz    }  /* usage */
};

const U8 PINS_NUMBER = sizeof(PinList)/sizeof(PinList[0]);

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Functions prototypes declaration
*--------------------------------------------------------------------------------*/
GPIOMode_TypeDef getPinMode(U8 modeIndex);

/*--------------------------------------------------------------------------------
*                           Functions prototypes definition
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
@name		getPinMode
@brief		Get pin initialization mode based on an index
@paramIn	speed index
@paramOut	speed type
*--------------------------------------------------------------------------------*/
GPIOMode_TypeDef getPinMode(U8 modeIndex)
{
    GPIOMode_TypeDef modeType = 0xFF;
    
    switch(modeIndex)
    {
        case PIN_MODE_AIN        :
            modeType = GPIO_Mode_AIN;
            break;
        case PIN_MODE_IN_FLOATING:
            modeType = GPIO_Mode_IN_FLOATING;
            break;
        case PIN_MODE_IPD        :
            modeType = GPIO_Mode_IPD;
            break;
        case PIN_MODE_IPU        :
            modeType = GPIO_Mode_IPU;
            break;
        case PIN_MODE_OUT_OD     :
            modeType = GPIO_Mode_Out_OD;
            break;
        case PIN_MODE_OUT_PP     :
            modeType = GPIO_Mode_Out_PP;
            break;
        case PIN_MODE_AF_OD      :
            modeType = GPIO_Mode_AF_OD;
            break;
        case PIN_MODE_AF_PP      :
            modeType = GPIO_Mode_AF_PP;
            break;
        default:
            /* should not be reached */
            break;
    }
    return modeType;
}

/*--------------------------------------------------------------------------------
@name		Port_Init
@brief		Initialize general porpouse in out ports
@paramIn	void
@paramOut	OK  if success
        NOK if failed
*--------------------------------------------------------------------------------*/
U8 Port_Init(void)
{
    U8 retCode = RETURN_OK;
    U8 index = 0;
    U8 portIsUsed[4] = {0};
    U8 afioPinCounter = 0;
    U32 gpioPortClock = 0;
    GPIO_InitTypeDef pinConfig;
    GPIO_TypeDef* gpioPort = GPIOA;
    
    /* enable pins peripheral clocks */
    for(index=0; index<PINS_NUMBER; index++)
    {
        if(PinList[index].pinMode != PIN_MODE_NOT_USED)
        {
            portIsUsed[PinList[index].pinPort]++;
            
            if( PinList[index].pinMode == PIN_MODE_AIN || 
                PinList[index].pinMode == PIN_MODE_AF_OD ||
                PinList[index].pinMode == PIN_MODE_AF_PP)
            {
                afioPinCounter++;
            }
                
        }
    }
    
    for(index=0; index<4; index++)
    {
        if(portIsUsed[index] > 0)
        {
            /* RCC_APB2Periph_GPIOA    ((uint32_t)0x00000004) */
            /* RCC_APB2Periph_GPIOB    ((uint32_t)0x00000008) */
            /* RCC_APB2Periph_GPIOC    ((uint32_t)0x00000010) */
            /* RCC_APB2Periph_GPIOD    ((uint32_t)0x00000020) */
            
            gpioPortClock = (1 << (index + 2));
            RCC_APB2PeriphClockCmd(gpioPortClock, ENABLE);
        }
    }
    
    /* enable AFIO clock if needed */
    if(afioPinCounter > 0)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    }
    
    /* initialize each pin */
    for(index=0; index<PINS_NUMBER; index++)
    {
        if(PinList[index].pinMode != PIN_MODE_NOT_USED)
        {   
            pinConfig.GPIO_Mode = (GPIOMode_TypeDef)getPinMode((U8)PinList[index].pinMode);
            pinConfig.GPIO_Speed = (GPIOSpeed_TypeDef)PinList[index].pinSpeed;
            pinConfig.GPIO_Pin = (U16)(1 << PinList[index].pinNumber);
            
            gpioPort = (GPIO_TypeDef*)((uint32_t)GPIOA + (PinList[index].pinPort * 0x400));
            GPIO_Init(gpioPort, &pinConfig);
        }
    }

    return retCode;
}

