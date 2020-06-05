#ifndef RTE_COMMANAGER_HEADER
#define RTE_COMMANAGER_HEADER

/*--------------------------------------------------------------------------------
*                           SENSOR interfaces
*--------------------------------------------------------------------------------*/
#include "Sensor.h"

#if (SWC_SENSOR_ENABLE == ON)

/* External functions that we want to call */
extern void Sensor_ReadSensor(U8 sensorIndex, struct Sensor_StatusType *result);

/* Interfaces to external functions */
#define Rte_ComManager_ReadSensor(sensorId, sensorStatus) Sensor_ReadSensor(sensorId, sensorStatus)

#else

/* Blank interfaces if the target component is disabled */
#define Rte_ComManager_ReadSensor(sensorId, sensorStatus) 

#endif /* (SWC_SENSOR_ENABLE == ON) */

/*--------------------------------------------------------------------------------
*                           X interfaces
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Y interfaces
*--------------------------------------------------------------------------------*/

#endif /* RTE_COMMANAGER_HEADER */