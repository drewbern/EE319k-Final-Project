#include "stdint.h"
#include "Player.h"
#include "Projectile.h"
#include "Enemy.h"
#include "graphicsBuffer.h"
#include "VectorMath.h"
#include "IO.h"
#include "Render.h"

#define MAX_ENEMIES 7
#define DESPAWN_DISTANCE 25
#define PROJECTILE_SPEED 0.6
#define ENEMY_SPAWN_TIMER_MAX 100

Enemy enemies[MAX_ENEMIES];
float enemySpawnTimer = 0;

void generateNewEnemies(void);
void Random_Init(uint32_t seed);
uint8_t Random(void);

Enemy newEnemy(Vector3f position, Vector3f velocity) {
	Enemy out  = {BASIC,
		newEnemyEntity(position, 0,0,0, newVector3f(1,1,1)),
		position,	velocity,
		20, 0};	//Change with randomized position and velocity and reload
	return out;
}

void moveEnemies(Player* player, Projectile_Collection* pCollection) {
	//Despawn old enemies
	for(int i = 0; i < MAX_ENEMIES; i ++) {
		if(enemies[i].position.z < 0 || enemies[i].position.z > DESPAWN_DISTANCE) {
			enemies[i].entity.health = 0;
		}
	}
	
	//Check hits, Check fire & move
	for(int i = 0; i < MAX_ENEMIES; i ++) {
		if(enemies[i].entity.health > 0) {
			enemies[i].position = add_vec3f(enemies[i].position, enemies[i].velocity); 
			enemies[i].entity.position = enemies[i].position;
			
			enemies[i].entity.health -= testCollision(&enemies[i].entity, pCollection, PLAYER_PROJECTILE);
			//enemies[i].entity.health -= testCollision(&enemies[i].entity, pCollection, 1);
			enemies[i].reloadCounter = fmax(enemies[i].reloadCounter - 1, 0);
			
			
			if(enemies[i].reloadCounter <= 0) {
				//beat();
				Vector3f velocity = sub_vec3f((*player).position, enemies[i].position);
				velocity = normalize_vec3f(velocity);
				velocity = scale_vec3f(velocity, PROJECTILE_SPEED);

				addProjectile(pCollection, newProjectile(enemies[i].position, velocity, ENEMY_PROJECTILE));
				
				enemies[i].reloadCounter = enemies[i].reloadTime;
			}
		}
	}

	//Spawn new enemies
	if(enemySpawnTimer <= 0) {
		generateNewEnemies();
		enemySpawnTimer = ENEMY_SPAWN_TIMER_MAX;
		//beat();
	}
	enemySpawnTimer --;;
}

#define ENEMY_FRONT_SPEED 0.1
#define ENEMY_BACK_SPEED -0.05

void generateNewEnemies(void) {
	Random_Init(NVIC_ST_CURRENT_R);
	
	for(int i = 0; i < MAX_ENEMIES; i ++) {
		if(enemies[i].entity.health <= 0) {
			beat();
			float xPos = ((float)(Random())-127)/32;
			float yPos = ((float)(Random())/255)+2;
			float zPos = DESPAWN_DISTANCE;
			float zVel = ENEMY_BACK_SPEED;
			
			if(Random() > 127) {
				zPos = 0;
				zVel = ENEMY_FRONT_SPEED;
			}
			
			Vector3f velocity =  newVector3f(-xPos/500, 0, zVel);
			
			Enemy enemy1 = newEnemy(newVector3f(xPos, yPos, zPos), velocity);
			enemies[i] = enemy1;
			
			
			Random_Init(NVIC_ST_CURRENT_R);
			if(Random() > 127) {
				for(int j = i; j < MAX_ENEMIES; j ++) {
					if(enemies[j].entity.health <=0) {
						Enemy enemy2 = newEnemy(newVector3f(xPos+1.5, yPos, zPos), velocity);
						enemies[j] = enemy2;
						break;
					}
				}	
			}
			Random_Init(NVIC_ST_CURRENT_R);
			if(Random() > 127) {
				for(int j = i; j < MAX_ENEMIES; j ++) {
					if(enemies[j].entity.health <=0) {
						Enemy enemy3 = newEnemy(newVector3f(xPos-1.5, yPos, zPos), velocity);
						enemies[j] = enemy3;
						break;
					}
				}	
			}
			return;			//Once spawned, dont spawn anymore so return
				
		}
	}
	
}

void renderEnemies(void) {
	int8_t numEnemies = 0;
	Entity entitiesToRender[MAX_ENEMIES];
	
	for(int i = 0; i < MAX_ENEMIES; i ++) {
		if(enemies[i].entity.health > 0) {
			entitiesToRender[numEnemies] = enemies[i].entity;
			numEnemies ++;
		}
	}
	
	Entity* oP = entitiesToRender;
	render(&oP, numEnemies);
}

