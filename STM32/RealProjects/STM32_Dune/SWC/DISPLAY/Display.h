#ifndef SWC_DISPLAY_HEADER
#define SWC_DISPLAY_HEADER

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

/* This is the main activation switch of "Display" software component */
#define SWC_DISPLAY_ENABLE  OFF

/* "Display" task period [ms] */
#define SWC_DISPLAY_PERIOD  40U

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/
void Display_InitRunnable(void);
void Display_MainRunnable(void);

#endif /* SWC_DISPLAY_HEADER */

