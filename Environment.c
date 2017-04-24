#include "Entity.h"
#include "Player.h"
#include "Random.h"
#include "tm4c123gh6pm.h"
#include "Render.h"
#include "Camera.h"

#define CAMERA_SPEED 0.1																				//Virtual speed of camera
#define MAX_OBSTACLES 5

Entity obstacles[MAX_OBSTACLES];
int putIndex = 0;
int getIndex = 0;
int numObstacles = 0;
float distanceTraveled = 0;

void generateNewObstacles(void);
void moveObstacles(void);

void manageEnvironment(Player* player) {
	//if((NVIC_ST_CTRL_R | 0x010000) ==1) {
	if(distanceTraveled >= 50) {
		generateNewObstacles();
		distanceTraveled = 0;
		//NVIC_ST_CTRL_R &= ~(0x010000);
		GPIO_PORTF_DATA_R ^= 0x2;	//red LED toggled
	}
	distanceTraveled ++;
	
	moveObstacles();
	
	//Despawn old obstacles
	if(obstacles[getIndex].position.z <= 0 && getIndex != putIndex) {
		getIndex ++;
		numObstacles --;
	}
}

//70% chance building spawn test every 5 seconds
void generateNewObstacles(void) {
	if(randomInt(10) > 7 && (putIndex+1)%MAX_OBSTACLES != getIndex) {
		float xPos = (randomInt(30)-15)/10;
		float width = (randomInt(150)+50)/100;
		float height = (randomInt(400)+50)/100;
		float depth = (randomInt(300)+50)/100;
		Entity newObstacle = newCube(newVector3f(xPos, 0, 8), 0, 0, 0, newVector3f(width, height, depth));
		obstacles[putIndex] = newObstacle;
		putIndex ++;
		numObstacles ++;
	}
}

void moveObstacles(void) {
	for(int i = 0; i < numObstacles; i++) {
		int index = (i + getIndex)%MAX_OBSTACLES;
		obstacles[index].position.z -= CAMERA_SPEED;
	}
}

void renderObstacles(Camera camera) {
	Entity* oP = obstacles;
	render(&oP, numObstacles, camera);
}
