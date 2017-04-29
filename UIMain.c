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

int main(void){
	PLL_Init();
	ST7735_InitR(INITR_REDTAB);
	
	drawBG();
	drawLives(3);
	drawBombs(3);
}
