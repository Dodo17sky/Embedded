#include "stdint.h"

//--------------------------------------------------
//  Global MACRO
//--------------------------------------------------
#define     RX_BUFFER_SIZE  256

//--------------------------------------------------
//  Function-like macros
//--------------------------------------------------
#define     uart_isAvailable()      (lastReceiveIdx > lastReadIdx)
#define     uart_available()        (lastReceiveIdx - lastReadIdx)
#define     uart_receive(data)      rxbuf[lastReceiveIdx] = (data); lastReceiveIdx = (lastReceiveIdx+1)%RX_BUFFER_SIZE;
#define     uart_getLast()          rxbuf[lastReadIdx++]; if(lastReadIdx == lastReceiveIdx) { lastReceiveIdx=0; lastReadIdx=0; }

//--------------------------------------------------
//  Extern data
//--------------------------------------------------
extern uint16_t lastReadIdx;
extern uint16_t lastReceiveIdx;
extern uint16_t rxbuf[];

//--------------------------------------------------
//  Functions prototype
//--------------------------------------------------

