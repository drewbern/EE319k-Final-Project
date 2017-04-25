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

void render(Entity** entitiesP, int numEntities, Camera c) {
	Entity* entities = *entitiesP;
		
	createViewMatrix(*camera);
	for(int i = 0; i < numEntities; i ++) {
		Entity entity = *(entities + i);
		prepareEntity(&entity);
		renderEntity(entity.vertexBuffer, entity.indexBuffer, entity.numIndices);
	}
}

void prepareEntity(Entity* entityP) {
	Matrix4f transformationMatrix = createTransformationMatrix((*entityP).position, (*entityP).scale);
	
	Matrix4f rotation = createRotationMatrix((*entityP).pitch, (*entityP).yaw, (*entityP).roll);
	
	for(int i = 0; i < (*entityP).numPoints-2; i +=3) {
		Vector2f* vertexBufferPointer = &((*entityP).vertexBuffer[i/3]);
		Vector3f entityPoint = {(*entityP).points[i], (*entityP).points[i+1], (*entityP).points[i+2]};
		Vector2f point = preparePoint(entityPoint, transformationMatrix, rotation);
		*vertexBufferPointer = point;
	}		
}

void renderEntity(Vector2f vertexBuffer[], int indexBuffer[], int numIndices) {
	for(int i = 0;  i < numIndices-2; i +=3) {
		int16_t x0 = (int16_t)vertexBuffer[indexBuffer[i]].x;
		int16_t y0 = (int16_t)vertexBuffer[indexBuffer[i]].y;
		int16_t x1 = (int16_t)vertexBuffer[indexBuffer[i+1]].x;
		int16_t y1 = (int16_t)vertexBuffer[indexBuffer[i+1]].y;
		int16_t x2 = (int16_t)vertexBuffer[indexBuffer[i+2]].x;
		int16_t y2 = (int16_t)vertexBuffer[indexBuffer[i+2]].y;
		
		/*
		ST7735_DrawLine(x0, y0, x1, y1, 0xFFFF);
		ST7735_DrawLine(x1, y1, x2, y2, 0xFFFF);
		ST7735_DrawLine(x2, y2, x0, y0, 0xFFFF);
		*/
		
		drawLine(x0, y0, x1, y1, 0xFFFF);
		drawLine(x1, y1, x2, y2, 0xFFFF);
		drawLine(x2, y2, x0, y0, 0xFFFF);
	}
}

void renderPlayer(Player player) {
	Matrix4f transformationMatrix = createTransformationMatrix(player.entity.position, player.entity.scale);
	
	Matrix4f rotation = createRotationMatrix(player.pitch, player.yaw, player.roll);
	
	Vector3f frontOfPlayer =  {0, 0, 5};
	Vector2f reticuleCenter = preparePoint(frontOfPlayer, transformationMatrix, rotation);
	
	//ST7735_FillRect(reticuleCenter.x-5, reticuleCenter.y-5, 10, 10, ST7735_Color565(255, 0, 0));	
	
	for(int i = 0; i < (player.entity).numPoints-2; i +=3) {
		Vector2f* vertexBufferPointer = &((player.entity).vertexBuffer[i/3]);
		Vector3f entityPoint = {(player.entity).points[i], (player.entity).points[i+1], (player.entity).points[i+2]};
		Vector2f point = preparePoint(entityPoint, transformationMatrix, rotation);
		*vertexBufferPointer = point;
	}
	
	renderEntity(player.entity.vertexBuffer, player.entity.indexBuffer, player.entity.numIndices);
}

Vector2f preparePoint(Vector3f pointA, Matrix4f transformation, Matrix4f rotation) {
	
	Vector4f worldPosition = mul_vec3f_mat4f(pointA, rotation);
	worldPosition = mul_vec4f(worldPosition, transformation);
	Vector4f positionRelativeToCam = mul_vec4f(worldPosition, viewMatrix);
	Vector4f point3D = mul_vec4f(positionRelativeToCam, projectionMatrix);
	
	float winX = (( point3D.x * WIDTH ) / (2.0 * point3D.w)) + HALF_WIDTH;
  float winY = -(( point3D.y * HEIGHT ) / (2.0 * point3D.w)) + HALF_HEIGHT;
		
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
  float winY = -(( point3D.y * HEIGHT ) / (2.0 * point3D.w)) + HALF_HEIGHT;
	
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

Matrix4f createTransformationMatrix(Vector3f translation, Vector3f scale) {
		Matrix4f matrix = {1,0,0,0,
											 0,1,0,0,
											 0,0,1,0,
											 0,0,0,1};
		
		scale_mat4f(scale, &matrix);
		translate_mat4f(translation, &matrix);
		
		return matrix;
}

Matrix4f axisToOrientation(Vector3f directionSide, Vector3f directionUp){
	Vector3f forward = cross_vec3f(directionSide, directionUp);
	Matrix4f out = {
		
		directionSide.x, directionUp.x, forward.x, 0,
		directionSide.y, directionUp.y, forward.y, 0,
		directionSide.z, directionUp.z, forward.z, 0,
		0,								0,					 		0,				 1

	};
	
	return out;
}

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
