// IO.c
// This software configures the switch and LED
// You are allowed to use any switch and any LED, 
// although the Lab suggests the SW1 switch PF4 and Red LED PF1
// Runs on LM4F120 or TM4C123
// Program written by: Zachary Chilton & Philip Tan
// Last Modified: 03/27/2017
// Lab number: 6

#include "tm4c123gh6pm.h"
#include <stdint.h>

#define delayCounter 320000

//------------IO_Init------------
// Initialize GPIO Port for a switch and an LED
// Input: none
// Output: none
void IO_Init(void) {
 // --UUU-- Code to initialize PF4 and PF1
	uint8_t delay = 1;
	SYSCTL_RCGCGPIO_R |= 0x20;
	delay--;
	
	GPIO_PORTF_LOCK_R = 0x4C4F434B;  
	GPIO_PORTF_CR_R = 0x1F;           //allow changes to PF4-0
	GPIO_PORTF_DIR_R |= 0x2;					//PF1 set as output
	GPIO_PORTF_DIR_R &= 0xEF;					//PF4 set as input
	GPIO_PORTF_DEN_R |= 0x12;
	GPIO_PORTF_PUR_R |= 0x40;
}

//------------IO_HeartBeat------------
// Toggle the output state of the  LED.
// Input: none
// Output: none
void IO_HeartBeat(void) {
 // --UUU-- PF1 is heartbeat
	GPIO_PORTF_DATA_R ^= 0x2;	//red LED toggled
}

//------------IO_Touch------------
// wait for release and press of the switch
// Delay to debounce the switch
// Input: none
// Output: none
void IO_Touch(void) {
 // --UUU-- wait for release; delay for 20ms; and then wait for press
	uint32_t delay = delayCounter;
	delay--;	//set delay counter correctly
	
	while(((GPIO_PORTF_DATA_R&0x10) >> 4) == 1){	//waiting for press
	}
	while(delay != 0)
		--delay;
	while(((GPIO_PORTF_DATA_R&0x10) >> 4) == 0){	//waiting for depress
	}
}  
