// Sound.h
// Prototypes for sound functions
// Zachary Chilton
#include <stdint.h>
#define LENGTH 8000

typedef struct sound{
	uint16_t length;
	uint8_t sounds[LENGTH];
} sound;

void soundInit(void);
void startSound(sound sound);
