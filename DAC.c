// dac.c
// This software sets up the DAC for 4-bit audio output
// Program written by: Zachary Chilton & Philip Tan
// Last Modified: 04/24/2017
// Hardware connections
// PB0-3 for DAC input

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Sound.h"

// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	uint8_t delay = 0;
	SYSCTL_RCGCGPIO_R |= 0x2;
	delay ++;
	
	GPIO_PORTB_DEN_R |= 0x3F;
	GPIO_PORTB_DIR_R |= 0x3F;
	GPIO_PORTB_DR8R_R |= 0x3F;
	
	soundInit();
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 127 
// Output: none
void DAC_Out(uint8_t data){
	GPIO_PORTB_DATA_R = data;
}
