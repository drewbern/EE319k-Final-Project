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
uint8_t lastHealth;

void EnableInterrupts(void);

int main(void){
	PLL_Init();
	ST7735_InitR(INITR_REDTAB);
	menuInit2();
	FiFo_Init();
	UART_Init();
	EnableInterrupts();
	
	startMenu();
	
	lastHealth = 3;
	
	while(1){	
		while(FiFo_Get(&out) == 0){}
					
		if(out == 0x2A){
			FiFo_Get(&out);		
		
			while(out != 0x7C){
				switch(out){		
					case 0x7F:
					FiFo_Get(&out);
					
					if(out != lastHealth){
						drawHealth(out);
						lastHealth = out;
					}
					
					FiFo_Get(&out);
					break;

					case 0x7E:
						FiFo_Get(&out);
						drawScore(out);
					
					FiFo_Get(&out);
					break;
				
					case 0x7D:
						FiFo_Get(&out);
						drawBombs(out);
					
					FiFo_Get(&out);
					break;
					
					default:
						FiFo_Get(&out);
				} 
			}
		}
	}
}
