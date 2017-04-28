#ifndef Enemy_h
#define Enemy_h

#include "stdint.h"
#include "vec3f.h"

enum enemyType {
	BASIC
};

typedef struct Enemy{
	enum enemyType type;
	Vector3f position;
	float size;
	//float health;
	
} Enemy;

typedef struct Enemy_Collection {
	Enemy projectiles[20];
	uint8_t putIndex;
	uint8_t getIndex;
	
} Enemy_Collection;

#endif