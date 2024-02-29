/*
 * baselevel.h
 */

#ifndef DRAWLINE_H_
#define DRAWLINE_H_

#include "world_config.h"



pviewer *locateAllVertices(pviewer p1,pviewer p2,int *size){

	int steps,dx,dy;

	float xinc,yinc,x,y;

	dx = round(p2.x - p1.x);
	dy = round(p2.y - p1.y);

	steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

	*size = steps + 1;
	pviewer *out;
	out = malloc((steps+1)*sizeof(pviewer));

	xinc = dx / (float) steps;
	yinc = dy / (float) steps;

	x = p1.x; y = p1.y;
	for (int i = 0; i <= steps; i++){

		out[i].x = x;
		out[i].y = y;

		x += xinc;
		y += yinc;
	}
	return out;
}

pviewer *findIntersection(pviewer **v_t,int v_t_s,int *vs,int min_y,int max_y,int y){

	int index = 0;

	pviewer *out = malloc(2*sizeof(pviewer));

	pviewer *temp = malloc(4*sizeof(pviewer));

	//To find each intersection go through every vectors
	for (int j=0;j<v_t_s;j++){
		//loop through current vector
		for (int k=0;k<vs[j];k++){
			if (y == round(v_t[j][k].y)){
				temp[index].x = v_t[j][k].x;
				temp[index].y = v_t[j][k].y;
				index++;
				break;
			}
		}
	}
	//Check if y is at min y or max y
	if (index == 2){
		out[0] = temp[0];
		out[1] = temp[1];
	}
	else {
		//deleting repeated values
		int size = 4;
		for (int i=0;i<size;i++){
			for (int j=i+1;j < size;j++){
				if (abs(temp[i].x - temp[j].x) <= 1 && abs(temp[i].y - temp[j].y) <= 1){
					for (int k = j; k < size;k++){
						temp[k] = temp[k+1];
					}
					j--;
					size--;
				}
			}
		}
		out[0] = temp[0];
		out[1] = temp[1];
	}
	free(temp);
	return out;
}

pworld worldToViewer(float cos_theta,float sin_theta,float cos_phi,float sin_phi,float rho, pworld world){

	pworld pview;

	pview.x = -sin_theta * world.x + cos_theta * world.y;
	pview.y = -cos_phi * cos_theta * world.x - cos_phi * sin_phi * world.y + sin_phi * world.z;
	pview.z = -sin_phi * cos_theta * world.x - sin_phi * cos_phi * world.y - cos_phi * world.z + rho;

	return pview;
}

pviewer viewerTo2D(pworld p){
	pviewer out;
	out.x = (disp_scale/p.z)*p.x;
	out.y = (disp_scale/p.z)*p.y;

	return out;
}

pviewer worldToViewerTo2D(float c_t,float s_t,float c_p,float s_p,float rho, pworld w){

	pworld temp;

	pviewer out;

	temp.x = -s_t*w.x + c_t*w.y;

	temp.y = -c_p*c_t*w.x - c_p*s_t*w.y + s_p*w.z;

	temp.z = abs(-s_p*c_t*w.x - s_p*c_t*w.y - c_p*w.z + rho);

	// convert to projection
	out.x = (disp_scale/temp.z) * temp.x;
	out.y = (disp_scale/temp.z) * temp.y;
	return out;
}

int fact(int n) {
    return n <= 0 ? 1 : n * fact(n-1);
}

float cosine(int deg) {

    deg %= 360; // make it less than 360
    float rad = deg * PI / 180;
    float cos = 0;

    int i;
    for(i = 0; i < 7; i++) {
        cos += pow(-1, i) * pow(rad, 2 * i) / fact(2 * i);
    }
    return cos;
}



float sine(int deg) {

    deg %= 360; // make it less than 360
    float rad = deg * PI / 180;
    float sin = 0;

    int i;
    for(i = 0; i < 7; i++) {
        sin += pow(-1, i) * pow(rad, 2 * i + 1) / fact(2 * i + 1);
    }
    return sin;
}
pworld rotate_tree_yz(pworld p2,pworld p1,int deg){
	pworld out;
	out.y = cosine(deg)*p2.y - sine(deg)*p2.z + (-p1.y)*cosine(deg) - (-p1.z)*sine(deg)- (-p1.y);
	out.z = sine(deg)*p2.y + cosine(deg)*p2.z + (-p1.y)*sine(deg) + (-p1.z)*cosine(deg)- (-p1.z);
	out.x = p2.x;
	return out;
}

pworld vector_rotate_xy(pworld p2,pworld p1,int deg){
	pworld out;
	out.x = cosine(deg)*p2.x - sine(deg)*p2.y + (-p1.x)*cosine(deg) - (-p1.y)*sine(deg)- (-p1.x);
	out.y = sine(deg)*p2.x + cosine(deg)*p2.y + (-p1.x)*sine(deg) + (-p1.y)*cosine(deg)- (-p1.y);
	out.z = p2.z;
	return out;
}


