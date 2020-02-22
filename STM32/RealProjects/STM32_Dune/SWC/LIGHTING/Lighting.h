#ifndef SWC_LIGHTING_HEADER
#define SWC_LIGHTING_HEADER

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
#define SWC_LIGHTING_PERIOD  20U

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/
void Lighting_InitRunnable(void);
void Lighting_MainRunnable(void);

 #endif /* SWC_LIGHTING_HEADER */

