#ifndef Camera_h
#define Camera_h

#include "vec3f.h"
#include "Player.h"

typedef struct Camera {
	Vector3f position;
	float pitch;
	float yaw;
	float roll;
	Player player;
} Camera;

void moveCamera(Camera* c);

Camera newCamera(Player* p);

void rotateCamera(Camera* camera);

#endif