void spiwrite(uint8_t c)
{
	 int pnum = 0;
	 src_addr[0] = c;
	 SSP_SSELToggle( pnum, 0 );
	 SSPSend( pnum, (uint8_t *)src_addr, 1 );
	 SSP_SSELToggle( pnum, 1 );
}

void writecommand(uint8_t c)
{
	 LPC_GPIO0->FIOCLR |= (0x1<<21);
	 spiwrite(c);
}

void writedata(uint8_t c)
{
	 LPC_GPIO0->FIOSET |= (0x1<<21);
	 spiwrite(c);
}

void writeword(uint16_t c)
{
	 uint8_t d;
	 d = c >> 8;
	 writedata(d);
	 d = c & 0xFF;
	 writedata(d);
}

void write888(uint32_t color, uint32_t repeat)
{
	 uint8_t red, green, blue;
	 int i;
	 red = (color >> 16);
	 green = (color >> 8) & 0xFF;
	 blue = color & 0xFF;

	 for (i = 0; i< repeat; i++) {
		  writedata(red);
		  writedata(green);
		  writedata(blue);
	 }
}
void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	 writecommand(ST7735_CASET);

	 writeword(x0);

	 writeword(x1);

	 writecommand(ST7735_RASET);

	 writeword(y0);

	 writeword(y1);
}
void drawPixel(int16_t x, int16_t y, uint32_t color)
{
	 if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
	 return;
	 setAddrWindow(x, y, x + 1, y + 1);
	 writecommand(ST7735_RAMWR);
	 write888(color, 1);
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color)
{

	/** converting from virtual to physical display **/

	 //P0(x0,y0)
	x0 = -x0 + (_width + 1)/2; // _width = ST7735_TFTWIDTH;
	y0 = y0 + (_height + 1)/2; // _height = ST7735_TFTHEIGHT;

	 //P1(x1,y1)
	x1 = -x1 + (_width + 1)/2; //_width = ST7735_TFTWIDTH;
	y1 = y1 + (_height + 1)/2; // _height = ST7735_TFTHEIGHT;

	 int16_t slope = abs(y1 - y0) > abs(x1 - x0);

	 if (slope) {
		 swap(x0, y0);
		 swap(x1, y1);
	 }

	 if (x0 > x1) {
		swap(x0, x1); //if x0 > x1, then swap between x0 and x1
		swap(y0, y1); //if x0 < x1, then swap between y0 and y1
	 }

	int16_t dx, dy;

	dx = x1 - x0;

	dy = abs(y1 - y0);

	int16_t err = dx / 2;

	int16_t ystep;

	if (y0 < y1)
	{
		ystep = 1;

	}else{
		ystep = -1;
	}

	for (; x0 <= x1; x0++)
	{
		 if (slope)
		 {
			 drawPixel(y0, x0, color);

		 }else{

			 drawPixel(x0, y0, color);
		 }

		 err -= dy;

		 if (err < 0)
		 {
			 y0 += ystep;
			 err += dx;
		 }
	  }
}

void lcddelay(int ms)
{
	 int count = 24000;
	 int i;
	 for ( i = count * ms; i > 0; i--);
}

void lcd_init()
{
	 int i;
	 printf("LCD Demo Begins!!!\n");

	 LPC_GPIO0->FIODIR |= (0x1<<16);// Set pins P0.16 as output
	 LPC_GPIO0->FIODIR |= (0x1<<21);// Set pins P0.21 as output
	 LPC_GPIO0->FIODIR |= (0x1<<22);// Set pins P0.22 as output

	 // Hardware Reset Sequence
	 LPC_GPIO0->FIOSET |= (0x1<<22);
	 lcddelay(500);

	 LPC_GPIO0->FIOCLR |= (0x1<<22);
	 lcddelay(500);

	 LPC_GPIO0->FIOSET |= (0x1<<22);
	 lcddelay(500);

	 // initialize buffers
	 for ( i = 0; i < SSP_BUFSIZE; i++ )
	 {
		src_addr[i] = 0;
		dest_addr[i] = 0;
	 }

	 // Take LCD display out of sleep mode
	 writecommand(ST7735_SLPOUT);
	 lcddelay(200);

	 // Turn LCD display on
	 writecommand(ST7735_DISPON);
	 lcddelay(200);
}

void fillrect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color)
{
	 //int16_t i;
	 int16_t width, height;
	 width = x1-x0+1;
	 height = y1-y0+1;
	 setAddrWindow(x0,y0,x1,y1);
	 writecommand(ST7735_RAMWR);
	 write888(color,width*height);
}

#endif /* DRAWLINE_H_ */
