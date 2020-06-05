#ifndef RTE_LIGHTING_HEADER
#define RTE_LIGHTING_HEADER

/*--------------------------------------------------------------------------------
*                           SENSOR interfaces
*--------------------------------------------------------------------------------*/
#include "Sensor.h"

#if (SWC_SENSOR_ENABLE == ON)

extern void Sensor_ReadSensor(U8 sensorIndex, struct Sensor_StatusType *result);

#define Rte_Lighting_ReadSensor(sensorId, sensorStatus) Sensor_ReadSensor(sensorId, sensorStatus)

#else

#define Rte_Lighting_ReadSensor(sensorId, sensorStatus) 

#endif /* (SWC_SENSOR_ENABLE == ON) */

/*--------------------------------------------------------------------------------
*                           X interfaces
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Y interfaces
*--------------------------------------------------------------------------------*/

#endif /* RTE_LIGHTING_HEADER */