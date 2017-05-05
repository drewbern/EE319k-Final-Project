// Sound.h
// Prototypes for sound functions
// Zachary Chilton
#include <stdint.h>
#define LENGTH 		1771
#define LENGTH2 	3323
#define LENGTH3 	4279
#define LENGTH4 	12712
#define LENGTH5		21125
#define LENGTH6 	38843

enum soundList{
	NONE,
	MENU,
	BOMB,
	LASER,
	SELECT,
	THEME,
	DEATH,
	ALERT,
	DAMAGE
};

typedef struct smallestwav{
	uint16_t length;
	enum soundList name;
	uint8_t sounds[LENGTH];
} smallestwav;

typedef struct wav{
	uint16_t length;
	enum soundList name;
	uint8_t sounds[LENGTH2];
} wav;

typedef struct medwav{
	uint16_t length;
	enum soundList name;
	uint16_t sounds[LENGTH3];
} medwav;

typedef struct biggerwav{
	uint16_t length;
	enum soundList name;
	uint16_t sounds[LENGTH4];
} biggerwav;

typedef struct death{
	uint16_t length;
	enum soundList name;
	uint16_t sounds[LENGTH5];
} deathwav;

typedef struct bigwav{
	uint16_t length;
	enum soundList name;
	uint16_t sounds[LENGTH6];
} bigwav;

typedef struct alert{
	uint16_t length;
	enum soundList name;
	uint16_t sounds[7013];
} alertwav;

typedef struct damage{
	uint16_t length;
	enum soundList name;
	uint16_t sounds[5176];
} damagewav;

void soundInit(void);
void startSound(enum soundList name);
void sound_laser(void);
void sound_bomb(void);
void sound_menu(void);
void sound_select(void);
void sound_theme(void);
void sound_death(void);
void sound_alert(void);
void sound_damage(void);

//DAC
void DACOut(uint8_t data);
void DACInit(void);
