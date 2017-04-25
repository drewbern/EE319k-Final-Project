#ifndef Entity_h
#define Entity_h

#include "vec2f.h"
#include "vec3f.h"
#include "Matrix4f.h"
#include "stdint.h"

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

Entity newCube(Vector3f position, float p, float y, float r, Vector3f scale);

Entity newPlane(Vector3f position, float p, float y, float r, Vector3f scale);

#endif
