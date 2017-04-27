#include <stdint.h>
#include "ADC.h"

float xPos;
float yPos;

void gatherInputs(void) {
	yPos = (((float)ADC0_In())-2048)/2048;												//Percentage how far pushed away from center
	xPos = (((float)ADC1_In())-2048)/2048;												//Percentage how far pushed away from center
	
}

float getXPos(void) {
	return xPos;
}

float getYPos(void) {
	return yPos;																		
}
