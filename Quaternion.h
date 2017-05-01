#ifndef Quat_h
#define Quat_h

#include "Math2.h"
#include "math2.h"
#include "vec3f.h"
#include "VectorMath.h"

typedef struct Quaternion {
	float x;
	float y;
	float z;
	float w;
	Vector3f v;	
} Quaternion;

Quaternion newQuat(Vector3f n, float a);

Quaternion mul_Quat(Quaternion p, Quaternion q);

Vector3f mul_vec3f_quat(Vector3f V, Quaternion p);

Quaternion invertQuaternion(Quaternion q);

Matrix4f convertToTransformationMatrix(Quaternion q);

#endif
