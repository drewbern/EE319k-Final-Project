#ifndef Entity_h
#define Entity_h

#include "vec2f.h"
#include "vec3f.h"
#include "Matrix4f.h"
#include "stdint.h"
#include "GraphicsBuffer.h"

extern const uint8_t CUBE_NUM_POINTS;
extern const uint8_t CUBE_NUM_INDICES;
extern const uint8_t PLANE_NUM_POINTS;
extern const uint8_t PLANE_NUM_INDICES;
extern const uint8_t ENEMY_NUM_POINTS;
extern const uint8_t ENEMY_NUM_INDICES;

//CUBE INFO
extern const float CUBE_POINTS[];

extern const uint8_t CUBE_INDICES[];

extern const uint8_t CUBE_COLOR_BUFFER[];

//PLANE INFO
extern const float PLANE_POINTS[];

extern const uint8_t PLANE_INDICES[];

extern const uint8_t PLANE_COLOR_BUFFER[];

//ENEMY INFO
extern const float ENEMY_POINTS[];

extern const uint8_t ENEMY_INDICES[];

extern const uint8_t ENEMY_COLOR_BUFFER[];

enum entityType {
	CUBE, PLANE, ENEMY
};

typedef struct Entity {
	Vector3f position;
	enum entityType type;
	Vector2f vertexBuffer[16];
	float pitch;
	float yaw;
	float roll;
	uint8_t health;
	uint8_t turnToRed;
	uint8_t framesRedLeft;
	Vector3f scale;
} Entity;

Entity newCube(Vector3f position, float p, float y, float r, Vector3f scale);

Entity newPlane(Vector3f position, float p, float y, float r, Vector3f scale);

Entity newEnemyEntity(Vector3f position, float p, float y, float r, Vector3f scale);

#endif
