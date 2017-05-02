#include "stdint.h"
#include "ST7735.h"
#include "Math2.h"

#define WIDTH					128
#define HEIGHT				160
#define SCREEN_HEIGHT	160

uint8_t graphicsBuffer[HEIGHT][WIDTH];			//ROW, COLUMN NOTATION
//uint16_t graphicsBuffer[5][5];

void clipAndDrawLine(float x0, float y0, float x1, float y1, uint8_t color);

void putLineInBuffer(float x0, float y0, float x1, float y1, uint8_t color);

void renderGraphicsBuffer() {
	/*
	Testing buffer
	for(int i = 0; i < WIDTH; i ++) {
		graphicsBuffer[0][i] = 0xFFFF;
	}
	*/
	ST7735_PushBuffer(0,160,&graphicsBuffer[0][0],WIDTH,HEIGHT);
	for(int r = 0; r < HEIGHT; r ++) {
		for(int c = 0; c < WIDTH; c++) {
			graphicsBuffer[r][c] = 0;
		}
	}
		
}

void renderPartialGraphicsBuffer(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	ST7735_PushBuffer(x,160-y,&graphicsBuffer[0][0],w,h);
	for(int r = y; r < h; r ++) {
		for(int c = x; c < w; c++) {
			graphicsBuffer[r][c] = 0;
		}
	}
		
}

void drawImage(int x, int y, const uint8_t *image, int w, int h ) {
	if(x > WIDTH || y > HEIGHT || (x + w) < 0 || (y + h) < 0) {
		return;
	}
	
	int xOffset = fmin(x, 0);						//If x is less than 0 it will be the offset
	int yOffset = fmin(y, 0);						//If y is less than 0 it will be the offset
	
	int realWidth = fmin(w + xOffset, WIDTH - x);
	int realHeight = fmin(h + yOffset, HEIGHT - y);
	
	for(int r = -yOffset; r < realHeight; r ++) {
		for(int c = -xOffset; c < realWidth; c ++) {
			graphicsBuffer[r+y][c+x] = image[r*w + c];
		}
	}
	
}

void drawPixel(int x, int y, uint8_t color) {
	if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
		graphicsBuffer[y][x] = color;
	}
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color) {
	clipAndDrawLine(x0,y0,x1,y1,color);
}


#define INSIDE 	0
#define LEFT		1
#define RIGHT		2
#define BOTTOM	4
#define TOP			8

int findRegion(float x, float y) {
	int region = INSIDE;
	if (x < 1)   
			region |= LEFT;
	else if (x >= WIDTH) 
			region |= RIGHT;
	if (y < 1) 
			region |= BOTTOM;
	else if (y >= HEIGHT)
			region |= TOP;
	return region;
}
 
// Implementing Cohen-Sutherland algorithm
// Clipping a line from P1 = (x2, y2) to P2 = (x2, y2)
void clipAndDrawLine(float x1, float y1, float x2, float y2, uint8_t color) {
	int region1 = findRegion(x1, y1);
	int region2 = findRegion(x2, y2);

	int inside = 0;

	while (1) {
		if ((region1 == 0) && (region2 == 0)) {
				inside = 1;
				break;
		}
		else if (region1 & region2) {
				break;
		}
		else {
			int code_out;
			float x, y;

			if (region1 != 0)
					code_out = region1;
			else
					code_out = region2;

			if (code_out & TOP)	{
					x = x1 + (x2 - x1) * (HEIGHT - 1 - y1) / (y2 - y1);
					y = HEIGHT - 1;
			}	else if (code_out & BOTTOM) {
					x = x1 + (x2 - x1) * (1 - y1) / (y2 - y1);
					y = 1;
			}	else if (code_out & RIGHT) {
					y = y1 + (y2 - y1) * (WIDTH - 1 - x1) / (x2 - x1);
					x = WIDTH - 1;
			} else if (code_out & LEFT) {
					y = y1 + (y2 - y1) * (1 - x1) / (x2 - x1);
					x = 1;
			}
			if (code_out == region1) {
					x1 = x;
					y1 = y;
					region1 = findRegion(x1, y1);
			}	else {
					x2 = x;
					y2 = y;
					region2 = findRegion(x2, y2);
			}
		}
	}
	if (inside == 1) {
		putLineInBuffer(x1, y1, x2, y2, color);
	}
}

void putLineInBuffer(float x0, float y0, float x1, float y1, uint8_t color) {
	if(x0 == x1) {
		if(y0 < y1) {
			for(int i = y0; i < y1; i ++) {
				graphicsBuffer[i][(int)x0] = color;
			}
		} else {
			for(int i = y1; i < y0; i ++) {
				graphicsBuffer[i][(int)x0] = color;
			}
		}
	} else if(x0 < x1) {
		float slope = (y1 - y0)/(x1 - x0);
		for(int i = 0; i < x1-x0; i ++ ) {
			if(slope >= 0) {
				for(float j = 0; j <= slope; j ++) {
					int yCoord = (int)(y0 + i * slope + j);
					int xCoord = (int)x0+i;
					if(xCoord > 0 && xCoord < WIDTH && yCoord > 0 && yCoord < HEIGHT)
						graphicsBuffer[yCoord][xCoord] = color;
				}
			} else {
				for(float j = 0; j >= slope; j --) {
					int yCoord = (int)(y0 + i * slope + j);
					int xCoord = (int)x0+i;
					if(xCoord > 0 && xCoord < WIDTH && yCoord > 0 && yCoord < HEIGHT)
						graphicsBuffer[yCoord][xCoord] = color;
				}
			}
		}
	} else {
		float slope = (y0 - y1)/(x0 - x1);
		for(int i = 0; i < x0-x1; i ++ ) {
			if(slope >= 0) {
				for(float j = 0; j <= slope; j ++) {
					int yCoord = (int)(y1 + i * slope + j);
					int xCoord = (int)x1+i;
					if(xCoord > 0 && xCoord < WIDTH && yCoord > 0 && yCoord < HEIGHT)
						graphicsBuffer[yCoord][xCoord] = color;
				}
			} else {
				for(float j = 0; j >= slope; j --) {
					int yCoord = (int)(y1 + i * slope + j);
					int xCoord = (int)x1+i;
					if(xCoord > 0 && xCoord < WIDTH && yCoord > 0 && yCoord < HEIGHT)
						graphicsBuffer[yCoord][xCoord] = color;
				}
			}
		}
	}
}
