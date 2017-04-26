#include <math.h>
#include "tm4c123gh6pm.h"

#include "vec3f.h"
#include "Entity.h"
#include "InputHandler.h"
#include "vec4f.h"
#include "VectorMath.h"
#include "Matrix4f.h"

#include "Projectile.h"

#define MAX_DISPLACEMENT 1						//How far the player can move from the middle
#define MAX_HEIGHT 1										//How far up player can move from ground
#define MIN_HEIGHT 0.05
#define MAX_ROLL 30											//Maximum roll player will go while turning (in degrees)
#define MAX_PITCH 13										//Maximum pitch player will do while going up or down

#define MOVE_SPEED 0.09									
#define ROLL_SPEED 5										//Degrees per frame
#define PITCH_SPEED 3										//Degrees per frame

//Can roll factor just be max roll / max input
#define ROLL_FACTOR -2200								//Converting movement input to how much roll should occur
	
typedef struct Player {
	Vector3f position;
	Entity entity;

	float health;
	float numBombs;
	
	float pitch;
	float yaw;
	float roll;
} Player;

Player newPlayer(void) {
	Vector3f initialPosition = {0, 5, 4.5};
	Player out = {
		initialPosition,
		newPlane(initialPosition, 0, 0, 0, newVector3f(0.3,0.3,0.3)),
		3,
		0,
		0,
		0,
		0
	};
	
	return out;
}

void movePlayer(Player* p, Projectile_Collection* pCollection) {
	
	//Horizontal Movement
	float horizontalMovement = getXPos() * MOVE_SPEED;
	
	(*p).position.x += horizontalMovement;
	(*p).position.x = fmin(fmax((*p).position.x, -MAX_DISPLACEMENT), MAX_DISPLACEMENT);				// clamps position

	
	
	float targetRoll = horizontalMovement * ROLL_FACTOR;
	
	float deltaRoll = (targetRoll - (*p).roll) * fabs(getXPos());
	if(deltaRoll > 0) {
		(*p).roll += fmin(deltaRoll, ROLL_SPEED);
	} else if (deltaRoll < 0) {
		(*p).roll += fmax(deltaRoll, -ROLL_SPEED);
	}
	(*p).roll = fmin(fmax((*p).roll, -MAX_ROLL), MAX_ROLL);	

	//Vertical Movement
	float verticalMovement = getYPos() * MOVE_SPEED;
	
	(*p).position.y += verticalMovement;
	(*p).position.y = fmin(fmax((*p).position.y, MIN_HEIGHT), MAX_HEIGHT);				// clamps position

	
	float targetPitch = verticalMovement * ROLL_FACTOR;
	if((*p).position.y <= MIN_HEIGHT+0.1 || (*p).position.y >= MAX_HEIGHT) {
		targetPitch = 0;
	}
	
	
	float deltaPitch = (targetPitch - (*p).pitch) * fabs(getYPos());
	if(deltaPitch > 0) {
		(*p).pitch += fmin(deltaPitch, PITCH_SPEED);
	} else if (deltaPitch < 0) {
		(*p).pitch += fmax(deltaPitch, -PITCH_SPEED);
	}
	(*p).pitch = fmin(fmax((*p).pitch, -MAX_PITCH), MAX_PITCH);			
	
	//Testing
	//(*p).position.y += 0.1;
	
	(*p).entity.position = (*p).position;
	(*p).entity.pitch = (*p).pitch;
	(*p).entity.roll = (*p).roll;
}

void shoot(Player* p, Projectile_Collection* pCollection) {
	if(((*pCollection).putIndex + 1) % PROJECTILE_COLLECTION_SIZE != (*pCollection).getIndex) {
		Vector4f velocity = {0, 0, 0.5, 1};
		Matrix4f rotate = createRotationMatrix((*p).pitch, (*p).yaw, (*p).roll);
		
		velocity = mul_vec4f(velocity, rotate);
		
		(*pCollection).projectiles[(*pCollection).putIndex] = newProjectile((*p).position, newVector3f(velocity.x, velocity.y,
			velocity.z));
	}
}
