#ifndef Player_h
#define Player_h

#include "vec3f.h"
#include "Entity.h"
#include "Projectile.h"


typedef struct Player {
	Vector3f position;
	Entity entity;

	float health;
	float numBombs;
	
	float pitch;
	float yaw;
	float roll;
} Player;

Player newPlayer(void);

void movePlayer(Player* p, Projectile_Collection* pCollection);

#endif
