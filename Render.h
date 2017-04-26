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

void renderEntity(Vector2f vertexBuffer[], int indexBuffer[], int numTriangles, uint8_t colorBuffer[]);

void renderPlayer(Player player);

Vector2f preparePoint(Vector3f pointA, Matrix4f transformation, Matrix4f rotation);

Vector2f preparePointSimple(Vector3f pointA);

void renderPoint(Vector3f pointA, Matrix4f transformation);

void createViewMatrix(Camera camera);

void createProjectionMatrix(void);

#endif
