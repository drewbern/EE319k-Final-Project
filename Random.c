#include "tm4c123gh6pm.h"

/**
Returns a random integer from 0 to range
**/
int  randomInt(int range) {
	return (NVIC_ST_CURRENT_R)%range;
}
