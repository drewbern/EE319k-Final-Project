// UART.c
// Last Modified: 04/28/2017
// Written by: Zachary Chilton & Philip Tan

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FiFo.h"

uint32_t errorCount;		//maybe for debugging
uint32_t RxCounter = 0;

// UART ISR
// uses codes from FiFo to perform certain actions
void UART1_Handler(void){
	// when ready, puts status data into FiFo
	while(!(UART1_FR_R&0x10))
		FiFo_Put((char)UART1_DR_R&0xFF);
	
	UART1_ICR_R = 0x10;	// acknowledge that interrupt occurred
}

// input 8 bits from UART
// spin if RxFifo is empty
char UART_InChar(void){
	while((UART1_FR_R&0x10) != 0){} 	// wait for RXFE
  return ((char)(UART1_DR_R&0xFF));
}

// output 8 bits to UART
// spins while T isn't ready
void UART_OutChar(char data){
  while((UART1_FR_R&UART_FR_TXFF) != 0);
  UART1_DR_R = data;
}

// initialize UART1 on PC4 (RxD) and PC5 (TxD)
// baud rate of 115200 bits/sec
void UART_Init(void){
	SYSCTL_RCGCUART_R |= 0x2;	// UART1 activated
	SYSCTL_RCGCGPIO_R |= 0x4;	// Port C clock initialized
	
	UART1_CTL_R &= ~0x1;		// UART disabled
	UART1_IBRD_R = 43;			// bus clock speed / (16 * baud rate)
	UART1_FBRD_R = 26;			// (pre-rounded IBRD value - integer IBRD value) * (message length * 4) (needs clarification)
	UART1_LCRH_R = 0x70;		// 8-bit, no parity bits, one stop, FiFos
	UART1_CTL_R = 0x301;		// UART enabled
	
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x220000;	// UART ports
	GPIO_PORTC_AMSEL_R &= ~0x30;	// disable PC4&5 analog functionality
	GPIO_PORTC_AFSEL_R |= 0x30;		// enable altfunct on PC4&5
	GPIO_PORTC_DEN_R |= 0x30;			// enable digital I/O on PC4&5
	
	//get clarifification on this section
	UART1_IM_R |= 0x10;       // ARM RXRIS
	UART1_IFLS_R |= 0x10;     // half full (bits 3-5)
	NVIC_PRI1_R |= 0x200000;  // priority 1 (bits 21-23)
	NVIC_EN0_R |= 0x40;       // enable interrupt 6 in NVIC
}
