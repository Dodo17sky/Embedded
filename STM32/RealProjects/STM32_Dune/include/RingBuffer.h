#ifndef RINGBUFFER_HEADER
#define RINGBUFFER_HEADER

/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "GeneralTypes.h"

/*--------------------------------------------------------------------------------
*                           Custom types
*--------------------------------------------------------------------------------*/
typedef struct{
    char* buffer;
    U16 size;
    U16 head;
    U16 tail;
    U16 overflowError;
} RingBuffer_Type;

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/
#define RB_Increment_Head(rb)   (rb)->head = (((rb)->head + 1) % (rb)->size)

#define RB_Increment_Tail(rb)   (rb)->tail = (((rb)->tail + 1) % (rb)->size)

#define RB_Init(rb, bufPtr, bufSize)        \
    (rb)->buffer = bufPtr;                  \
    (rb)->size = bufSize;                   \
    (rb)->head = 0;                         \
    (rb)->tail = 0;                         \
    (rb)->overflowError = 0

#define RB_Write(rb, data)                  \
    if(((rb)->head+1)==(rb)->tail) {            \
        (rb)->overflowError++;                \
    } else {                                \
        (rb)->buffer[(rb)->head] = data;        \
        RB_Increment_Head(rb);              \
    }

#define RB_Read(rb, outBufPtr, outBufSize, bytesRead)                   \
    {   U16 idx = 0;                                                    \
        for(idx=0; ((idx<outBufSize) && ((rb)->tail!=(rb)->head)); idx++) { \
            outBufPtr[idx] = (rb)->buffer[(rb)->tail];                      \
            RB_Increment_Tail(rb); }                                    \
        bytesRead = idx;                                                \
    }

/*--------------------------------------------------------------------------------
*                           Function-like macros
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Typedef's
*--------------------------------------------------------------------------------*/


#endif /* RINGBUFFER_HEADER */

