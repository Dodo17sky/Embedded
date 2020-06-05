/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Sensor.h"

#if (SWC_SENSOR_ENABLE == ON)

/*--------------------------------------------------------------------------------
@name		Sensor_InitRunnable
@brief		Initialize Sensor application
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void Sensor_InitRunnable(void)
{
}

/*--------------------------------------------------------------------------------
@name		Sensor_MainRunnable
@brief		This is the cycle function of Sensor software component
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void Sensor_MainRunnable(void)
{
}

#else /* (SWC_SENSOR_ENABLE == ON) */

void Sensor_InitRunnable(void) {}
void Sensor_MainRunnable(void) {}

#endif /* (SWC_SENSOR_ENABLE == ON) */