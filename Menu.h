// Menu.h

#include <stdint.h>
#include "Camera.h"

void menuInit(Camera* c);
void playMenu(void);
uint8_t difficultyMenu(Camera c);
void deathMenu(uint16_t score);
