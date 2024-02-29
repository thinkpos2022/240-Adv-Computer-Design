/*
===============================================================================
 Name        : main.c
 Author      : Nanthana Thanonklin
 Project 2   : World Coordinate System and Transformation Pipeline
 LPC 1769
===============================================================================
*/

#include <cr_section_macros.h>
#include <NXP/crp.h>
#include "LPC17xx.h"                        /* LPC17xx definitions */
#include "ssp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "Cube.h"
#include "sphere.h"
#include "drawline.h"
#include "world_config.h"

int main (void)
{
	uint32_t pnum = PORT_NUM;
	pnum = 0 ;
	if ( pnum == 0 )
		SSP0Init();
	else
		puts("Incorrect Port!");
	lcd_init();
	// fill background color with BROWN
	fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
	pworld Eye_Loc; //virtual camera location
	//E(x,y,z) = (200, 200, 200) and D = 20~50 for perspective projection
	Eye_Loc.x = 200;
	Eye_Loc.y = 200;
	Eye_Loc.z = 200;
	// Rho, sinϴ, cosϴ, sinϕ and cosϕ computation for world-to-viewer
	float Rho = sqrt(pow(Eye_Loc.x,2)+ pow(Eye_Loc.y,2) + pow(Eye_Loc.z,2));

	float cos_theta = Eye_Loc.x /sqrt(pow(Eye_Loc.x,2)+pow(Eye_Loc.y,2));

	float sine_theta = Eye_Loc.y / sqrt(pow(Eye_Loc.x,2)+pow(Eye_Loc.y,2));

	float cos_Phi = Eye_Loc.z / Rho;

	float sine_Phi = sqrt(pow(Eye_Loc.x,2) + pow(Eye_Loc.y,2)) / Rho;

	//define the x-y-z world coordinate

	pworld *world = malloc(4*sizeof(pworld));

	world[0].x = 0.0; world[0].y = 0.0; world[0].z = 0.0; // origin (0,0,0)

	world[1].x = 250.0; world[1].y = 0.0; world[1].z = 0.0;	// x-axis (250,0,0)

	world[2].x = 0.0; world[2].y = 250.0; world[2].z = 0.0; // y-axis(0,250,0)

	world[3].x = 0.0; world[3].y = 0.0; world[3].z = 250.0; // y-axis(0,0,250)

	//Convert world viewer (3D)to 2D viewer, pviewer is prospective viewer
	pviewer *perspective_2D;

	perspective_2D = worldview2D(cos_theta,sine_theta,cos_Phi,sine_Phi,Rho, world);

	pworld *Cube;
	Cube = malloc(8*sizeof(pworld));

	//The top surface of the cube

	Cube[0].x = 100; Cube[0].y = 100; Cube[0].z = 110; //The first vertex

	Cube[1].x = 100; Cube[1].y = 0; Cube[1].z = 110; //The second vertex

	Cube[2].x = 0; Cube[2].y = 0; Cube[2].z = 110; //The third vertex

	Cube[3].x = 0; Cube[3].y = 100; Cube[3].z = 110; //The fourth vertex

	// The bottom surface of the cube
	Cube[4].x = 100; Cube[4].y = 100; Cube[4].z = 10; //The first vertex

	Cube[5].x = 100; Cube[5].y = 0; Cube[5].z = 10;//The second vertex

	Cube[6].x = 0; Cube[6].y = 0; Cube[6].z = 10;	//The third vertex

	Cube[7].x = 0; Cube[7].y = 100; Cube[7].z = 10;//The fourth vertex

	//Convert world to viewer points, and then to 2D projection
	pviewer *Cube_2D;
	Cube_2D = Cube2Dviewer(Cube, cos_theta, sine_theta, cos_Phi, sine_Phi, Rho);

	//Draw World coordinates (3D)
	draw3Dplane(perspective_2D);

	//Draw the world coordinate system with the floating Cube in white
	drawFloatingCube(Cube_2D, WHITE);

	//initialize points on the sphere
	//define each of the axis equal to 50
	pworld circleWorldview = {.x = 50,.y = 50, .z = 50};

	float *sphere_pts = (float *)malloc(5 * sizeof(float));

	sphere_pts[0] = cos_theta;
	sphere_pts[1] = sine_theta;
	sphere_pts[2] = cos_Phi;
	sphere_pts[3] = sine_Phi;
	sphere_pts[4] = Rho;

	//Create a sphere with R = 100 with 10 levels of cross section contours

	//void drawTopHalfSphere(pworld start_center,float *prams,int start_r,int deg_inc)

	drawTopHalfSphere(circleWorldview, sphere_pts, 100, 10);

	free(perspective_2D);
	free(Cube_2D);
	free(Cube);
	free(world);

	return 0 ;
}
