/*
 * world_config.h
 */
#include <math.h>

#ifndef WORLD_CONFIG_H_
#define WORLD_CONFIG_H_

#define PORT_NUM            0


uint8_t src_addr[SSP_BUFSIZE];
uint8_t dest_addr[SSP_BUFSIZE];

#define ST7735_TFTWIDTH 127
#define ST7735_TFTHEIGHT 159
#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_RAMWR 0x2C
#define ST7735_SLPOUT 0x11
#define ST7735_DISPON 0x29
#define PI 3.1415926
#define swap(x, y) {x = x + y; y = x - y; x = x - y ;}

#define BLACK 0x000000
#define GREEN 0x00FF00
#define RED 0xFF0000
#define BLUE 0x0007FF
#define BROWN 0x8B4513
#define WHITE 0xFFFFFF
#define LIGHTBLUE 0x00FFE0


int round(float num);
float min(float *vec, int size);
float max(float *vec, int size);
int _height = ST7735_TFTHEIGHT;
int _width = ST7735_TFTWIDTH;

typedef struct{
	float x;
	float y;
}pviewer;

typedef struct{
	float x;
	float y;
	float z;
}pworld;

typedef struct{
	float red;
	float green;
	float blue;
}RGB;

//Define constant scale
#define disp_scale 100.00
#define kdr 0.85

#endif /* WORLD_CONFIG_H_ */
