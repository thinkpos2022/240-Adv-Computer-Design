
#ifndef SPHERE_H_
#define SPHERE_H_

#include "drawline.h"
#include <stdio.h>
#include <math.h>


void sphereBase(pworld center,float *param,int r,int next){

	pworld p1;

	p1.x = center.x + r;

	p1.y = center.y;

	p1.z = center.z;

	for (int deg = 0; deg < 360; deg += next) {

	    pworld p2 = vector_rotate_xy(p1, center, next);

	    //transform 3d to 2d
	    pviewer p1TwoD = worldToViewerTo2D(param[0],param[1],param[2],param[3],param[4],p1);
	    pviewer p2TwoD = worldToViewerTo2D(param[0],param[1],param[2],param[3],param[4],p2);

	    drawLine(p1TwoD.x, p1TwoD.y, p2TwoD.x, p2TwoD.y, LIGHTBLUE);

	    //shift p2 to p1
	    p1.x = p2.x;
	    p1.y = p2.y;
	    p1.z = p2.z;
	}
}

pworld *halfSpherePts(pworld origin,float *param,int radius,int next){

	pworld p1;

	p1.x = origin.x;
	p1.y = origin.y + radius;
	p1.z = origin.z;

	pworld *point = (pworld *)malloc(18 * sizeof(pworld));

	int count = 0;

	for (int deg = 0; deg < 180; deg += next) {

		point[count].x = p1.x;

		point[count].y = p1.y;

		point[count].z = p1.z;

		count++;

	    pworld p2 = rotate_tree_yz(p1, origin, next);

	    pviewer p1TwoD = worldToViewerTo2D(param[0],param[1],param[2],param[3],param[4], p1);

	    pviewer p2TwoD = worldToViewerTo2D(param[0],param[1],param[2],param[3],param[4], p2);

	    drawLine(p1TwoD.x, p1TwoD.y, p2TwoD.x, p2TwoD.y, LIGHTBLUE);

	    p1.x = p2.x;
	    p1.y = p2.y;
	    p1.z = p2.z;
	}
	return point;
}

void halfVerticalSpherePt(pworld start_origin,float *param, int deg, pworld *half_pts){

	pviewer *draw_temp = (pviewer*)malloc(18*sizeof(pviewer));

	for (int i = 0;i < 18; i++){
		pworld temp = vector_rotate_xy(half_pts[i], start_origin, deg);
		draw_temp[i] = worldToViewerTo2D(param[0],param[1],param[2],param[3],param[4], temp);

	}
	for (int j = 0; j < 18 - 1; j++){
		drawLine(draw_temp[j].x, draw_temp[j].y, draw_temp[j+1].x, draw_temp[j+1].y, LIGHTBLUE);
	}

}



void horizontalBase_wrt_Z(pworld center,float *param,int radius,int next,float z){

	pworld p1;

	p1.x = center.x + radius;
	p1.y = center.y;
	p1.z = z;

	for (int deg = 0; deg < 360; deg += next) {

	    pworld p2 = vector_rotate_xy(p1, center, next);

	    //from world view to perspective view
	    pviewer p1TwoD = worldToViewerTo2D(param[0],param[1],param[2],param[3],param[4],p1);

	    pviewer p2TwoD = worldToViewerTo2D(param[0],param[1],param[2],param[3],param[4],p2);

	    drawLine(p1TwoD.x, p1TwoD.y, p2TwoD.x, p2TwoD.y, LIGHTBLUE);


	    //P2 to P1
	    p1.x = p2.x;
	    p1.y = p2.y;
	    p1.z = p2.z;
	}
}
int radiusFromXY(pworld center, pworld P){

	int point;

	point = sqrt(pow(center.x - P.x, 2) + pow(center.y - P.y, 2));

	return point;
}


void drawTopHalfSphere(pworld origin,float *param,int radius,int next){

	pworld *verticalPt;

	sphereBase(origin, param, radius, next);

	verticalPt = halfSpherePts(origin, param, radius, next);


	for (int deg = 0; deg < 180; deg += next) {

		halfVerticalSpherePt(origin,param, deg, verticalPt);
	}

	for (int i = 0 ; i < 18; i++){

		int temp_radius = radiusFromXY(origin, verticalPt[i]);

		horizontalBase_wrt_Z(origin,param,temp_radius,next,verticalPt[i].z);
	}
}

#endif /* SPHERE_H_ */
