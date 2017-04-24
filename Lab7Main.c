// Lab7Main.c
// Runs on LM4F120/TM4C123
// Test the functions in ST7735.c by printing basic
// patterns to the LCD.
//    16-bit color, 128 wide by 160 high LCD
// Daniel Valvano
// Last Modified: 3/6/2015
// Ramesh Yerraballi modified 3/20/2017

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected 
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground
#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"
#include "IO.h"
#include "Print.h"

#include "Math2.h"
#include "vec2f.h"
#include "vec3f.h"
#include "vec4f.h"
#include "Matrix3f.h"
#include "Matrix4f.h"
#include "Render.h"
#include "VectorMath.h"
#include "Quaternion.h"
#include "Camera.h"
#include "Ground.h"
#include "Environment.h"
#include "Player.h"

#include "ADC.h"
#include "InputHandler.h"

void SysTick_Init(){
	//uint32_t period = 400000000;	// set period here (1 every 5 seconds)
	uint32_t period = 1000000;	
	
	NVIC_ST_CTRL_R = 0;						// disable SysTick during initialization
	NVIC_ST_RELOAD_R = period-1;	// set reload to period
	NVIC_ST_CURRENT_R = 0;				// reset current time
	NVIC_ST_CTRL_R |= 0x5;				// re-enable SysTick while enabling clock and interrupts
}

int main(void){ 
	ADC_Init();
	SysTick_Init();
	IO_Init();
	PLL_Init();                           // set system clock to 80 MHz
	ST7735_InitR(INITR_REDTAB);
	
	
	Player player = newPlayer();
	Camera camera = newCamera(&player);
	
	Entity entities[2];
  initRenderer(&camera);
  ST7735_FillScreen(0);            // set screen to black
			
	//testDrawLine();
  while(1){
		gatherInputs();
		Entity* entitiesP = entities;
		ST7735_FillScreen(0);
		renderGround(camera);
		render(&entitiesP, 0, camera);
		renderPlayer(player);
		//renderObstacles(camera);
		
		//manageEnvironment(&player);
		movePlayer(&player);
		moveCamera(&camera);
		
		//for(int i = 0; i < 1000000; i ++) {		}
		
    IO_HeartBeat();
  }
}

void testDrawLine() {
	ST7735_DrawLine(64,80,64,160, 0xFFFF);			
	ST7735_DrawLine(64,80,128,80, 0xFFFF);			
	ST7735_DrawLine(64,80,64,-1, 0xFFFF);			
	ST7735_DrawLine(64,80,-1,80, 0xFFFF);	
				
	ST7735_DrawLine(64,80,128,160, 0xFFFF);			
	ST7735_DrawLine(64,80,-1,160, 0xFFFF);			
	ST7735_DrawLine(64,80,-1,-1, 0xFFFF);			
	ST7735_DrawLine(64,80,128,-1, 0xFFFF);

	ST7735_DrawLine(-10, 10, 64,80, 0xFFFF);			
	ST7735_DrawLine(-10, 150, 64,80, 0xFFFF);			
	ST7735_DrawLine(138,10,64,80, 0xFFFF);			
	ST7735_DrawLine(138,150,64,80, 0xFFFF);			
}
