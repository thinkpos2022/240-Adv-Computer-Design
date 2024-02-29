/*
 * cube.h
 */

#ifndef CUBE_H_
#define CUBE_H_

#include "drawline.h"
#include "world_config.h"

void draw3Dplane(pviewer * twoD){

	drawLine(twoD[0].x,twoD[0].y,twoD[1].x,twoD[1].y,RED); // Draw x-axis
	drawLine(twoD[0].x,twoD[0].y,twoD[2].x,twoD[2].y,GREEN);// Draw y-axis
	drawLine(twoD[0].x,twoD[0].y,twoD[3].x,twoD[3].y,BLUE);// Draw z-axis
}

pviewer *worldview2D(float cosT,float sinT,float cosPhi,float sinePhi,float Rho,pworld *W){

	pviewer *out;
	out = malloc(4*sizeof(pviewer));

	out[0] = worldToViewerTo2D(cosT,sinT,cosPhi,sinePhi,Rho,W[0]);
	out[1] = worldToViewerTo2D(cosT,sinT,cosPhi,sinePhi,Rho,W[1]);
	out[2] = worldToViewerTo2D(cosT,sinT,cosPhi,sinePhi,Rho,W[2]);
	out[3] = worldToViewerTo2D(cosT,sinT,cosPhi,sinePhi,Rho,W[3]);

	return out;
}



void connectFourVertices(uint32_t color,pviewer p0,pviewer p1,pviewer p2,pviewer p3){

	int minY, maxY;

	float y_vec[4] = {p0.y,p1.y,p2.y,p3.y};

	minY = round(min(y_vec,4));

	maxY = round(max(y_vec,4));


	int v0_s,v1_s,v2_s,v3_s;

	pviewer *v0 = locateAllVertices(p0,p1,&v0_s);
	pviewer *v1 = locateAllVertices(p1,p2,&v1_s);
	pviewer *v2 = locateAllVertices(p2,p3,&v2_s);
	pviewer *v3 = locateAllVertices(p3,p0,&v3_s);

	int v_st[4] = {v0_s,v1_s,v2_s,v3_s};

	pviewer **v_total = malloc(4*sizeof(pviewer *));

	v_total[0]=v0;v_total[1]=v1;v_total[2]=v2;v_total[3]=v3;

	for (int i = minY; i <= maxY; i++){

		pviewer *inter_pts = findIntersection(v_total,4,v_st,minY,maxY,i);

		drawLine(round(inter_pts[0].x),round(inter_pts[0].y),round(inter_pts[1].x),round(inter_pts[1].y),color);

		free(inter_pts);
	}
}

void drawCube(pviewer *p){

	//draw the first surface
	connectFourVertices(WHITE,p[4],p[0],p[3],p[7]);

}

void drawFloatingCube(pviewer *p, uint32_t color){

	//the upper layer
	drawLine(p[0].x,p[0].y,p[1].x,p[1].y,color);
	drawLine(p[1].x,p[1].y,p[2].x,p[2].y,color);
	drawLine(p[2].x,p[2].y,p[3].x,p[3].y,color);
	drawLine(p[3].x,p[3].y,p[0].x,p[0].y,color);
	//Then draw middle poles
	drawLine(p[0].x,p[0].y,p[4].x,p[4].y,color);
	drawLine(p[1].x,p[1].y,p[5].x,p[5].y,color);
	drawLine(p[2].x,p[2].y,p[6].x,p[6].y,color);
	drawLine(p[3].x,p[3].y,p[7].x,p[7].y,color);
	//Then draw lower layers
	drawLine(p[4].x,p[4].y,p[5].x,p[5].y,color);
	drawLine(p[5].x,p[5].y,p[6].x,p[6].y,color);
	drawLine(p[6].x,p[6].y,p[7].x,p[7].y,color);
	drawLine(p[7].x,p[7].y,p[4].x,p[4].y,color);
}


// convert from world viewer to prospective viewer
pviewer *Cube2Dviewer(pworld *Cube,float cosTheta,float sineTheta,float cosPhi,float sinePhi,float Rho){

	pworld *temp;
	pviewer *pview;

	temp = malloc(8 * sizeof(pworld));

	pview = malloc(8 * sizeof(pviewer));

	for (int i = 0; i < 8; i++){

		temp[i] = worldToViewer(cosTheta,sineTheta,cosPhi,sinePhi,Rho, Cube[i]);
		pview[i] = viewerTo2D(temp[i]);
	}

	free(temp);
	return pview;
}

#endif /* CUBE_H_ */
