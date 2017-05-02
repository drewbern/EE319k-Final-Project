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
#include "UART.h"

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
#include "Enemy.h"

#include "GraphicsBuffer.h"

#include "Menu.h"

Player player;
Projectile_Collection pCollection;
Projectile_Collection pCollection_E;
uint16_t score;

int main(void){ 
	//Construct stuff
	player = newPlayer();
	Camera camera = newCamera(&player);
	initRenderer(&camera);
	
	//Arrays
	//Entity entities[2];
	//entities[0] = newCube(newVector3f(0,0,10), 0, 0, 0, newVector3f(1,1,1));
	//entities[0] = newEnemyEntity(newVector3f(0,0,10), 0, 0, 0, newVector3f(1,1,1));
	
	
  pCollection = newProjectileCollection();
	//addProjectile(&pCollection, newProjectile(newVector3f(0, 1, 5), newVector3f(0, 0, 0)));
	//addProjectile(&pCollection, newProjectile(newVector3f(0, 2, 5), newVector3f(0, 0, 0)));
	//addProjectile(&pCollection, newProjectile(newVector3f(0, 3, 5), newVector3f(0, 0, 0)));
	
	//ADCInit();
	soundInit();
	IOInit();
	PLL_Init();									// sets system clock to 80 MHz
	ST7735_InitR(INITR_REDTAB);
	//menuInit(&camera);
	
	//while(1) {
		//playMenu();
		
	//}
	//difficultyMenu(camera);
	
  while(1){
		//gatherInputs();
				
		//Game Logic
		shoot(&player, &pCollection);
		manageEnvironment(&player, &pCollection);
		moveEnemies(&player, &pCollection);
		movePlayer(&player, &pCollection);			
		moveProjectiles(&pCollection);
		moveCamera(&camera);
		
		//Rendering
		prepareRenderer(camera);
		//Entity* entitiesP = entities;
		//render(&entitiesP, 1);
		renderGround(camera);
		renderObstacles();
		renderEnemies();
		renderPlayer(player);
		renderProjectiles(pCollection);
		renderGraphicsBuffer();
		
    //IO_HeartBeat();
  }
}

void sendShootAction() {
	shoot(&player, &pCollection);
}

void increaseScore(uint32_t changeInScore) {
	score += changeInScore;
	uint8_t status = 0x02;
	UART_OutChar(status);
	
	uint8_t tempScore = score&0x0FF;
	UART_OutChar(tempScore);
	tempScore = (score&0xFF00) >> 8;
	UART_OutChar(tempScore);
}

