#include <math.h>
#include "ST7735.h"

#include "Math2.h"
#include "vec2f.h"
#include "vec3f.h"
#include "vec4f.h"
#include "Matrix3f.h"
#include "Matrix4f.h"
#include "Render.h"
#include "VectorMath.h"
#include "Quaternion.h"

#include "Camera.h"
#include "Entity.h"

#include "GraphicsBuffer.h"

#define WIDTH					128
#define HEIGHT				160
#define HALF_WIDTH		64
#define HALF_HEIGHT		80
#define FOV						90					//Angle of FOV, in degrees
#define NEAR_PLANE		0.1
#define FAR_PLANE			1000

Matrix4f viewMatrix = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	}; 
Matrix4f projectionMatrix = {0, 0, 0, 0, 0, 0, 0, 0, 0};

Camera* camera;

void initRenderer(Camera* cameraP) {
	createProjectionMatrix();
	camera = cameraP;
}

void prepareRenderer(Camera c) {
	createViewMatrix(*camera);
}

void render(Entity** entitiesP, int numElements) {
	Entity* entities = *entitiesP;
	
	for(int i = 0; i < numElements; i ++) {
		Entity entity = *(entities + i);
		if(entity.type == PLANE) {
			prepareEntity(&entity, PLANE_POINTS);
			renderEntity(entity.vertexBuffer, PLANE_INDICES, PLANE_NUM_INDICES, PLANE_COLOR_BUFFER);	
		} else if (entity.type == CUBE) {
			prepareEntity(&entity, CUBE_POINTS);
			renderEntity(entity.vertexBuffer, CUBE_INDICES, CUBE_NUM_INDICES, CUBE_COLOR_BUFFER);	
		}
	}
}

void prepareEntity(Entity* entityP, const float points[]) {
	Matrix4f transformationMatrix = createTransformationMatrix((*entityP).position, (*entityP).scale);
	
	Matrix4f rotation = createRotationMatrix((*entityP).pitch, (*entityP).yaw, (*entityP).roll);
	
	uint8_t numPoints = 0;
	if((*entityP).type == PLANE) {
		numPoints = PLANE_NUM_POINTS;
	} else if ((*entityP).type == CUBE) {
		numPoints = CUBE_NUM_POINTS;
	}
	
	for(int i = 0; i < numPoints-2; i +=3) {
		Vector2f* vertexBufferPointer = &((*entityP).vertexBuffer[i/3]);
		Vector3f entityPoint = {points[i], points[i+1], points[i+2]};
		Vector2f point = preparePoint(entityPoint, transformationMatrix, rotation);
		*vertexBufferPointer = point;
	}		
}

void renderEntity(Vector2f vertexBuffer[], const uint8_t indexBuffer[], int numIndices, const uint8_t colorBuffer[]) {
	for(int i = 0;  i < numIndices-2; i +=3) {
		int16_t x0 = (int16_t)vertexBuffer[indexBuffer[i]].x;
		int16_t y0 = (int16_t)vertexBuffer[indexBuffer[i]].y;
		int16_t x1 = (int16_t)vertexBuffer[indexBuffer[i+1]].x;
		int16_t y1 = (int16_t)vertexBuffer[indexBuffer[i+1]].y;
		int16_t x2 = (int16_t)vertexBuffer[indexBuffer[i+2]].x;
		int16_t y2 = (int16_t)vertexBuffer[indexBuffer[i+2]].y;

		uint8_t color = *(colorBuffer+i/3);
		drawLine(x0, y0, x1, y1, color);
		drawLine(x1, y1, x2, y2, color);
		drawLine(x2, y2, x0, y0, color);
	}
}

