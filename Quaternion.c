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

Quaternion newQuat(Vector3f n, float a) {
	Vector3f v = n;
	float aRad = toRadians(a);
		
	float w = cos(aRad/2);
	float x = (n.x* sin(aRad/2));
	float y = (n.y* sin(aRad/2));
	float z = (n.z* sin(aRad/2));
	v = scale_vec3f(v, sin(aRad/2));
	
	Quaternion out = {x, y, z, w, v};
	return out;
}

Quaternion mul_Quat(Quaternion p, Quaternion q) {
		Quaternion r = newQuat(newVector3f(0,0,0), 0);
		
		r.w = p.w*q.w - dot_vec3f(q.v, p.v);
		
		r.v = newVector3f(p.v.x*q.w, p.v.y*q.w, p.v.z*q.w);
		r.v = add_vec3f(r.v, newVector3f(q.v.x*p.w, q.v.y*p.w, q.v.z*p.w));
		r.v = add_vec3f(r.v, cross_vec3f(p.v, q.v));
		r.x = r.v.x;
		r.y = r.v.y;
		r.z = r.v.z;
		return r;
}

Vector3f mul_vec3f_quat(Vector3f V, Quaternion p) {
		Vector3f vcV = cross_vec3f(p.v, V);
		
		Vector3f out = add_vec3f(V, newVector3f(vcV.x*2*p.w, vcV.y*2*p.w, vcV.z*2*p.w));
		vcV = cross_vec3f(p.v, vcV);
		
		out = add_vec3f(out, newVector3f(vcV.x*2, vcV.y*2, vcV.z*2));
		
		return out;
}

Quaternion invertQuaternion(Quaternion q) {
		Quaternion out = {-q.x, -q.y, -q.z, q.w, q.v};
		
		return out;
}

Matrix4f convertToTransformationMatrix(Quaternion q) {
	Matrix4f out = {0, 0, 0, 0,
									0, 0, 0, 0,
									0, 0, 0, 0,
									0, 0, 0, 0};
	float qw = q.w;
	float qx = q.x;
	float qy = q.y;
	float qz = q.z;
	
	
	out.m00 = 1.0f - 2.0f*qy*qy - 2.0f*qz*qz;
	out.m01 = 2.0f*qx*qy - 2.0f*qz*qw;
	out.m02 = 2.0f*qx*qz + 2.0f*qy*qw;
	out.m03 = 0.0f;
	
	out.m10 = 2.0f*qx*qy + 2.0f*qz*qw;
	out.m11 = 1.0f - 2.0f*qx*qx - 2.0f*qz*qz;
	out.m12 = 2.0f*qy*qz - 2.0f*qx*qw;
	out.m13 = 0.0f;
	
	out.m20 = 2.0f*qx*qz - 2.0f*qy*qw;
	out.m21 = 2.0f*qy*qz + 2.0f*qx*qw;
	out.m22 = 1.0f - 2.0f*qx*qx - 2.0f*qy*qy;
	out.m23 = 0.0f;
	
	out.m30 = 0.0f;
	out.m31 = 0.0f;
	out.m32 = 0.0f;
	out.m33 = 1.0f;
    
	return out;
}
