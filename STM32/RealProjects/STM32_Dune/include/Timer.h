#ifndef TIMER_HEADER
#define TIMER_HEADER

/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "GeneralTypes.h" 

/*--------------------------------------------------------------------------------
*                           Custom types
*--------------------------------------------------------------------------------*/
typedef struct
{
    U32 lastTimestamp;
    U32 period;
} TimerStruct_Type;

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/
 
 
/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/
#define TIMER_ELAPSED       1
#define TIMER_NOT_ELAPSED   0
 
/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/
U8 Timer_Init(void);
U8 Timer_Deinit(void);
void Timer_Delay_MS(U32 delayTime);
U64 Timer_GetCurrentTime(void);
U8 Timer_CountDown_Start(TimerStruct_Type* timer, U32 period);
U8 Timer_CountDown_IsDone(TimerStruct_Type* timer);
 
#endif /* TIMER_HEADER */

