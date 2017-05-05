// FiFo.c
// Written by: Philip Tan & Zachary Chilton
// Last Modified: 04/28/2017
// Allows for a UART UI in which the FiFo will likely be full of UI data

#include <stdint.h>
#include "Menu2.h"
#include "FiFo.h"

#define FIFO_SIZE 100
uint8_t fifo[FIFO_SIZE];
uint8_t putIndex;
uint8_t getIndex;

//sets up FIFO indeces
void FiFo_Init(){
	putIndex = 0;
	getIndex = 0;
	
	for(uint8_t n = 0; n < FIFO_SIZE; n++)
		fifo[n] = 0;
}

//puts data into the FiFo
//returns 0 if failure due to full FiFo, returns 1 if success
uint32_t FiFo_Put(char data){	
	if((putIndex+1)%FIFO_SIZE == getIndex){
		//full FiFo
		return 0;
	}
	else{	
		fifo[putIndex] = data;
		++putIndex;
		putIndex %= FIFO_SIZE;
		
		//success
		return(1);
	}
}

//gets an element from the FiFo
//returns 0 if failure due to empty FiFo, returns 1 if success
uint32_t FiFo_Get(char *datapt){
	if(putIndex == getIndex){
		//empty FiFo
		//mBeat();
		return 0;
	}
	else{
		*datapt = fifo[getIndex];
		
		++getIndex;
		getIndex %= FIFO_SIZE;
		
		//success
		return(1);
	}
}
