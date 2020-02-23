/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "System.h"
#include "Port.h"
#include "Timer.h"
#include "Task.h"
#include "Uart.h"
#include "Spi.h"
#include "Adc.h"

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
@name		System_Init
@brief		First function that must be called to initialize the system
@paramIn	void
@paramOut	OK  if success
        NOK if failed
*--------------------------------------------------------------------------------*/
U8 System_Init(void)
{
    U8 retCode = RETURN_OK;

    /* Peripherals initialization */
    retCode += Adc_Init();
    retCode += Port_Init();
    retCode += Uart_Init();
    retCode += Timer_Init();
    retCode += Spi_Init();
    retCode += Task_Init();

    return retCode;
}

/*--------------------------------------------------------------------------------
@name		System_Deinit
@brief		De-initialization function of System module
@paramIn	void
@paramOut	OK  if success
            NOK if failed
*--------------------------------------------------------------------------------*/
U8 System_Deinit(void)
{
    U8 retCode = RETURN_OK;

    /* Peripherals deinitialization */
    retCode += Adc_Deinit();
    retCode += Port_Deinit();
    retCode += Uart_Deinit();
    retCode += Timer_Deinit();
    retCode += Spi_Deinit();
    retCode += Task_Deinit();

    return retCode;
}

/*--------------------------------------------------------------------------------
@name		System_Scheduler
@brief		Manage the execution of all task
@paramIn	void
@paramOut	void
*--------------------------------------------------------------------------------*/
void System_Scheduler(void)
{
    /* init thread */
    Task_InitRunnable();
    
    /* cyclic thread */
    Task_MainRunnable();
}

