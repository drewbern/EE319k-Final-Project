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

uint8_t code;
uint16_t data;

int main(void){
	PLL_Init();
	ST7735_InitR(INITR_REDTAB);
	menuInit2();
	FiFo_Init();
	UART_Init();
	
	startMenu();
	
	while(1){
		while(FiFo_Get(&out) == 0){}
			
		FiFo_Get(&out);			// out now holds transmitted code
		uint8_t code = out;	// code now holds out
			
		FiFo_Get(&out);
		data = 0;
		data += out;			// lower byte of data now holds lower byte of data
		data &= ~0xFF00;	
		
		FiFo_Get(&out);
		uint16_t tempOut = out;
		tempOut = tempOut << 8;
		data += tempOut;	// upper byte of data now holds upper byte of data	
			
		// cases for code received to change stats/menu status
		switch (code){
			case 0:	// health update
				drawHealth(data);
			break;
			
			case 1: // bombs update
				drawBombs(data);
			break;
			
			case 2: // score update
			break;
			
			case 3:	// start game w/ initial UI
				drawBG();
				drawShip();
				startHealth();
				drawBombs(1);
			break;
			
			case 4: // game over
				// TODO game over screen
			break;
			
			default:
				ST7735_FillScreen(0xFFFF);	// fills screen with white upon invalid code
			break;
		}
	}
}
