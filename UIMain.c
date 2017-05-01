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
#include "Menu.h"
#include "FiFo.h"
#include "UART.h"

char status;
char lastStatus;

int main(void){
	PLL_Init();
	ST7735_InitR(INITR_REDTAB);
	menuInit();
	FiFo_Init();
	UART_Init();
	
	startMenu();
	
	while(1){
		while(FiFo_Get(&status) == 0){}
			
		FiFo_Get(&status);				// status now holds transmitted data
		if(status != lastStatus){	// only continues if status changed
			
		mBeat();	
		uint8_t code = status&0x0F;	// lower byte holds code
		uint8_t data = status&0xF0;	// upper byte holds data
			
		// cases for code received to change stats/menu status
		switch (code){
			case 0:	// health update
				drawHealth(data);
			break;
			
			case 1: // bombs update
				drawBombs(data);
			break;
			
			case 2:	// start game w/ initial UI
				drawBG();
				drawShip();
				startHealth();
				drawBombs(1);
			break;
			
			case 3: // game over
				// TODO game over screen
			break;
			
			default:
				ST7735_FillScreen(0xFFFF);	// fills screen with white upon invalid code
			break;
		}
		}
	}
}
