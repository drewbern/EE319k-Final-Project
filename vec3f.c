#include "Matrix3f.h"

typedef struct Vector3f{
	float x;
	float y;
	float z;
} Vector3f;

Vector3f newVector3f(float x, float y, float z) {
	Vector3f out = {x,y,z};
	return out;
}
