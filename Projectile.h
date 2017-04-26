#ifndef Projectile_h
#define Projectile_h

#include "vec3f.h"
#include "Entity.h"

#define PROJECTILE_COLLECTION_SIZE 20

typedef struct Projectile {
	Vector3f position;
	Vector3f velocity;
} Projectile;

typedef struct Projectile_Collection {
	Projectile projectiles[20];
	int8_t putIndex;
	int8_t getIndex;
	
} Projectile_Collection;

void testCollision(Entity entitiesToTest[], Projectile projctiles[]);

Projectile_Collection newProjectileCollection(void);

Projectile newProjectile(Vector3f position, Vector3f velocity);

void addProjectile(Projectile_Collection* pCollection, Projectile projectile);

void renderProjectiles(Projectile_Collection pCollection);

#endif
