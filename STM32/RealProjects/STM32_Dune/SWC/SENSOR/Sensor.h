#ifndef SWC_SENSOR_HEADER
#define SWC_SENSOR_HEADER

/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "GeneralTypes.h" 

/*--------------------------------------------------------------------------------
*                           Custom data types
*--------------------------------------------------------------------------------*/
struct Sensor_ConfigType
{
    U8 pin:4;               /* Variants from Port.h: PIN_NUMBER_0/../15         */
    U8 port:2;              /* Variants from Port.h: PIN_PORT_A/B/C/D           */
    U8 type:2;              /* Variants: SENSOR_TYPE_DIGITAL, SENSOR_TYPE_PWM   */
};

struct Sensor_StatusType
{
    U8 value:7;             /* Sensor value                                     */
    U8 updated:1;           /* Variants : TRUE, FALSE                           */
};


/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/* Sensor pin type */
#define SENSOR_TYPE_DIGITAL         0U
#define SENSOR_TYPE_PWM             1U

/* This is the main activation switch of "Sensor" software component */
#define SWC_SENSOR_ENABLE           ON

/* "Sensor" task period [ms] */
#define SWC_SENSOR_PERIOD           1U

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/
void Sensor_InitRunnable(void);
void Sensor_MainRunnable(void);

 #endif /* SWC_SENSOR_HEADER */