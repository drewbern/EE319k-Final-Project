#include "vec3f.h"
#include "VectorMath.h"
#include "stdint.h"
#include "Render.h"
#include "GraphicsBuffer.h"

#define CAMERA_Z -6.3				//The focal point
#define MAX_PROJECTILE_DISTANCE 10

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

Projectile_Collection newProjectileCollection() {
	Projectile_Collection out = {
		{{newVector3f(0,0,0),newVector3f(0,0,0)}}, 0 , 0
	};
	
	return out;
}

Projectile newProjectile(Vector3f position, Vector3f velocity) {
	Projectile out = {position, velocity};
	return out;
}

void addProjectile(Projectile_Collection* pCollection, Projectile projectile) {
	if(((*pCollection).putIndex + 1) % 20 != (*pCollection).getIndex ) {
			(*pCollection).projectiles[(*pCollection).putIndex] = projectile;
			(*pCollection).putIndex = ((*pCollection).putIndex + 1) % 20;
	}
}

void removeProjectile(Projectile_Collection* pCollection, int index) {
	if((*pCollection).putIndex != (*pCollection).getIndex ) {
			(*pCollection).getIndex = ((*pCollection).getIndex + 1)%20;
	}
}

void moveProjectiles(Projectile_Collection* pCollection) {
	uint8_t index = (*pCollection).getIndex;
	uint8_t numToRemove = 0;
	uint8_t projToRemove[20];
	
	while ((index) != (*pCollection).putIndex) {
		if((*pCollection).projectiles[index].position.z >= MAX_PROJECTILE_DISTANCE ||
					(*pCollection).projectiles[index].position.y <= 0) {
			projToRemove[numToRemove] = index;
			numToRemove ++;			
		}
		
		(*pCollection).projectiles[index].position = add_vec3f((*pCollection).projectiles[index].position,
			(*pCollection).projectiles[index].velocity);
		index = (index+1)%20;
	}
	
	for(int i = 0; i < numToRemove; i ++) {
		removeProjectile(pCollection, projToRemove[i]);
	}
}

/*
void testCollision(Entity entitiesToTest[], Projectile projctiles[]) {
	
}
*/
void renderProjectiles(Projectile_Collection pCollection) {
	uint8_t index = (pCollection).getIndex;
	
	while ((index) != pCollection.putIndex) {
		float size = 10 * -CAMERA_Z / (pCollection.projectiles[index].position.z - CAMERA_Z);
		
		Vector2f screenPos = preparePointSimple(pCollection.projectiles[index].position);
		
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
			
		index = (index+1)%20;	
	}
}
