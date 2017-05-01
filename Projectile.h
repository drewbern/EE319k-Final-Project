#ifndef Projectile_h
#define Projectile_h

#include "vec3f.h"
#include "Entity.h"

#define PROJECTILE_COLLECTION_SIZE 20


enum ProjectileType {
	PLAYER_PROJECTILE,
	ENEMY_PROJECTILE,
	ALL	
};


typedef struct Projectile {
	Vector3f position;
	Vector3f velocity;
	uint8_t live;														//If the projectile should be rendered, moved, etc. 
	enum ProjectileType type;
	//uint8_t type;
} Projectile;

typedef struct Projectile_Collection {
	Projectile projectiles[20];	
} Projectile_Collection;

uint8_t testCollision(Entity* entity, Projectile_Collection* pCollection, enum ProjectileType typeToTest);
//uint8_t testCollision(Entity* entity, Projectile_Collection* pCollection, uint8_t typeToTest);

void moveProjectiles(Projectile_Collection* pCollection);
	
Projectile_Collection newProjectileCollection(void);

Projectile newProjectile(Vector3f position, Vector3f velocity, enum ProjectileType type);
//Projectile newProjectile(Vector3f position, Vector3f velocity, uint8_t type);

void addProjectile(Projectile_Collection* pCollection, Projectile projectile);

void renderProjectiles(Projectile_Collection pCollection);

#endif
