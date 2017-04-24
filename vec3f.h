#ifndef Vec3f_h
#define Vec3f_h

#include "Matrix3f.h"

typedef struct Vector3f{
	float x;
	float y;
	float z;
} Vector3f;

Vector3f newVector3f(float x, float y, float z);

#endif
