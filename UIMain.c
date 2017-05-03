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

char out;
uint16_t score;

int main(void){
	PLL_Init();
	ST7735_InitR(INITR_REDTAB);
	menuInit2();
	FiFo_Init();
	UART_Init();
	
	//startMenu();
	
	drawBG();
	drawShip();
	startHealth();
	drawBombs(1);
	
	drawScore(0);
	
	while(1){
		while(FiFo_Get(&out) == 0){}
			
		score = 0;
			
		if(out == 0)
			mBeat();
			
		drawHealth(out);
		FiFo_Get(&out);
		//score += out;
		FiFo_Get(&out);
		//score += (out << 8);
		//drawScore(score);
		FiFo_Get(&out);
		drawBombs(out);
		FiFo_Get(&out);
		uint8_t gameStatus = out;
	}
}
