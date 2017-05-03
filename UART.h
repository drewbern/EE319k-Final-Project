// UART.h
// Last Modified: 04/28/2017
// Written by: Zachary Chilton & Philip Tan

void UART_Init(void);
char UART_InChar(void);
void UART_OutChar(char data);
void UART_changeStats(uint8_t health, uint16_t score, uint8_t bombs, uint8_t gameStatus, uint32_t frames);