void renderPlayer(Player player) {
	Matrix4f transformationMatrix = createTransformationMatrix(player.entity.position, player.entity.scale);
	
	Matrix4f rotation = createRotationMatrix(player.pitch, player.yaw, player.roll);
	
	Vector3f frontOfPlayer =  {0, 0, 8};
	Vector2f reticuleCenter = preparePoint(frontOfPlayer, transformationMatrix, rotation);
	
	drawLine(reticuleCenter.x - 5, reticuleCenter.y - 7, reticuleCenter.x + 5, reticuleCenter.y - 7, RED);
	drawLine(reticuleCenter.x - 5, reticuleCenter.y + 7, reticuleCenter.x + 5, reticuleCenter.y + 7, RED);
	drawLine(reticuleCenter.x - 7, reticuleCenter.y + 5, reticuleCenter.x - 7, reticuleCenter.y - 5, RED);
	drawLine(reticuleCenter.x + 7, reticuleCenter.y + 5, reticuleCenter.x + 7, reticuleCenter.y - 5, RED);
	
	drawLine(reticuleCenter.x - 4, reticuleCenter.y - 4, reticuleCenter.x - 8, reticuleCenter.y - 8, RED);
	drawLine(reticuleCenter.x + 4, reticuleCenter.y - 4, reticuleCenter.x + 8, reticuleCenter.y - 8, RED);
	drawLine(reticuleCenter.x - 4, reticuleCenter.y + 4, reticuleCenter.x - 8, reticuleCenter.y + 8, RED);
	drawLine(reticuleCenter.x + 4, reticuleCenter.y + 4, reticuleCenter.x + 8, reticuleCenter.y + 8, RED);
	
	frontOfPlayer =  newVector3f(0, 0, 20);
	reticuleCenter = preparePoint(frontOfPlayer, transformationMatrix, rotation);
	
	drawLine(reticuleCenter.x - 2, reticuleCenter.y - 2, reticuleCenter.x + 2, reticuleCenter.y - 2, RED);
	drawLine(reticuleCenter.x - 2, reticuleCenter.y + 2, reticuleCenter.x + 2, reticuleCenter.y + 2, RED);
	drawLine(reticuleCenter.x - 2, reticuleCenter.y + 2, reticuleCenter.x - 2, reticuleCenter.y - 2, RED);
	drawLine(reticuleCenter.x + 2, reticuleCenter.y + 2, reticuleCenter.x + 2, reticuleCenter.y - 2, RED);
	
	

	for(int i = 0; i < PLANE_NUM_POINTS-2; i +=3) {
		Vector2f* vertexBufferPointer = &((player.entity).vertexBuffer[i/3]);
		Vector3f entityPoint = {PLANE_POINTS[i], PLANE_POINTS[i+1], PLANE_POINTS[i+2]};
		Vector2f point = preparePoint(entityPoint, transformationMatrix, rotation);
		*vertexBufferPointer = point;
	}
	
	renderEntity(player.entity.vertexBuffer, PLANE_INDICES, PLANE_NUM_INDICES, PLANE_COLOR_BUFFER);
}

Vector2f preparePoint(Vector3f pointA, Matrix4f transformation, Matrix4f rotation) {
	
	Vector4f worldPosition = mul_vec3f_mat4f(pointA, rotation);
	worldPosition = mul_vec4f(worldPosition, transformation);
	Vector4f positionRelativeToCam = mul_vec4f(worldPosition, viewMatrix);
	Vector4f point3D = mul_vec4f(positionRelativeToCam, projectionMatrix);
	
	float winX = (( point3D.x * WIDTH ) / (2.0 * point3D.w)) + HALF_WIDTH;
  float winY = (( point3D.y * HEIGHT ) / (2.0 * point3D.w)) + HALF_HEIGHT;
		
	Vector2f point2D = {winX, winY};
	return point2D;
	
}
/**
Use this for static points in the global context
**/
Vector2f preparePointSimple(Vector3f pointA) {
	
	Vector4f positionRelativeToCam = mul_vec3f_mat4f(pointA, viewMatrix);
	Vector4f point3D = mul_vec4f(positionRelativeToCam, projectionMatrix);
		
	
	float winX = (( point3D.x * WIDTH ) / (2.0 * point3D.w)) + HALF_WIDTH;
  float winY = (( point3D.y * HEIGHT ) / (2.0 * point3D.w)) + HALF_HEIGHT;
	
	Vector2f point2D = {winX, winY};
	return point2D;
}

void createViewMatrix(Camera camera) {
	viewMatrix = createRotationMatrix(camera.pitch, camera.yaw, camera.roll);
	Vector3f negativeCameraPos = {-camera.position.x, -camera.position.y, -camera.position.z};
	translate_mat4f(negativeCameraPos, &viewMatrix);
}

void createProjectionMatrix(void) {		 
														 
	float aspectRatio = (float) WIDTH / (float) HEIGHT;
	float y_scale = (float) (1.0 / tan(toRadians(FOV / 2.0)) * aspectRatio);
	float x_scale = y_scale / aspectRatio;
	float frustum_length = FAR_PLANE - NEAR_PLANE;
	
	Matrix4f setMatrix = {x_scale, 	0, 				0,																								0,
												0, 				y_scale, 	0,																								0,
												0, 				0,				((FAR_PLANE + NEAR_PLANE) / frustum_length),			1,
												0,				0,				((2 * FAR_PLANE * NEAR_PLANE) / frustum_length),	0};
	projectionMatrix = setMatrix;
	
}
