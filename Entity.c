#include "vec2f.h"
#include "vec3f.h"
#include "Matrix4f.h"
#include "stdint.h"
#include "GraphicsBuffer.h"

typedef struct Entity {
	Vector3f position;
	float points[50];
	int numPoints;
	int indexBuffer[50];
	int numIndices;
	Vector2f vertexBuffer[16];
	uint8_t colorBuffer[15];
	float pitch;
	float yaw;
	float roll;
	Vector3f scale;
} Entity;

Entity newCube(Vector3f position, float p, float y, float r, Vector3f scale) {
	Entity out = {
		position,
		//Centered
		/*{-0.5f,0.5f,0.5,
		 0.5f,0.5f,0.5,
		 -0.5f,-0.5f,0.5,
			0.5f,-0.5f,0.5,
			
			-0.5f,0.5f,-0.5,
			0.5f,0.5f,-0.5,
			-0.5f,-0.5f,-0.5,
			0.5f,-0.5f,-0.5,
		},*/
		//Bottom of Cube
		{0,1,1,
		 1,1,1,
		 0,0,1,
		 1,0,1,
			
		 0,1,0,
		 1,1,0,
		 0,0,0,
		 1,0,0,
		},
		24,
		{0, 1, 2,
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
		},
		36,
		{newVector2f(0,0), newVector2f(0,0), newVector2f(0,0), newVector2f(0,0),
		newVector2f(0,0), newVector2f(0,0), newVector2f(0,0), newVector2f(0,0)},
		{WHITE},
		p, y, r,
		scale};
		return out;
}

Entity newPlane(Vector3f position, float p, float y, float r, Vector3f scale) {
	Entity out = {
		position,
		{
			0,0,2,								//Nose											0
			0,1,0,								//Top fin										1
			-0.6,0,-1,						//Left Fuselage							2
			0.6,0,-1,							//Right Fuselage						3
			-3,0,-1,							//Left wing tip							4
			-0.8,0,-2,						//Left wing backstay				5
			3,0,-1,								//Right wing tip						6
			0.8,0,-2,							//Right wing backstay				7
			-1.5,1.5,-1,					//Left Top Fin							8
			1.5,1.5,-1,						//Right Top Fin							9
			-0.7,-0.6,-1,					//Left Bottom Fin						10
			0.7,-0.6,-1						//Right Bottom Fin					11
			
		},
		36,
		{0, 1, 2,								//Left Fuselage
		 0, 1, 3,								//Right Fuselage
		 1, 2, 3,								//Back of plane
		 2, 4, 5,								//Left wing
		 3, 6, 7,								//Right wing
		 2, 5, 8,								//Left Top fin
		 3, 7, 9,								//Right Top fin
		 2, 5, 10,							//Left Bottom fin
		 3, 7, 11								//Right Bottom fin
		},
		27,
		{newVector2f(0,0), newVector2f(0,0), newVector2f(0,0), newVector2f(0,0),
		newVector2f(0,0), newVector2f(0,0), newVector2f(0,0), newVector2f(0,0)},
		{WHITE, WHITE, WHITE, WHITE, WHITE, BLUE, BLUE, BLUE, BLUE},
		p, y, r,
		scale};
		return out;
}
