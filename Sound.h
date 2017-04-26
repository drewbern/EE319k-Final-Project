// Sound.h
// Prototypes for sound functions
// Zachary Chilton
#include <stdint.h>
#define LENGTH 8000

enum soundList{
	NONE,
	LASER
};

typedef struct wav{
	uint16_t length;
	enum soundList name;
	uint8_t sounds[LENGTH];
} wav;

void soundInit(void);
void startSound(wav soundToPlay);
