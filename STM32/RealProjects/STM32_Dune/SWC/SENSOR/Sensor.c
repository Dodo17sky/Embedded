/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Sensor.h"

#if (SWC_SENSOR_ENABLE == ON)

#include "Port.h"

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/* List of configured sensors */
static const struct Sensor_ConfigType SensorList[] =
{
    /*     PIN NUMBER       |    PIN_PORT       |    SENSOR TYPE                */
    /*----------------------+-------------------+-------------------------------*/
    /*                      |                   |                               */
    {   PIN_NUMBER_12       , PIN_PORT_B        , SENSOR_TYPE_DIGITAL           }
};

/* Number of configured sensors */
static const U8 SENSOR_NUMBER = sizeof(SensorList)/sizeof(SensorList[0]);

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/

/* Sensors status */
static struct Sensor_StatusType SensorStatus[SENSOR_NUMBER] = {0}; 

/*--------------------------------------------------------------------------------
*                           Functions prototypes declaration
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
@name		Sensor_InitRunnable
@brief		Initialize Sensor application
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void Sensor_InitRunnable(void)
{
    U8 index = 0;
    
    for(index=0; index<SENSOR_NUMBER; index++)
    {
        SensorStatus[index].updated = FALSE;
        SensorStatus[index].value = 0;
    }
}

/*--------------------------------------------------------------------------------
@name		Sensor_MainRunnable
@brief		This is the cycle function of Sensor software component
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void Sensor_MainRunnable(void)
{
    U8 index = 0;
    U8 newSensorValue = 0;
    
    for(index=0; index<SENSOR_NUMBER; index++)
    {
        if(SENSOR_TYPE_DIGITAL ==  SensorList[index].type)
        {
            newSensorValue = Port_ReadPin(SensorList[index].port, SensorList[index].pin);
            if(SensorStatus[index].value != newSensorValue)
            {
                SensorStatus[index].value = newSensorValue;
                SensorStatus[index].updated = TRUE;
            }
        }
    }

}

#else /* (SWC_SENSOR_ENABLE == ON) */

void Sensor_InitRunnable(void) {}
void Sensor_MainRunnable(void) {}

#endif /* (SWC_SENSOR_ENABLE == ON) */