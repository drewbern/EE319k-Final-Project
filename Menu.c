// Menu.c

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

void menuInit(void){
	SYSCTL_RCGCGPIO_R |= 0x20;
	uint8_t delay = 0;
	delay++;
	
	GPIO_PORTF_LOCK_R = 0x4C4F434B;	// unlocking CR
	GPIO_PORTF_CR_R = 0x1F;					// Port F unlocked
	
	GPIO_PORTF_DIR_R |= ~0x11;	// PF0&4 inputs
	GPIO_PORTF_PUR_R |= 0x11;		// PF0%4 positive logic
	GPIO_PORTF_DEN_R |= 0x11;		// PF0&4 digital IO enabled
}

void startMenu(void){
	ST7735_FillScreen(0);
	
	Player player = newPlayer();
	Camera camera = newCamera(&player);
	initRenderer(&camera);
	
	Entity entities[2];
	Projectile_Collection pCollection = newProjectileCollection();
	
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
	}
}
