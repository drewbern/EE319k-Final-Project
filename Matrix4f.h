#ifndef Matrix4f_h
#define Matrix4f_h

#include "vec3f.h"

typedef struct Matrix4f{
	float m00, m10, m20, m30;						//Top row, left to right
	float m01, m11, m21, m31;
	float m02, m12, m22, m32;
	float m03, m13, m23, m33;	//Bottom row
	
} Matrix4f;

void translate_mat4f(Vector3f vec, Matrix4f* srcP);

void scale_mat4f(Vector3f vec, Matrix4f* srcP);

Matrix4f newMat4f(void);

#endif
