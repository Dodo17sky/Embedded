#ifndef SWC_COM_HEADER
#define SWC_COM_HEADER

/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "GeneralTypes.h" 

/*--------------------------------------------------------------------------------
*                           Custom data types
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/* This is the main activation switch of "Com" software component */
#define SWC_COM_ENABLE              OFF

/* "Com" task period [ms] */
#define SWC_COM_PERIOD              5U

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/
void Com_InitRunnable(void);
void Com_MainRunnable(void);

#endif /* SWC_COM_HEADER */

