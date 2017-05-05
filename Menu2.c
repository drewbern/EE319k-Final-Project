// Menu2.c

#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "assets.h"
#include "FiFo.h"
#include "Menu2.h"
#include "ST7735.h"

#define startInputs ((GPIO_PORTF_DATA_R&0x1) || ((GPIO_PORTF_DATA_R&0x10) >> 4))

void menuInit2(void){
	SYSCTL_RCGCGPIO_R |= 0x20;
	uint8_t delay = 0;
	delay++;
	
	GPIO_PORTF_LOCK_R = 0x4C4F434B;	// unlocking CR
	GPIO_PORTF_CR_R = 0x1F;					// Port F unlocked
	
	GPIO_PORTF_DIR_R &= ~0x11;	// PF0&4 inputs
	GPIO_PORTF_DIR_R |= 0x1C;		// PF2 output
	//GPIO_PORTF_PUR_R |= 0x13;		// PF0&4
	//GPIO_PORTF_PDR_R |= 0x4;		// PF2
	GPIO_PORTF_DEN_R |= 0x1D;		// PF0,2,4 digital IO enabled	
}

void startMenu(void){
	menuHeader();
	char out;

	while(out != 0x7A){
		FiFo_Get(&out);
	}
		
	FiFo_Init();
	drawBG();
	drawShip();
	startHealth();
	
	ST7735_DrawString(8, 2, "SCORE", 0xFFFF);
	drawScore(0);
}

void mBeat(void){
	GPIO_PORTF_DATA_R ^= 0x4;	// menu status change LED toggled
}
