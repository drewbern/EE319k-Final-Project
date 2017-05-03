// UI.c
// Written by: Zachary Chilton & Philip Tan
// Last modified: 04/28/2017
// Uses UART to implement user interface

#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "Print.h"
#include "PLL.h"
#include "ST7735.h"

#include "assets.h"
#include "Menu2.h"
#include "FiFo.h"
#include "UART.h"

char out = 0;
uint8_t score;

int main(void){
	PLL_Init();
	ST7735_InitR(INITR_REDTAB);
	menuInit2();
	FiFo_Init();
	UART_Init();
	
	startMenu();
	
	while(FiFo_Get(&out) == 0){}

	drawBG();
	drawShip();
	startHealth();
	
	drawScore(0);
		
	FiFo_Init();
	
	while(1){
		while(FiFo_Get(&out) == 0){}
		if(out == 0x2A){	
		
		while(FiFo_Get(&out) == 0){}
		drawHealth(out);
		
		while(FiFo_Get(&out) == 0){}
		drawScore(out);	
			
		while(FiFo_Get(&out) == 0){}
		drawBombs(out);
		}
	}
}
