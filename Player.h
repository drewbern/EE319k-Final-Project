#ifndef Player_h
#define Player_h

#include "vec3f.h"
#include "Entity.h"
#include "Projectile.h"

typedef struct Player {
	Vector3f position;
	Entity entity;

	uint8_t health;
	uint8_t numBombs;
	
	float pitch;
	float yaw;
	float roll;
	
	float reloadTime;
	float reloadCounter;
	int bombReloadCounter;
} Player;

Player newPlayer(void);

void movePlayer(Player* p, Projectile_Collection* pCollection);

void shoot(Player* p, Projectile_Collection* pCollection);

uint8_t isDoingBarrelRoll();
	
#endif
