#include "stdint.h"
#include "ST7735.h"
#include <math.h>

//Original 	(R1 R2 R3 G1 G2 G3 B1 B2)
//Byte 1 		(R1 R1 R2 R2 R3 G1 G1 G2)
//Byte 2		(G2 G3 G3 B1 B1 B2 B2 B2)

#define RED								0x03
#define BLUE							0xE0
#define WHITE							0xFF
#define GREEN							0x1C
#define BLACK							0
#define GREY							0xDA
#define YELLOW						0xFC						//This looks like cyan for some reason RIP but I'll roll with it
#define DARK_YELLOW				0xFE
#define ORANGE						0x3F						//xFFA500

void renderGraphicsBuffer(void);

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);

void drawPixel(int x, int y, uint8_t color);

void drawImage(int x, int y, const uint8_t *image, int w, int h );
