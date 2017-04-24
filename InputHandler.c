#include <stdint.h>
#include "ADC.h"

float xPos;
float yPos;

void gatherInputs(void) {
	xPos = (((float)ADC_In())-2048)/2048;												//Percentage how far pushed away from center
	yPos = (((float)ADC_In())-2048)/2048;												//Percentage how far pushed away from center
}

float getXPos(void) {
	return xPos;
}

float getYPos(void) {
	return yPos;																		
}
