#ifndef Render_h
#define Render_h

#include "Math2.h"
#include "vec2f.h"
#include "vec3f.h"
#include "vec4f.h"
#include "Matrix3f.h"
#include "Matrix4f.h"
#include "Render.h"
#include "VectorMath.h"

#include "Camera.h"
#include "Entity.h"

void initRenderer(Camera* cameraP);

void render(Entity** entities, int numEntities, Camera c);

void prepareEntity(Entity* entityP);

void renderEntity(Vector2f vertexBuffer[], int indexBuffer[], int numTriangles);

void renderPlayer(Player player);

Vector2f preparePoint(Vector3f pointA, Matrix4f transformation, Matrix4f rotation);

Vector2f preparePointSimple(Vector3f pointA);

void renderPoint(Vector3f pointA, Matrix4f transformation);

void createViewMatrix(Camera camera);

void createProjectionMatrix(void);

Matrix4f createTransformationMatrix(Vector3f translation, Vector3f scale);

Vector3f mul(Vector3f vector, Matrix3f matrix);

Matrix4f axisToOrientation(Vector3f forward, Vector3f directionUp);

Matrix4f createRotationMatrix(float pitch, float yaw, float roll);

#endif
