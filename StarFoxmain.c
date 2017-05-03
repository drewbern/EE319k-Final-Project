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
#include "Menu2.h"
#include "UART.h"
#include "FiFo.h"

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

uint8_t gameStatus;	// 0 = game, 1 = game over
uint16_t score;
uint32_t frames;

void increaseScore(uint32_t changeInScore);
void sendBomb(void);

int main(void){ 
	//Construct stuff
	player = newPlayer();
	Camera camera = newCamera(&player);
	initRenderer(&camera);

  pCollection = newProjectileCollection();
	
	ADCInit();
	soundInit();
	IOInit();
	PLL_Init();									// sets system clock to 80 MHz
	ST7735_InitR(INITR_REDTAB);
	FiFo_Init();
	UART_Init();
	menuInit2();

	while(1) {	
		camera = newCamera(&player);
		player = newPlayer();
		pCollection = newProjectileCollection();
		menuInit(&camera);
		
		playMenu();
		uint8_t gameDifficulty = difficultyMenu(camera);
		gameStatus = 0;
		
		while(player.entity.health > 0){
			gatherInputs();
					
			//Game Logic
			manageEnvironment(&player, &pCollection, gameDifficulty);
			moveEnemies(&player, &pCollection, &increaseScore, gameDifficulty);
			movePlayer(&player, &pCollection);			
			moveProjectiles(&pCollection);
			moveCamera(&camera);
			
			//Rendering
			prepareRenderer(camera);
			renderGround(camera);
			renderObstacles();
			renderEnemies();
			renderPlayer(player);
			renderProjectiles(pCollection);
			renderGraphicsBuffer();
			
			
			//Last things
			if((GPIO_PORTE_DATA_R & 0x20) != 0) {
				shoot(&player, &pCollection);
			}
			if((GPIO_PORTE_DATA_R & 0x10) != 0) {
				sendBomb();
			}
			increaseScore(gameDifficulty);	// Hey, if you survived a frame, you deserve some points
			if(frames % 30 == 0)
				UART_changeStats(player.entity.health, score, player.numBombs, gameStatus);
			frames++;
					
			//IO_HeartBeat();
			
		}
		
		//Player died, show death screen
		
		removeAllEnemies();
		removeAllProjectiles(&pCollection);
		removeAllObstacles();
		deathMenu(score);
	}
}

void sendShootAction() {
	shoot(&player, &pCollection);
}

void increaseScore(uint32_t changeInScore) {
	score += changeInScore;
	
}

void sendBomb(void) {
	ST7735_FillRect(0,1,128,160, 0xFFFF);
	removeAllEnemies();
	removeAllProjectiles(&pCollection);
	removeAllObstacles();
	
	player.numBombs --;
}
