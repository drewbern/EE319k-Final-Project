#include "vec3f.h"
#include "VectorMath.h"
#include "stdint.h"
#include "Render.h"
#include "GraphicsBuffer.h"
#include "Entity.h"
#include "IO.h"

#define CAMERA_Z -8.3				//The focal point
#define MAX_PROJECTILE_DISTANCE 20
#define MAX_PROJECTILES_IN_COLLECTION 25
#define PROJECTILE_HITBOX_SIZE	0.13							//The "radius" of hitbox

const uint8_t Projectile_10[] = {
	BLACK, BLACK, BLACK, DARK_YELLOW, YELLOW, YELLOW, DARK_YELLOW, BLACK, BLACK, BLACK,
	BLACK, DARK_YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, DARK_YELLOW, BLACK,
	BLACK, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK,
	DARK_YELLOW, YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW, DARK_YELLOW,
	YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,
	YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,
	DARK_YELLOW, YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW, DARK_YELLOW,
	BLACK, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK,
	BLACK, DARK_YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, DARK_YELLOW, BLACK,
	BLACK, BLACK, BLACK, DARK_YELLOW, YELLOW, YELLOW, DARK_YELLOW, BLACK, BLACK, BLACK
};

const uint8_t Projectile_8[] = {
	BLACK, BLACK, DARK_YELLOW, YELLOW, YELLOW, DARK_YELLOW, BLACK, BLACK,
	BLACK, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK,
	DARK_YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, DARK_YELLOW,
	YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 
	YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 
	DARK_YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, DARK_YELLOW,
	BLACK, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK,
	BLACK, BLACK, DARK_YELLOW, YELLOW, YELLOW, DARK_YELLOW, BLACK, BLACK
};

const uint8_t Projectile_6[] = {
	BLACK, DARK_YELLOW, YELLOW, YELLOW, DARK_YELLOW, BLACK,
	DARK_YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, DARK_YELLOW,
	YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 
	YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 
	DARK_YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, DARK_YELLOW,
	BLACK, DARK_YELLOW, YELLOW, YELLOW, DARK_YELLOW, BLACK
};

const uint8_t Projectile_4[] = {
	DARK_YELLOW, YELLOW, YELLOW, BLACK,
	YELLOW, YELLOW, YELLOW, YELLOW,
	YELLOW, YELLOW, YELLOW, DARK_YELLOW,
	BLACK, YELLOW, DARK_YELLOW, BLACK
};

const uint8_t Projectile_3[] = {
	DARK_YELLOW, YELLOW, DARK_YELLOW,
	YELLOW, YELLOW, YELLOW,
	DARK_YELLOW, YELLOW, DARK_YELLOW
};

const uint8_t Projectile_2[] = {
	YELLOW, YELLOW,
	YELLOW, YELLOW
};


//ENEMY PROJECTILES
const uint8_t Projectile_10_E[] = {
	BLACK, BLACK, BLACK, ORANGE, ORANGE, ORANGE, ORANGE, BLACK, BLACK, BLACK,
	BLACK, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, BLACK,
	BLACK, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, BLACK,
	ORANGE, ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE, ORANGE,
	ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,
	ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,
	ORANGE, ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE, ORANGE,
	BLACK, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, BLACK,
	BLACK, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, BLACK,
	BLACK, BLACK, BLACK, ORANGE, ORANGE, ORANGE, ORANGE, BLACK, BLACK, BLACK
};

const uint8_t Projectile_8_E[] = {
	BLACK, BLACK, ORANGE, ORANGE, ORANGE, ORANGE, BLACK, BLACK,
	BLACK, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, BLACK,
	ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE,
	ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 
	ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 
	ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE,
	BLACK, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, BLACK,
	BLACK, BLACK, ORANGE, ORANGE, ORANGE, ORANGE, BLACK, BLACK
};

const uint8_t Projectile_6_E[] = {
	BLACK, ORANGE, ORANGE, ORANGE, ORANGE, BLACK,
	ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE,
	ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 
	ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 
	ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE,
	BLACK, ORANGE, ORANGE, ORANGE, ORANGE, BLACK
};

const uint8_t Projectile_4_E[] = {
	BLACK, ORANGE, ORANGE, BLACK,
	ORANGE, ORANGE, ORANGE, ORANGE,
	ORANGE, ORANGE, ORANGE, ORANGE,
	BLACK, ORANGE, ORANGE, BLACK
};

const uint8_t Projectile_3_E[] = {
	BLACK, ORANGE, BLACK,
	ORANGE, ORANGE, ORANGE,
	BLACK, ORANGE, BLACK
};

const uint8_t Projectile_2_E[] = {
	ORANGE, ORANGE,
	ORANGE, ORANGE
};

Projectile_Collection newProjectileCollection() {
	Projectile_Collection out = {
		{newProjectile(newVector3f(0,0,0),newVector3f(0,0,0),ENEMY_PROJECTILE)}
	};
	
	return out;
}

Projectile newProjectile(Vector3f position, Vector3f velocity, enum ProjectileType type) {
	Projectile out = {position, velocity, 1, type};
	return out;
}

void addProjectile(Projectile_Collection* pCollection, Projectile projectile) {
	for(int i = 0; i < MAX_PROJECTILES_IN_COLLECTION; i ++) {
		if((*pCollection).projectiles[i].live == 0) {
			(*pCollection).projectiles[i] = projectile;
			(*pCollection).projectiles[i].live = 1;
			return;
		}
	}
}

