// StarFox Game for Lab 10 Competition
// By Philip Tan and Zach Chilton
// Spicy game

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
#include "Sound.h"

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
#include "Projectile.h"

#include "GraphicsBuffer.h"

#include "Menu.h"

void testDrawLine(void);

int main(void){ 
	//Construct stuff
	Player player = newPlayer();
	Camera camera = newCamera(&player);
	initRenderer(&camera);
	
	//Arrays
	Entity entities[2];
	//entities[0] = newCube(newVector3f(0,0,10), 0, 0, 0, newVector3f(1,1,1));
  Projectile_Collection pCollection = newProjectileCollection();
	//addProjectile(&pCollection, newProjectile(newVector3f(0, 1, 5), newVector3f(0, 0, 0)));
	//addProjectile(&pCollection, newProjectile(newVector3f(0, 2, 5), newVector3f(0, 0, 0)));
	//addProjectile(&pCollection, newProjectile(newVector3f(0, 3, 5), newVector3f(0, 0, 0)));
	
	//ADCInit();
	soundInit();
	IOInit();
	PLL_Init();									// sets system clock to 80 MHz
	ST7735_InitR(INITR_REDTAB);
	
	menuInit();
	// start menu testing
	//while(1){
	//	startMenu();
	//}
	
  while(1){
		gatherInputs();
		
		//Rendering
		prepareRenderer(camera);
		Entity* entitiesP = entities;
		renderGround(camera);
		renderObstacles();
		render(&entitiesP, 1);
		renderPlayer(player);
		renderProjectiles(pCollection);
		renderGraphicsBuffer();
		
		//Game Logic
		manageEnvironment(&player);
		movePlayer(&player, &pCollection);			
		moveProjectiles(&pCollection);
		moveCamera(&camera);
		
		//for(int i = 0; i < 1000000; i ++) {		}
		
    //IO_HeartBeat();
  }
}

void testDrawLine() {
	/*
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
*/
	//drawLine(64,80,64,160, 0xFF);			
	//drawLine(64,80,128,80, 0xFF);			
	//drawLine(64,80,64,-10, 0xFF);			
	
	/*
	drawLine(64,80,-1,80, 0xFF);	
				
	drawLine(64,80,128,160, 0xFF);			
	drawLine(64,80,-1,160, 0xFF);			
	drawLine(64,80,-1,-1, 0xFF);			
	drawLine(64,80,128,-1, 0xFF);

	
	drawLine(-10, 10, 64,80, 0xFF);			
	drawLine(-10, 150, 64,80, 0xFF);			
	drawLine(138,10,64,80, 0xFF);			
	drawLine(138,150,64,80, 0xFF);	
	*/
}
