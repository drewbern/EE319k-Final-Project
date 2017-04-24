#ifndef Matrix3f_h
#define Matrix3f_h

typedef struct Matrix3f{
	float m00, m10, m20;							//Top row, left to right
	float m01, m11, m21;
	float m02, m12, m22;							//Bottom row
} Matrix3f;

#endif
