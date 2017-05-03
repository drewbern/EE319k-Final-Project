// UART.h
// Last Modified: 04/28/2017
// Written by: Zachary Chilton & Philip Tan

#include <stdint.h>

void UART_Init(void);
char UART_InChar(void);
void UART_OutChar(char data);
void UART_changeStats(uint8_t health, uint16_t score, uint8_t bombs);
