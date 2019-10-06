#include "stdint.h"

//--------------------------------------------------
//  Global MACRO
//--------------------------------------------------
#define     RX_BUFFER_SIZE  256
#define     SEPARATOR       '\n'

//--------------------------------------------------
//  Function-like macros
//--------------------------------------------------
#define     uart_isAvailable()      (lastReceiveIdx > lastReadIdx)
#define     uart_available()        (lastReceiveIdx - lastReadIdx)
#define     uart_receive(data)      rxbuf[lastReceiveIdx] = (data); if(lastReceiveIdx<(RX_BUFFER_SIZE-1)) lastReceiveIdx++;
#define     uart_getLast()          rxbuf[lastReadIdx++]; if((lastReadIdx==lastReceiveIdx)||(lastReadIdx==256)) { lastReceiveIdx=0; lastReadIdx=0; }
#define     uart_isNewSeparator()   (rxbuf[lastReceiveIdx-1] == SEPARATOR)

//--------------------------------------------------
//  Extern data
//--------------------------------------------------
extern uint16_t lastReadIdx;
extern uint16_t lastReceiveIdx;
extern uint16_t rxbuf[];

//--------------------------------------------------
//  Functions prototype
//--------------------------------------------------
int uart_getAll(char* buffer);
