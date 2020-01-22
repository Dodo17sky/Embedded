/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "String.h"

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
@name		IntToString
@brief		
@paramIn	
@paramOut	
*--------------------------------------------------------------------------------*/
U8 IntToString(U32 number, ArraySize11* buf)
{
    U8 nmbSize = 0;
    U8 tmpStr[11] = { 0 };
    U8 idxTmp = 0;

    if(number == 0)
    {
        tmpStr[idxTmp++] = 0;
    }
    else
    {
        while (number) {
            tmpStr[idxTmp++] = (number % 10);
            number /= 10;
        }
    }

    while (idxTmp) {
        (*buf)[nmbSize++] = '0' + (tmpStr[--idxTmp]);
    }

    return nmbSize;
}

/*--------------------------------------------------------------------------------
@name		GetStrLength
@brief		Calculate number of non-null characters from the given buffer
@paramIn	buffer - the string buffer
@return     number of chars	
*--------------------------------------------------------------------------------*/
U16 GetStrLength(char* buffer)
{
    U16 charsNumber = 0;
    while(0 != buffer[charsNumber])
    {
        charsNumber++;
    }
    return charsNumber;
}


