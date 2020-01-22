/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Timer.h"
#include "stm32f10x.h"

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/
static volatile U64 TimingDelay;
static volatile U64 SystemTime;

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
@name		Timer_Init
@brief		Initialization function of Timer module
@paramIn	void
@paramOut	OK  if success
            NOK if failed
*--------------------------------------------------------------------------------*/
U8 Timer_Init(void)
{
    
    U8 retCode = RETURN_OK;
     
    // Nomatter what is the freq. of CPU,
    // the System Tick Interrupt will be called 1000 times/second,
    // meaning at 1 millisecond
	SysTick_Config(SystemCoreClock / 1000);
     
    /* initialize global timer variable */
    TimingDelay = 0;
    
    /* initialize system time */
    SystemTime = 0;
        
    return retCode;
}

/*--------------------------------------------------------------------------------
@name		Timer_Delay_MS
@brief		Delay execution for delayTime milliseconds
@paramIn	U32 delayTime: time to be delayed
@paramOut	void
*--------------------------------------------------------------------------------*/
void Timer_Delay_MS(U32 delayTime)
{
    TimingDelay = delayTime;
    while(TimingDelay != 0)
    {
        /* wait */
    }
}

/*--------------------------------------------------------------------------------
@name		GetCurrentTime
@brief		Get the value of global system time
@paramIn	void
@paramOut	SystemTime - global time stamp in milliseconds
*--------------------------------------------------------------------------------*/
U64 Timer_GetCurrentTime(void)
{
    return SystemTime;
}

/*--------------------------------------------------------------------------------
@name		Timer_CountDown_Start
@brief		Mark the beginning of a new period
@paramIn	Pointer to timer to be reset
@paramOut	
*--------------------------------------------------------------------------------*/
U8 Timer_CountDown_Start(TimerStruct_Type* timer, U32 period)
{
    U8 retCode = RETURN_OK;

    timer->period = period;
    timer->lastTimestamp = Timer_GetCurrentTime();
    
    return retCode;
}

/*--------------------------------------------------------------------------------
@name		Timer_CountDown_IsDone
@brief		Check if the timer has elapsed
@paramIn	Pointer to timer to be verified
@paramOut	TIMER_ELAPSED - timer has elapsed
            TIMER_NOT_ELAPSED - timer has not elapsed
*--------------------------------------------------------------------------------*/
U8 Timer_CountDown_IsDone(TimerStruct_Type* timer)
{
    U8 timerStatus = TIMER_NOT_ELAPSED;
    U32 now = Timer_GetCurrentTime();
    
    if(now >= (timer->lastTimestamp + timer->period))
    {
        timerStatus = TIMER_ELAPSED;
    }
    
    return timerStatus;
}

/*--------------------------------------------------------------------------------
@name		SysTick_Handler
@brief		ISR for system tick
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/ 
void SysTick_Handler(void)
{
    if (TimingDelay != 0)
    {
        TimingDelay--;
    }
    SystemTime++;
}


