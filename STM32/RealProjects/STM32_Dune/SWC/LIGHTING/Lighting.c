/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Lighting.h"

#if (SWC_LIGHTING_ENABLE == ON)

#include "Rte_Lighting.h"
#include "Timer.h"
#include "Port.h"

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/
static TimerStruct_Type TimerLedC13;
/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/
#define LED_C13_BLINK_PERIOD    1000

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
@name		Lighting_MainRunnable
@brief		This is the cycle function of Lighting software component
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void Lighting_MainRunnable(void)
{
    U8 ledStatus = PIN_STATUS_LOW;
    struct Sensor_StatusType pushBtn;
    Rte_Lighting_ReadSensor(0, &pushBtn);
    
    /* Onboard LED is connected to 3.3V, so turning PC13 to high will turn off the led */
    if(pushBtn.value == PIN_STATUS_LOW)
        ledStatus = PIN_STATUS_HIGH;
    
    Port_WritePin(PIN_PORT_C, PIN_NUMBER_13, ledStatus);
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

#else /* (SWC_LIGHTING_ENABLE == ON) */

void Lighting_InitRunnable(void) {}
void Lighting_MainRunnable(void) {}

#endif /* (SWC_LIGHTING_ENABLE == ON) */