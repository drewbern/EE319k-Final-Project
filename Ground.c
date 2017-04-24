#include "Camera.h"
#include "Render.h"
#include "vec2f.h"
#include "vec3f.h"
#include "ST7735.h"

#define CAMERA_SPEED 0.1																				//Virtual speed of camera

#define LENGTH 3.0
#define HALF_LENGTH LENGTH/2
#define FORWARD_DISTANCE 2																				//Distance between each horizontal line
#define NUM_HORIZONTAL_LINES 4																		//Number of horizontal lines
#define HORIZON_DISTANCE FORWARD_DISTANCE * NUM_HORIZONTAL_LINES	//Distance of the horizon from camera

#define NUM_DEPTH_LINES 5
#define DEPTH_LINE_SPREAD LENGTH/NUM_DEPTH_LINES									

float firstLineDistance = FORWARD_DISTANCE; //The first horiztal line distance from camera

void renderGround(Camera camera) {
	float leftCoord = - HALF_LENGTH;
	float rightCoord = HALF_LENGTH;
	
	for(int i = 0; i < NUM_HORIZONTAL_LINES; i ++) {
		Vector2f leftPoint = preparePointSimple(newVector3f(leftCoord, 0, firstLineDistance + (i*FORWARD_DISTANCE)));
		Vector2f rightPoint = preparePointSimple(newVector3f(rightCoord, 0, firstLineDistance + (i*FORWARD_DISTANCE)));
		
		ST7735_DrawLine(leftPoint.x, leftPoint.y, rightPoint.x, rightPoint.y, ST7735_GREEN);
	}
	//Draw Horizon Line
	Vector2f leftPoint = preparePointSimple(newVector3f(leftCoord, 0, HORIZON_DISTANCE));
	Vector2f rightPoint = preparePointSimple(newVector3f(rightCoord, 0, HORIZON_DISTANCE));
	
	ST7735_DrawLine(leftPoint.x, leftPoint.y, rightPoint.x, rightPoint.y, ST7735_GREEN);
	
	firstLineDistance -= CAMERA_SPEED;
	if(firstLineDistance <= 0.11) {
		firstLineDistance = FORWARD_DISTANCE;								//If first line goes behind camera, shift all lines back
	}
	
	
	float farCoord = camera.position.z + HORIZON_DISTANCE;
	for(int i = 0; i <= NUM_DEPTH_LINES; i ++) {
		Vector2f nearPoint  = preparePointSimple(newVector3f(leftCoord + i*DEPTH_LINE_SPREAD, 0, 1));
		Vector2f farPoint  = preparePointSimple(newVector3f(leftCoord + i*DEPTH_LINE_SPREAD, 0, HORIZON_DISTANCE));
		
		ST7735_DrawLine(nearPoint.x, nearPoint.y, farPoint.x, farPoint.y, ST7735_GREEN);
	}
}
