#include "uart_buffer.h"

uint16_t rxbuf[RX_BUFFER_SIZE];
uint16_t lastReadIdx = 0;
uint16_t lastReceiveIdx = 0;

int uart_getAll(char* buffer)
{
    uint16_t bufSize = 0;
    
    while(uart_isAvailable()) {
        buffer[bufSize] = uart_getLast();
        bufSize++;
    }
    
    return bufSize;
}