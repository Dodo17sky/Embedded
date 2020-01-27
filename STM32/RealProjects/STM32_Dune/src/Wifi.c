/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "Wifi.h"
#include "Uart.h"
#include "Timer.h"
#include "String.h"
#include "Utils.h"

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/
#define UART_CHN_USER       USART1
#define UART_CHN_ESP        USART3

#define CMD_BUFFER_SIZE     (UART_RX_BUFFER_SIZE + 2) /* 2 extra space for \r\n chars */

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/
static char CmdBuf[CMD_BUFFER_SIZE];
static U8 isNewCmdAvailable;

/*--------------------------------------------------------------------------------
*                           Functions prototypes declaration
*--------------------------------------------------------------------------------*/
static U8 Read_UserInputs(void);
static U8 Forward_CmdToWifi(void);
static U8 Read_WifiModuleMessages(void);
static U8 SendNewCommand(char* cmdBuf);

/*--------------------------------------------------------------------------------
*                           Functions prototypes definition
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
@name       Wifi_Init
@brief      
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U8 Wifi_Init(void)
{
    U8 retCode = RETURN_OK;
    
    isNewCmdAvailable = FALSE;

    return retCode;
}

/*--------------------------------------------------------------------------------
@name       Wifi_Deinit
@brief      
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
U8 Wifi_Deinit(void)
{
    U8 retCode = RETURN_OK;
    
    return retCode;
}

/*--------------------------------------------------------------------------------
@name       Wifi_MainFunction
@brief      This function shall be called cyclically by an upper layer component
            to handle continous user commands
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
void Wifi_MainFunction()
{
    /* Read any commands from user */
    Read_UserInputs();
    
    /* Send new arrived commands from user, to ESP module */
    Forward_CmdToWifi();
    
    /* Read messages from Wifi module */
    Read_WifiModuleMessages();
}

/*--------------------------------------------------------------------------------
@name       Wifi_MainFunction
@brief      This function shall be called cyclically by an upper layer component
            to handle continous user commands
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
static U8 Read_UserInputs(void)
{
    U8 retCode = RETURN_OK;
    U16 nmbBytesRead = 0;
    char userCmdBuf[128] = {0};

    nmbBytesRead = Uart_RB_Read(UART_CHN_USER, userCmdBuf, 128);
    if(0 < nmbBytesRead)
    {
        /* check if EndOfCommand char ('\n') was received */
        if('\n' == userCmdBuf[nmbBytesRead-1])
        {
            SendNewCommand(userCmdBuf);
        }
        else
        {
            /* TBD: handle partial command */
        }
    }
    
    return retCode;
}

/*--------------------------------------------------------------------------------
@name       Forward_CmdToWifi
@brief      This function forwards commands from user to ESP board
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
static U8 Forward_CmdToWifi(void)
{
    U8 retCode = RETURN_OK;
    U8 dataLength = 0;
    U8 index = 0;
    
    if(TRUE == isNewCmdAvailable)
    {
        dataLength = GetStrLength(CmdBuf);
        Uart_Send_String(UART_CHN_ESP, CmdBuf, dataLength);
        ZERO_FILL_ARRAY(CmdBuf);
        isNewCmdAvailable = FALSE;
    }
    
    return retCode;
}

/*--------------------------------------------------------------------------------
@name       Wifi_MainFunction
@brief      This function shall be called cyclically by an upper layer component
            to handle continous user commands
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
static U8 Read_WifiModuleMessages(void)
{
    U8 retCode = RETURN_OK;
    U16 dataRead = 0;
    char espResponse[400];

    dataRead = Uart_RB_Read(UART_CHN_ESP, espResponse, 400);
    if(0 < dataRead)
    {
        Uart_Send_String(UART_CHN_USER, espResponse, dataRead);
    }
    
    return retCode;
}

/*--------------------------------------------------------------------------------
@name       SendNewCommand
@brief      Send new command to ESP01 board
@paramIn    
@paramOut   
*--------------------------------------------------------------------------------*/
static U8 SendNewCommand(char* cmdBuf)
{
    U8 retCode = RETURN_OK;
    U8 readStatus = RETURN_NOK;
    U8 dataLength = 0;
    U8 index = 0;
    
    dataLength = GetStrLength((char*)cmdBuf);
    
    /* escape any \r or \n from the tail */
    while( (cmdBuf[dataLength-1]=='\r') || (cmdBuf[dataLength-1]=='\n') )
    {
        --dataLength;
    }
    
    for(index=0; index<dataLength; index++)
    {
        CmdBuf[index] = cmdBuf[index];
    }
    CmdBuf[index] = '\r';
    CmdBuf[index+1] = '\n';

    isNewCmdAvailable = TRUE;
    
    return retCode;
}




