#ifndef SYSTEM_HEADER
#define SYSTEM_HEADER

/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "GeneralTypes.h" 

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/
U8 System_Init(void);
U8 System_Deinit(void);
void System_Scheduler(void);

#endif /* SYSTEM_HEADER */

