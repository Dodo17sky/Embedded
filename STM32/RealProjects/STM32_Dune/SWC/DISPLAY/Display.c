/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Display.h"

#if (SWC_DISPLAY_ENABLE == ON)

#include "ST7735.h"
#include "Timer.h"
#include "Adc.h"

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/
static U16 Timer;
static U32 Seconds;

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
@name		Display_MainRunnable
@brief		Display main function which runs cycle
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void Display_MainRunnable(void)
{
    if( Timer == (1000/SWC_DISPLAY_PERIOD) )
    {
        Timer = 0;
        Seconds++;
        U16 adcValue = Adc_Read(ADC_Channel_0);
        
        ST7735_FillRect(0, 29, 127, 90, COLOR565_RED);
        ST7735_PutInt5x7(3, 5, 30, adcValue, COLOR565_YELLOW, COLOR565_RED);
        ST7735_PutInt5x7(3, 5, 66, Seconds , COLOR565_BLUE  , COLOR565_RED);
    }
    Timer++;
}

/*--------------------------------------------------------------------------------
@name		Display_InitRunnable
@brief		Initialize Display application
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void Display_InitRunnable(void)
{
    ST7735_Init();
    ST7735_AddrSet(0,0,159,127);
	ST7735_Orientation(scr_180);
    
    ST7735_Clear(COLOR565_RED);
}

#else /* (SWC_DISPLAY_ENABLE == ON) */

void Display_InitRunnable(void) {}
void Display_MainRunnable(void) {}

#endif /* (SWC_DISPLAY_ENABLE == ON) */