void removeProjectile(Projectile_Collection* pCollection, int index) {
	(*pCollection).projectiles[index].live = 0;
	(*pCollection).projectiles[index].position.y = -100;
	(*pCollection).projectiles[index].velocity = newVector3f(0,0,0);
}

void moveProjectiles(Projectile_Collection* pCollection) {
	for(int i = 0; i < MAX_PROJECTILES_IN_COLLECTION; i ++) {
		if((*pCollection).projectiles[i].position.z >= MAX_PROJECTILE_DISTANCE || (*pCollection).projectiles[i].position.z < 0 ||
					(*pCollection).projectiles[i].position.y <= 0) {
			(*pCollection).projectiles[i].live = 0;		
		}
		
		if((*pCollection).projectiles[i].live != 0) {
			(*pCollection).projectiles[i].position = add_vec3f((*pCollection).projectiles[i].position,
				(*pCollection).projectiles[i].velocity);
		}
	}
}

/**
Return 0 if no hit, 1 if hit
**/
uint8_t testCollision(Entity* entity, Projectile_Collection* pCollection, enum ProjectileType typeToTest) {
	uint8_t hit = 0;
	
	
	for(int i = 0; i < MAX_PROJECTILES_IN_COLLECTION; i ++) {
		//Projectile p = (*pCollection).projectiles[i];
	
		if((*pCollection).projectiles[i].live != 0 && ((*pCollection).projectiles[i].type == typeToTest || typeToTest == ALL) && 
			//Is projectile between front and back of entity?
			(*pCollection).projectiles[i].position.z < ((*entity).position.z + (*entity).scale.z/2 + PROJECTILE_HITBOX_SIZE) && 
			(*pCollection).projectiles[i].position.z > ((*entity).position.z - (*entity).scale.z/2 - PROJECTILE_HITBOX_SIZE) &&
			//Is projectile between top and bottom of entity?
			(*pCollection).projectiles[i].position.y < ((*entity).position.y + (*entity).scale.y + PROJECTILE_HITBOX_SIZE) &&
			(*pCollection).projectiles[i].position.y > ((*entity).position.y - PROJECTILE_HITBOX_SIZE) && 
			//Is projectile between left and right of entity?
			(*pCollection).projectiles[i].position.x < ((*entity).position.x + (*entity).scale.x/2 + PROJECTILE_HITBOX_SIZE) &&
			(*pCollection).projectiles[i].position.x > ((*entity).position.x - (*entity).scale.x/2) - PROJECTILE_HITBOX_SIZE) {
			
			removeProjectile(pCollection, i);
			hit = 1;
			(*entity).turnToRed = 1;
			(*entity).framesRedLeft = 5;
			//beat();
		} else if ((*pCollection).projectiles[i].live != 0) {
			(*entity).framesRedLeft --;
			if((*entity).framesRedLeft <= 0) {
				(*entity).turnToRed = 0;
				(*entity).framesRedLeft = 0;
			}
		}
	}
	
	return hit;
}

void renderProjectiles(Projectile_Collection pCollection) {
	for(int i = 0; i < MAX_PROJECTILES_IN_COLLECTION; i ++) {
		if(pCollection.projectiles[i].live != 0) {
			float size = 10 * -CAMERA_Z / (pCollection.projectiles[i].position.z - CAMERA_Z);
			
			Vector2f screenPos = preparePointSimple(pCollection.projectiles[i].position);
			
			if(pCollection.projectiles[i].type == PLAYER_PROJECTILE) {
				if(size > 8) {
					drawImage(screenPos.x - 5, screenPos.y - 5, Projectile_10, 10, 10);
				} else if (size > 6) {
					drawImage(screenPos.x - 4, screenPos.y - 4, Projectile_8, 8, 8);
				} else if (size > 4) {
					drawImage(screenPos.x - 3, screenPos.y - 3, Projectile_6, 6, 6);
				} else if (size > 3) {
					drawImage(screenPos.x - 2, screenPos.y - 5, Projectile_4, 4, 4);
				} else if (size > 2) {
					drawImage(screenPos.x - 1.5, screenPos.y - 1.5, Projectile_3, 3, 3);
				} else if (size > 1) {
					drawImage(screenPos.x - 1, screenPos.y - 1, Projectile_2, 2, 2);
				} else {
					drawPixel(screenPos.x, screenPos.y, YELLOW);
				}
			} else {
				if(size > 8) {
					drawImage(screenPos.x - 5, screenPos.y - 5, Projectile_10_E, 10, 10);
				} else if (size > 6) {
					drawImage(screenPos.x - 4, screenPos.y - 4, Projectile_8_E, 8, 8);
				} else if (size > 4) {
					drawImage(screenPos.x - 3, screenPos.y - 3, Projectile_6_E, 6, 6);
				} else if (size > 3) {
					drawImage(screenPos.x - 2, screenPos.y - 5, Projectile_4_E, 4, 4);
				} else if (size > 2) {
					drawImage(screenPos.x - 1.5, screenPos.y - 1.5, Projectile_3_E, 3, 3);
				} else if (size > 1) {
					drawImage(screenPos.x - 1, screenPos.y - 1, Projectile_2_E, 2, 2);
				} else {
					drawPixel(screenPos.x, screenPos.y, ORANGE);
				}
			}
		}
	}
}
