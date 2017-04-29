// FiFo.h
// Last Modified: 4/12/2016 
// Written by: Zachary Chilton & Philip Tan

#include <stdint.h>

void FiFo_Init(void);
uint32_t FiFo_Put(char data);
uint32_t FiFo_Get(char *datapt);
