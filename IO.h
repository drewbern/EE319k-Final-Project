// IO.h
// Program written by: Zachary Chilton & Philip Tan
// Last Modified: 04/28/2017
// Lab number: 6

#include "tm4c123gh6pm.h"
#include <stdint.h>

//basic IO
void IOInit(void);
void beat(void);

//ADC
void ADCInit(void);
uint32_t ADC0In(void);
uint32_t ADC1In(void);

//ADC -> game input
void gatherInputs(void);
float getXPos(void);
float getYPos(void);
