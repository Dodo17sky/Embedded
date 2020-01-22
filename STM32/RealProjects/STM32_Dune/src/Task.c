/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Task.h"
#include "Timer.h"

/* software components includes */
#include "Lighting.h"
#include "Com.h"

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/
#define TASK_NUMBER         5
#define NO_TASK_RUNNABLE    0
#define MAX_TASK_PERIOD     0xFFFF

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/
TaskConfiguration_Type taskList[] = 
{
    /*  task init function          ,   task cyclic function        ,     task period [ms]    */
    {   &Lighting_InitRunnable      ,   &Lighting_MainRunnable      ,     20                  },
    {   &Com_InitRunnable           ,   &Com_MainRunnable           ,     10                  }
};

const U8 TASKS_NUMBER = sizeof(taskList)/sizeof(taskList[0]);

U32 lastRun[TASKS_NUMBER] = {0};

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
@name		Task_Init
@brief		Initialize module
@paramIn	void
@paramOut	OK - all task have been schedueled
            NOK - task init errors
*--------------------------------------------------------------------------------*/
U8 Task_Init(void)
{
    return RETURN_OK;
}

/*--------------------------------------------------------------------------------
@name		Task_InitRunnable
@brief		Call the initialization functions for all tasks
@paramIn	void
@paramOut	
*--------------------------------------------------------------------------------*/
void Task_InitRunnable(void)
{
    U8 index = 0;
    
    for(index=0; index<TASKS_NUMBER; index++)
    {
        taskList[index].taskInitRunnable();
    }
}

/*--------------------------------------------------------------------------------
@name		Task_MainRunnable
@brief		This task run forever. This will launch all the task in this system
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/
void Task_MainRunnable()
{
    U64 currentTime = 0;
    
    while(1)
    {
        /*************************************************
         *   ROUND ROBIN method
         *************************************************/
        
        currentTime = Timer_GetCurrentTime();
        for(U8 index=0; index<TASKS_NUMBER; index++)
        {
            if(currentTime >= (lastRun[index] + taskList[index].taskPeriod))
            {
                if(taskList[index].taskMainRunnable != NO_TASK_RUNNABLE)
                {
                    taskList[index].taskMainRunnable();
                    lastRun[index] = currentTime;
                }
            }
        }       
    }
}