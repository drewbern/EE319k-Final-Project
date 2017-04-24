#ifndef VectorMath_h
#define VectorMath_h

#include "vec3f.h"
#include "vec4f.h"
#include "Matrix3f.h"
#include "Matrix4f.h"

Vector3f scale_vec3f(Vector3f vector, float scale);

Vector4f scale_vec4f(Vector4f vector, float scale);

float dot_vec3f(Vector3f a, Vector3f b);

float dot_vec4f(Vector4f a, Vector4f b);

Vector3f cross_vec3f(Vector3f left, Vector3f right);

Vector3f add_vec3f(Vector3f a, Vector3f b);

Vector4f mul_vec3f_mat4f(Vector3f vector, Matrix4f matrix);

Vector3f mul_vec3f(Vector3f vector, Matrix3f matrix);

Vector4f mul_vec4f(Vector4f vector, Matrix4f matrix);

Matrix4f mul_mat4f_mat4f(Matrix4f left, Matrix4f right);

Matrix4f rotate_mat4f(float angle, Vector3f axis, Matrix4f src);

#endif
