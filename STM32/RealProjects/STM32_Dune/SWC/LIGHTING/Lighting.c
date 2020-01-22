/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Lighting.h"
#include "Timer.h"
#include "stm32f10x_gpio.h"

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/
static TimerStruct_Type TimerLedC13;
/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/
#define LED_C13_BLINK_PERIOD    850

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/
void Second_Light(void);

/*--------------------------------------------------------------------------------
@name		Task_MainRunnable
@brief		This task run forever. This will launch all the task in this system
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void Lighting_MainRunnable(void)
{
    static U8 isOn = FALSE;
    
    if(TIMER_ELAPSED == Timer_CountDown_IsDone(&TimerLedC13))
    {
        if(FALSE == isOn)
        {
            GPIO_ResetBits(GPIOC, GPIO_Pin_13);
            isOn = TRUE;
        }
        else
        {
            GPIO_SetBits(GPIOC, GPIO_Pin_13);
            isOn = FALSE;
        }
        Timer_CountDown_Start(&TimerLedC13, LED_C13_BLINK_PERIOD);
    }
}

/*--------------------------------------------------------------------------------
@name		Lighting_InitRunnable
@brief		Initialize Lighting application
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void Lighting_InitRunnable(void)
{
    /* Start LED C13 toggle timer */
    Timer_CountDown_Start(&TimerLedC13, LED_C13_BLINK_PERIOD);
}

/*--------------------------------------------------------------------------------
@brief		This controls second LED - B12
*--------------------------------------------------------------------------------*/ 
void Second_Light(void)
{
    static U8 isOn = FALSE;
    if(isOn)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        isOn = FALSE;
    }
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
        isOn = TRUE;
    }
}