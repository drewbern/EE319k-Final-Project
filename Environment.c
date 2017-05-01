#include "Entity.h"
#include "Player.h"
#include "tm4c123gh6pm.h"
#include "Render.h"
#include "Camera.h"
#include "Ground.h"
#include "Projectile.h"
#include "IO.h"
#include "Enemy.h"

#define MAX_OBSTACLES 7
#define MAX_ENEMIES 7

Entity obstacles[MAX_OBSTACLES];
int numObstacles = 0;
float distanceTraveled = 0;

void generateNewObstacles(void);
void checkCollisionEntityObstacle(Entity* entity);
void moveObstacles(Projectile_Collection* pCollection);
void Random_Init(uint32_t seed);
uint8_t Random(void);

void manageEnvironment(Player* player, Enemy** enemies, Projectile_Collection* pCollection,
	void increaseScore(uint32_t changeInScore)) {
		
	//Despawn old obstacles
	for(int i = 0; i < MAX_OBSTACLES; i ++) {
		if(obstacles[i].position.z <= 0) {
			obstacles[i].health = 0;
		}
	}
	
	if(distanceTraveled >= 20) {
		generateNewObstacles();
		distanceTraveled = 0;
		//beat();
	}

	
	distanceTraveled ++;
	increaseScore(1);
	
	moveObstacles(pCollection);
	checkCollisionEntityObstacle(&(*player).entity);
	
	for(int i = 0; i < MAX_ENEMIES; i ++) {
		if((**enemies).entity.health > 0) {
			checkCollisionEntityObstacle(&(**enemies).entity);
		}
	}
}

//70% chance building spawn test every 5 seconds
void generateNewObstacles(void) {
	Random_Init(NVIC_ST_CURRENT_R);
	//if(Random() > 230 && (putIndex+1)%MAX_OBSTACLES != getIndex) {
	
	for(int i = 0; i < MAX_OBSTACLES; i ++) {
		if(obstacles[i].health <= 0) {
			float xPos = ((float)(Random()-128))/20;
			//float xPos = 0;
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

void moveObstacles(Projectile_Collection* pCollection) {
	for(int i = 0; i < MAX_OBSTACLES; i ++) {
		if(obstacles[i].health > 0) {
			obstacles[i].position.z -= CAMERA_SPEED;
			obstacles[i].health -= testCollision(&obstacles[i], pCollection, ALL);
			//obstacles[i].health -= testCollision(&obstacles[i], pCollection, 3);
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

void checkCollisionEntityObstacle(Entity* entity) {
	for(int i = 0; i < MAX_OBSTACLES; i ++) {
		Entity o = obstacles[i];
		
		if(o.health > 0 &&
			//Is projectile between front and back of entity?
			o.position.z < ((*entity).position.z + (*entity).scale.z/2 + o.scale.z/2) &&  o.position.z > ((*entity).position.z - (*entity).scale.z/2 - o.scale.z/2) &&
			//Is projectile between top and bottom of entity?
			(*entity).position.y < (o.position.y + o.scale.y) &&  (*entity).position.y > o.position.y - (*entity).scale.y  && 
			//Is projectile between left and right of entity?
			(*entity).position.x > (o.position.x  - o.scale.x/2  - (*entity).scale.x/2) &&  (*entity).position.x < (o.position.x + o.scale.x/2 + (*entity).scale.x/2)) {
			
			obstacles[i].health --;
			//beat();
			(*entity).turnToRed = 1;
			(*entity).framesRedLeft = 5;
			obstacles[i].turnToRed = 1;
			obstacles[i].framesRedLeft = 5;
			beat();
		} else if (o.health > 0) {
			(*entity).framesRedLeft --;
			obstacles[i].framesRedLeft --;
			if((*entity).framesRedLeft <= 0) {
				(*entity).turnToRed = 0;
				(*entity).framesRedLeft = 0;
				obstacles[i].turnToRed = 0;
				obstacles[i].framesRedLeft = 0;
			}
		}
	}
}
