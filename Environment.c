#include "Entity.h"
#include "Player.h"
#include "tm4c123gh6pm.h"
#include "Render.h"
#include "Camera.h"
#include "Ground.h"
#include "Projectile.h"
#include "IO.h"

#define MAX_OBSTACLES 7

Entity obstacles[MAX_OBSTACLES];
int numObstacles = 0;
float distanceTraveled = 0;

void generateNewObstacles(void);
void moveObstacles(void);
void Random_Init(uint32_t seed);
uint8_t Random(void);

void manageEnvironment(Player* player) {
	//Despawn old obstacles
	for(int i = 0; i < MAX_OBSTACLES; i ++) {
		if(obstacles[i].position.z <= 0) {
			obstacles[i].health = 0;
		}
	}
	
	if(distanceTraveled >= 20) {
		generateNewObstacles();
		distanceTraveled = 0;
		beat();
	}

	
	distanceTraveled ++;
	
	moveObstacles();
}

//70% chance building spawn test every 5 seconds
void generateNewObstacles(void) {
	Random_Init(NVIC_ST_CURRENT_R);
	//if(Random() > 230 && (putIndex+1)%MAX_OBSTACLES != getIndex) {
	
	for(int i = 0; i < MAX_OBSTACLES; i ++) {
		if(obstacles[i].health <= 0) {
			float xPos = ((float)(Random()-128))/20;
			float width = ((float)(Random()+30))/70;
			float height = ((float)(Random()+70))/70;
			float depth = ((float)(Random()+50))/50;
				
			Entity newObstacle = newCube(newVector3f(xPos, 0, 28), 0, 0, 0, newVector3f(width, height, depth));
			obstacles[i] = newObstacle;
			
			
			for(int j = i; j < MAX_OBSTACLES; j ++) {
				if(obstacles[j].health <=0 && width < 1.9 && fabs(xPos) > (width/2+0.05)) {
					Entity newObstacle2 = newCube(newVector3f(-xPos, 0, 28), 0, 0, 0, newVector3f(width, height*3/2, depth));
					obstacles[j] = newObstacle2;
					break;
				}
			}
			
						
			return;			//Once spawned, dont spawn anymore so return
		}
	}
	
}

void moveObstacles(void) {
	for(int i = 0; i < MAX_OBSTACLES; i ++) {
		if(obstacles[i].health > 0) {
			obstacles[i].position.z -= CAMERA_SPEED;
		}
	}
}

void renderObstacles(void) {
	int8_t numObstacles = 0;
	Entity entitiesToRender[MAX_OBSTACLES];
	
	for(int i = 0; i < MAX_OBSTACLES; i ++) {
		if(obstacles[i].health > 0) {
			entitiesToRender[numObstacles] = obstacles[i];
			numObstacles ++;
		}
	}
	
	Entity* oP = entitiesToRender;
	render(&oP, numObstacles);
}
