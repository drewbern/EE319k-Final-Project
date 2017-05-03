#include "vec2f.h"
#include "vec3f.h"
#include "Matrix4f.h"
#include "stdint.h"
#include "GraphicsBuffer.h"
#include "Entity.h"

#define CUBE_HEALTH 4

const uint8_t CUBE_NUM_POINTS = 24;
const uint8_t CUBE_NUM_INDICES = 36;
const uint8_t PLANE_NUM_POINTS = 36;
const uint8_t PLANE_NUM_INDICES = 27;
const uint8_t ENEMY_NUM_POINTS = 18;
const uint8_t ENEMY_NUM_INDICES = 24;


//CUBE INFO
const float CUBE_POINTS[] = {
	-0.5,1,0.5,											//LEFT TOP BACK
	0.5,1,0.5,											//RIGHT TOP BACK
	-0.5,0,0.5,											//LEFT BOTTOM BACK
	0.5,0,0.5,											//RIGHT BOTTOM BACK

	-0.5,1,-0.5,										//LEFT TOP FRONT
	0.5,1,-0.5,											//RIGHT TOP FRONT
	-0.5,0,-0.5,											//LEFT BOTTOM FRONT
	0.5,0,-0.5											//RIGHT BOTTOM FRONT
};

const uint8_t CUBE_INDICES[] = {
	0, 1, 2,
	1, 2, 3,
	4, 5, 6,
	5, 6, 7,
	0, 4, 6,
	0, 6, 2,
	1, 5, 7,
	1, 7, 3,
	0, 4, 5,
	0, 1, 5,
	2, 6, 7,
	2, 3, 7
};

const uint8_t CUBE_COLOR_BUFFER[] = {
	WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE
};

//PLANE INFO
#define PLANE_PADDING	1.5						//Make a lil bigger to make the hitbox more forgiving 
const float PLANE_POINTS[] = {
	0 * PLANE_PADDING,			0.29 * PLANE_PADDING,		1 * PLANE_PADDING,							//Nose											0
	0 * PLANE_PADDING,			0.76 * PLANE_PADDING,		0 * PLANE_PADDING,							//Top fin										1
	-(0.2 * PLANE_PADDING),	0.29 * PLANE_PADDING,		-(0.5 * PLANE_PADDING),					//Left Fuselage							2
	0.2 * PLANE_PADDING,		0.29 * PLANE_PADDING,		-(0.5 * PLANE_PADDING),					//Right Fuselage						3
	-(1 * PLANE_PADDING),		0.29 * PLANE_PADDING,		-(0.5 * PLANE_PADDING),					//Left wing tip							4
	-(0.27 * PLANE_PADDING),0.29 * PLANE_PADDING,		-(1 * PLANE_PADDING),						//Left wing backstay				5
	1 * PLANE_PADDING,			0.29 * PLANE_PADDING,		-(0.5 * PLANE_PADDING),					//Right wing tip						6
	0.27 * PLANE_PADDING,		0.29 * PLANE_PADDING,		-(1 * PLANE_PADDING),						//Right wing backstay				7
	-(0.5 * PLANE_PADDING),	1 * PLANE_PADDING,			-(0.5 * PLANE_PADDING),					//Left Top Fin							8
	0.5 * PLANE_PADDING,		1 * PLANE_PADDING,			-(0.5 * PLANE_PADDING),					//Right Top Fin							9
	-(0.23 * PLANE_PADDING),0 * PLANE_PADDING,			-(0.5 * PLANE_PADDING),					//Left Bottom Fin						10
	0.23 * PLANE_PADDING,		0 * PLANE_PADDING,			-(0.5 * PLANE_PADDING)					//Right Bottom Fin					11
};

const uint8_t PLANE_INDICES[] = {
	0, 1, 2,								//Left Fuselage
	0, 1, 3,								//Right Fuselage
	1, 2, 3,								//Back of plane
	2, 4, 5,								//Left wing
	3, 6, 7,								//Right wing
	2, 5, 8,								//Left Top fin
	3, 7, 9,								//Right Top fin
	2, 5, 10,							//Left Bottom fin
	3, 7, 11								//Right Bottom fin
};

const uint8_t PLANE_COLOR_BUFFER[] = {
	GREY, GREY, GREY, GREY, GREY, BLUE, BLUE, BLUE, BLUE
};

const float ENEMY_POINTS[] = {
	0,0.5,0,											//Top point			0
	0.5,0,0,											//Right point		1
	0,0,0.5,											//Front point		2
	0,-0.5,0,											//Bottom point	3
	-0.5,0,0,											//Left point		4
	0,0,-0.5,											//Back point		5
};

const uint8_t ENEMY_INDICES[] = {
	0,1,2,			//TRF
	0,2,4,			//TFL
	0,4,5,			//TLB
	0,5,1,			//TBR
	
	3,1,2,			//BRF
	3,2,4,			//BFL
	3,4,5,			//BLB
	3,5,1,			//BBR	
};

const uint8_t ENEMY_COLOR_BUFFER[] = {
	ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE
};

Entity newCube(Vector3f position, float p, float y, float r, Vector3f scale) {
	Entity out = {
		position,
		CUBE, 
		{newVector2f(0,0), newVector2f(0,0), newVector2f(0,0), newVector2f(0,0),
		newVector2f(0,0), newVector2f(0,0), newVector2f(0,0), newVector2f(0,0)},
		p, y, r,
		CUBE_HEALTH,
		0, 0, 0,
		scale};
		return out;
}

Entity newPlane(Vector3f position, float p, float y, float r, Vector3f scale) {
	Entity out = {
		position,
		PLANE,
		{newVector2f(0,0), newVector2f(0,0), newVector2f(0,0), newVector2f(0,0),
		newVector2f(0,0), newVector2f(0,0), newVector2f(0,0), newVector2f(0,0)},
		p, y, r,
		3,	//Change to constructor later
		0, 0, 0,
		scale};
		return out;
}

Entity newEnemyEntity(Vector3f position, float p, float y, float r, Vector3f scale) {
	Entity out = {
		position,
		ENEMY,
		{newVector2f(0,0), newVector2f(0,0), newVector2f(0,0), newVector2f(0,0),
		newVector2f(0,0), newVector2f(0,0), newVector2f(0,0), newVector2f(0,0)},
		p, y, r,
		1,	//Change to constructor later
		0, 0, 0,
		scale};
		return out;
}
