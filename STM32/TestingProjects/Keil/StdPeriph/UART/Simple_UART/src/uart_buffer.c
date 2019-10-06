#include "uart_buffer.h"

uint16_t rxbuf[RX_BUFFER_SIZE];
uint16_t lastReadIdx = 0;
uint16_t lastReceiveIdx = 0;

