#include "Math2.h"
#include "math2.h"
#include "vec3f.h"
#include "vec4f.h"
#include "Matrix3f.h"
#include "Matrix4f.h"
#include "VectorMath.h"

Vector3f scale_vec3f(Vector3f vector, float scale) {
	Vector3f out = {vector.x * scale, vector.y * scale, vector.z * scale};
	return out;
}

Vector4f scale_vec4f(Vector4f vector, float scale) {
	Vector4f out = {vector.x * scale, vector.y * scale, vector.z * scale, vector.w * scale};
	return out;
}

float dot_vec3f(Vector3f a, Vector3f b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float dot_vec4f(Vector4f a, Vector4f b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vector4f mul_vec3f_mat4f(Vector3f vector, Matrix4f matrix) {
	Vector4f a = {vector.x, vector.y, vector.z, 1.0};
	return mul_vec4f(a, matrix);
}

Vector3f mul_vec3f(Vector3f vector, Matrix3f matrix) {
	Vector3f out =  {vector.x * matrix.m00 + vector.y * matrix.m10 + vector.z * matrix.m20,
									 vector.x * matrix.m01 + vector.y * matrix.m11 + vector.z * matrix.m21,
									 vector.x * matrix.m02 + vector.y * matrix.m12 + vector.z * matrix.m22};
	return out;
}

Vector3f add_vec3f(Vector3f a, Vector3f b) {
	Vector3f out = {a.x + b.x, a.y + b.y, a.z + b.z};
	return out;
}

/**
Returns left sub right
**/
Vector3f sub_vec3f(Vector3f left, Vector3f right) {
	Vector3f out = {left.x - right.x, left.y - right.y, left.z - right.z};
	return out;
}

Vector3f normalize_vec3f(Vector3f v) {
	float length = length_vec3f(v);
	
	Vector3f out = {v.x/length, v.y/length, v.z/length};
	
	return out;
}

float length_vec3f(Vector3f v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3f cross_vec3f(Vector3f left, Vector3f right) {
	Vector3f out = {
				left.y * right.z - left.z * right.y,
				right.x * left.z - right.z * left.x,
				left.x * right.y - left.y * right.x};
	return out;
}

Vector4f mul_vec4f(Vector4f vector, Matrix4f matrix) {
	Vector4f out =  {vector.x * matrix.m00 + vector.y * matrix.m10 + vector.z * matrix.m20 + vector.w * matrix.m30,
									 vector.x * matrix.m01 + vector.y * matrix.m11 + vector.z * matrix.m21 + vector.w * matrix.m31,
									 vector.x * matrix.m02 + vector.y * matrix.m12 + vector.z * matrix.m22 + vector.w * matrix.m32,
									 vector.x * matrix.m03 + vector.y * matrix.m13 + vector.z * matrix.m23 + vector.w * matrix.m33};
	return out;
}

Matrix4f mul_mat4f_mat4f(Matrix4f left, Matrix4f right) {
	Matrix4f out = {
		left.m00 * right.m00 + left.m10 * right.m01 + left.m20 * right.m02 + left.m30 * right.m03,
		left.m01 * right.m00 + left.m11 * right.m01 + left.m21 * right.m02 + left.m31 * right.m03,
		left.m02 * right.m00 + left.m12 * right.m01 + left.m22 * right.m02 + left.m32 * right.m03,
		left.m03 * right.m00 + left.m13 * right.m01 + left.m23 * right.m02 + left.m33 * right.m03,
		left.m00 * right.m10 + left.m10 * right.m11 + left.m20 * right.m12 + left.m30 * right.m13,
		left.m01 * right.m10 + left.m11 * right.m11 + left.m21 * right.m12 + left.m31 * right.m13,
		left.m02 * right.m10 + left.m12 * right.m11 + left.m22 * right.m12 + left.m32 * right.m13,
		left.m03 * right.m10 + left.m13 * right.m11 + left.m23 * right.m12 + left.m33 * right.m13,
		left.m00 * right.m20 + left.m10 * right.m21 + left.m20 * right.m22 + left.m30 * right.m23,
		left.m01 * right.m20 + left.m11 * right.m21 + left.m21 * right.m22 + left.m31 * right.m23,
		left.m02 * right.m20 + left.m12 * right.m21 + left.m22 * right.m22 + left.m32 * right.m23,
		left.m03 * right.m20 + left.m13 * right.m21 + left.m23 * right.m22 + left.m33 * right.m23,
		left.m00 * right.m30 + left.m10 * right.m31 + left.m20 * right.m32 + left.m30 * right.m33,
		left.m01 * right.m30 + left.m11 * right.m31 + left.m21 * right.m32 + left.m31 * right.m33,
		left.m02 * right.m30 + left.m12 * right.m31 + left.m22 * right.m32 + left.m32 * right.m33,
		left.m03 * right.m30 + left.m13 * right.m31 + left.m23 * right.m32 + left.m33 * right.m33
	};
	
	return out;
}

Matrix4f createTransformationMatrix(Vector3f translation, Vector3f scale) {
		Matrix4f matrix = {1,0,0,0,
											 0,1,0,0,
											 0,0,1,0,
											 0,0,0,1};
		
		scale_mat4f(scale, &matrix);
		translate_mat4f(translation, &matrix);
		
		return matrix;
}

/**
Used help from
http://inside.mines.edu/fs_home/gmurray/ArbitraryAxisRotation/
(no code, just math algorithms)
**/
Matrix4f createRotationMatrix(float p, float y, float r) {
	float pitch = toRadians(p);
	float yaw = toRadians(y);
	float roll = toRadians(r);
	
	Matrix4f matrix = {
		cos(yaw)*cos(roll), cos(roll)*sin(pitch)*sin(yaw)-cos(pitch)*sin(roll), cos(pitch)*cos(roll)*sin(yaw)+sin(pitch)*sin(roll), 0,
		cos(yaw)*sin(roll), cos(pitch)*cos(roll)+sin(pitch)*sin(yaw)*sin(roll), cos(pitch)*sin(yaw)*sin(roll)-cos(roll)*sin(pitch), 0,
		-sin(yaw), cos(yaw)*sin(pitch), cos(pitch)*cos(yaw), 0,
		0, 0, 0, 1
	};
	
	return matrix;
}
