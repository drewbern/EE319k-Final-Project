// IO.h
// Program written by: Zachary Chilton & Philip Tan
// Last Modified: 04/28/2017
// Lab number: 6

#include "tm4c123gh6pm.h"
#include <stdint.h>

//basic IO
void IO_Init(void);
void IO_HeartBeat(void);

//ADC
void ADC_Init(void);
uint32_t ADC0_In(void);
uint32_t ADC1_In(void);

//ADC -> game input
void gatherInputs(void);
float getXPos(void);
float getYPos(void);
