#include "vec3f.h"
#include "stdint.h"
#include "Render.h"
#include "GraphicsBuffer.h"

#define CAMERA_Z 2.5				//The focal point

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

void addProjectile(Projectile projectile) {
	
}

/*
void testCollision(Entity entitiesToTest[], Projectile projctiles[]) {
	
}
*/
void renderProjectiles(Projectile_Collection pCollection) {
	for(int i = 0; i < (pCollection.getIndex-pCollection.putIndex)	% 20; i ++) {
		float size = 10 / (pCollection.projectiles[pCollection.getIndex].position.z - CAMERA_Z);
		
		Vector2f screenPos = preparePointSimple(pCollection.projectiles[i].position);
		
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
	}
	
}
