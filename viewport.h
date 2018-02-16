#ifndef POINT_WARNA_H
#define POINT_WARNA_H
#include "point_warna.h"

typedef struct{
	point p1,p2,p3,p4;
	double xmax,xmin,ymax,ymin;
} viewport;

//Mengisi nilai xmax xmin ymax ymin sebuah viewport
void initialize(viewport view){
	double xmax,xmin,ymax,ymin;
	xmax = view.p1.x;
	xmin = view.p1.x;
	ymax = view.p1.y;
	ymin = view.p2.y;
	if(view.p2.x >= xmax){
		xmax = view.p2.x;
	}
	if(view.p2.x < xmin){
		xmin = view.p2.x;
	}
	if(view.p3.x >= xmax){
		xmax = view.p3.x;
	}
	if(view.p3.x < xmin){
		xmin = view.p3.x;
	}
	if(view.p4.x >= xmax){
		xmax = view.p4.x;
	}
	if(view.p4.x < xmin){
		xmin = view.p4.x;
	}
	
	if(view.p2.y >= ymax){
		ymax = view.p2.y;
	}
	if(view.p2.y < ymin){
		ymin = view.p2.y;
	}
	if(view.p3.y >= ymax){
		ymax = view.p3.y;
	}
	if(view.p3.y < ymin){
		ymin = view.p3.y;
	}
	if(view.p4.y >= ymax){
		ymax = view.p4.y;
	}
	if(view.p4.y < ymin){
		ymin = view.p4.y;
	}
}
//Mengecek apakah sebuah titik terdapat didalam viewport
int pointInside(viewport view, point p){
	
}

#endif