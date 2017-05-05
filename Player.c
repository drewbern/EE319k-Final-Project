// FiFo.h
// Last Modified: 04/28/2017
// Written by: Zachary Chilton & Philip Tan

#include "Math2.h"
#include "tm4c123gh6pm.h"

#include "vec3f.h"
#include "Entity.h"
#include "vec4f.h"
#include "VectorMath.h"
#include "Matrix4f.h"
#include "IO.h"
#include "UART.h"

#include "Projectile.h"
#include "Player.h"
#include "Sound.h"

#define MAX_DISPLACEMENT 2							//How far the player can move from the middle
#define MAX_HEIGHT 3										//How far up player can move from ground
#define MIN_HEIGHT 0.05
#define MAX_ROLL 30											//Maximum roll player will go while turning (in degrees)
#define MAX_PITCH 16										//Maximum pitch player will do while going up or down
#define MAX_YAW 35

#define MOVE_SPEED 0.30									
#define ROLL_SPEED 5										//Degrees per frame
#define PITCH_SPEED 3										//Degrees per frame
#define YAW_SPEED 0.001

#define PROJECTILE_SPEED 0.6
#define BOMB_RELOAD			300

//Can roll factor just be max roll / max input
#define ROLL_FACTOR -2200								//Converting movement input to how much roll should occur
#define YAW_FACTOR 50		

void takenDamage(uint8_t);

Player newPlayer(void) {
	Vector3f initialPosition = {0, 2, 2};
	Player out = {
		initialPosition,
		newPlane(initialPosition, 0, 0, 0, newVector3f(0.6,0.42,0.4)),
		
		3,
		2,
		
		0,
		0,
		0,
		
		5,
		0		
	};
	
	return out;
}

void movePlayer(Player* p, Projectile_Collection* pCollection) {
	//Other important stuff for player that is not movement
	(*p).reloadCounter = fmax((*p).reloadCounter - 1, 0);
	uint8_t hit = testCollision(&(*p).entity, pCollection, ENEMY_PROJECTILE);
	if(hit != 0) {
		(*p).entity.health -= hit;
		//Play sound here
	}
	(*p).bombReloadCounter = fmax((*p).bombReloadCounter - 1, 0);
	
	
	//Horizontal Movement
	float horizontalMovement = getXPos() * MOVE_SPEED;
	
	if(fabs(horizontalMovement)  > MOVE_SPEED*0.6) {
		(*p).position.x += horizontalMovement;
		(*p).position.x = fmin(fmax((*p).position.x, -MAX_DISPLACEMENT), MAX_DISPLACEMENT);				// clamps position
	}
	
	
	float targetRoll = horizontalMovement * ROLL_FACTOR;
	
	float deltaRoll = (targetRoll - (*p).roll) * fabs(getXPos());
	if(deltaRoll > 0) {
		(*p).roll += fmin(deltaRoll, ROLL_SPEED);
	} else if (deltaRoll < 0) {
		(*p).roll += fmax(deltaRoll, -ROLL_SPEED);
	}
	(*p).roll = fmin(fmax((*p).roll, -MAX_ROLL), MAX_ROLL);	
	
	float targetYaw = horizontalMovement * YAW_FACTOR;
	
	float deltaYaw = (targetYaw - (*p).yaw) * fabs(getXPos());
	if(deltaYaw < 0) {
		(*p).yaw += fmin(deltaYaw, YAW_SPEED);
	} else if (deltaRoll < 0) {
		(*p).yaw += fmax(deltaYaw, -YAW_SPEED);
	}
	(*p).yaw = fmin(fmax((*p).yaw, -MAX_YAW), MAX_YAW);

	//Vertical Movement
	float verticalMovement = getYPos() * MOVE_SPEED;
	
	if(fabs(verticalMovement) >= MOVE_SPEED*0.6) {
		(*p).position.y += verticalMovement;
		(*p).position.y = fmin(fmax((*p).position.y, MIN_HEIGHT), MAX_HEIGHT);				// clamps position
	}
	
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
		
	(*p).entity.position = (*p).position;
	(*p).entity.pitch = (*p).pitch;
	(*p).entity.roll = (*p).roll;
}

void shoot(Player* p, Projectile_Collection* pCollection) {
	if((*p).reloadCounter <= 0) {
		sound_laser();
		Vector4f velocity = {0, 0, PROJECTILE_SPEED, 1};
		Matrix4f rotate = createRotationMatrix((*p).pitch, (*p).yaw, (*p).roll);
		
		velocity = mul_vec4f(velocity, rotate);
		
		addProjectile(pCollection, newProjectile((*p).position, newVector3f(velocity.x, velocity.y, velocity.z), PLAYER_PROJECTILE));
		
		(*p).reloadCounter = (*p).reloadTime;
	}
}
