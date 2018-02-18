#include "point_warna.h"

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

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
//Mengecek posisi titik terhadap viewport. Jika pos = 0 maka titik berada dalam viewport 
int pointPos(viewport view, point p){
	int pos = 0;
	if (p.x < view.xmin){
		pos += 8;
	} else if (p.x > view.xmax){
		pos += 4;
	}
	
	if (p.y < view.ymin){
		pos += 2;
	} else if (p.y > view.ymax){
		pos += 1;
	}
	
	return pos;
}


//Memotong garis terhadap XMin, Stat adalah hasil jika Stat = 1 berarti memotong, Stat = 0 berarti tidak berubah, Stat = -1 Garis berada diluar
void trimLineXMin(viewport view, point p1, point p2,point *np1,point *np2, int *stat){
	int pos1 = pointPos(view,p1);
	int pos2 = pointPos(view,p2);
	if((CHECK_BIT(pos1,4))&&(!CHECK_BIT(pos2,4))){
		np2->x = p2.x;
		np2->y = p2.y;
		np1->x = view.xmin;
		
		float grad = (p2.y - p1.y) / (p2.x - p1.x);
		float c = p2.y - (grad*p2.x);
		np1->y =  (double)(grad*view.xmin) +  (double)(c);
		*stat = 1;
		
	} else if ((!CHECK_BIT(pos1,4))&&(CHECK_BIT(pos2,4))){
		np1->x = p1.x;
		np1->y = p1.y;
		np2->x = view.xmin;
		
		float grad = (p2.y - p1.y) / (p2.x - p1.x);
		float c = p2.y - (grad*p2.x);
		np2->y =  (double)(grad*view.xmin) +  (double)(c);
		*stat = 1;
		
	} else if ((!CHECK_BIT(pos1,4))&&(!CHECK_BIT(pos2,4))){
		np2->x = p2.x;
		np2->y = p2.y;
		np1->x = p1.x;
		np1->y = p1.y;
		*stat = 0;
		
	} else if ((CHECK_BIT(pos1,4))&&(*np1(pos2,4))){
		np1->x = 0;
		np2->x = 0;
		np1->y = 0;
		np2->y = 0;
		*stat = -1;
	}
}

void trimLineXMax(viewport view, point p1, point p2,point *np1,point *np2, int *stat){
	int pos1 = pointPos(view,p1);
	int pos2 = pointPos(view,p2);
	if((CHECK_BIT(pos1,3))&&(!CHECK_BIT(pos2,3))){
		np2->x = p2.x;
		np2->y = p2.y;
		np1->x = view.xmax;
		
		float grad = (p2.y - p1.y) / (p2.x - p1.x);
		float c = p2.y - (grad*p2.x);
		np1->y =  (double)(grad*view.xmax) +  (double)(c);
		*stat = 1;
		
	} else if ((!CHECK_BIT(pos1,3))&&(CHECK_BIT(pos2,3))){
		np1->x = p1.x;
		np1->y = p1.y;
		np2->x = view.xmax;
		
		float grad = (p2.y - p1.y) / (p2.x - p1.x);
		float c = p2.y - (grad*p2.x);
		np2->y =  (double)(grad*view.xmax) +  (double)(c);
		*stat = 1;
		
	} else if ((!CHECK_BIT(pos1,3))&&(!CHECK_BIT(pos2,3))){
		np2->x = p2.x;
		np2->y = p2.y;
		np1->x = p1.x;
		np1->y = p1.y;
		*stat = 0;
		
	} else if ((CHECK_BIT(pos1,3))&&(CHECK_BIT(pos2,3))){
		np1->x = 0;
		np2->x = 0;
		np1->y = 0;
		np2->y = 0;
		*stat = -1;
	}
}

void trimLineYMin(viewport view, point p1, point p2,point *np1,point *np2, int *stat){
	int pos1 = pointPos(view,p1);
	int pos2 = pointPos(view,p2);
	if((CHECK_BIT(pos1,2))&&(!CHECK_BIT(pos2,2))){
		np2->x = p2.x;
		np2->y = p2.y;
		np1->y = view.ymin;
		
		float grad = (p2.y - p1.y) / (p2.x - p1.x);
		float c = p2.y - (grad*p2.x);
		np1->x = (double)(view.ymin / grad) - (double) (c);
		*stat = 1;
		
	} else if ((!CHECK_BIT(pos1,2))&&(CHECK_BIT(pos2,2))){
		np1->x = p1.x;
		np1->y = p1.y;
		np2->x = view.ymin;
		
		float grad = (p2.y - p1.y) / (p2.x - p1.x);
		float c = p2.y - (grad*p2.x);
		np1->x = (double)(view.ymin / grad) - (double) (c);
		*stat = 1;
		
	} else if ((!CHECK_BIT(pos1,2))&&(!CHECK_BIT(pos2,2))){
		np2->x = p2.x;
		np2->y = p2.y;
		np1->x = p1.x;
		np1->y = p1.y;
		*stat = 0;
		
	} else if ((CHECK_BIT(pos1,2))&&(CHECK_BIT(pos2,2))){
		np1->x = 0;
		np2->x = 0;
		np1->y = 0;
		np2->y = 0;
		*stat = -1;
	}
}

void trimLineYMax(viewport view, point p1, point p2,point *np1,point *np2, int *stat){
	int pos1 = pointPos(view,p1);
	int pos2 = pointPos(view,p2);
	if((CHECK_BIT(pos1,1))&&(!CHECK_BIT(pos2,1))){
		np2->x = p2.x;
		np2->y = p2.y;
		np1->y = view.ymax;
		
		float grad = (p2.y - p1.y) / (p2.x - p1.x);
		float c = p2.y - (grad*p2.x);
		np1->x = (double)(view.ymax / grad) - (double) (c);
		*stat = 1;
		
	} else if ((!CHECK_BIT(pos1,1))&&(CHECK_BIT(pos2,1))){
		np1->x = p1.x;
		np1->y = p1.y;
		np2->x = view.ymax
		
		float grad = (p2.y - p1.y) / (p2.x - p1.x);
		float c = p2.y - (grad*p2.x);
		np1->x = (double)(view.ymax / grad) - (double) (c);
		*stat = 1;
		
	} else if ((!CHECK_BIT(pos1,1))&&(!CHECK_BIT(pos2,1))){
		np2->x = p2.x;
		np2->y = p2.y;
		np1->x = p1.x;
		np1->y = p1.y;
		*stat = 0;
		
	} else if ((CHECK_BIT(pos1,1))&&(CHECK_BIT(pos2,1))){
		np1->x = 0;
		np2->x = 0;
		np1->y = 0;
		np2->y = 0;
		*stat = -1;
	}
}


// ArrNodes adalah Array yang berisi titik sebelum di trim, nArrNodes adalah Array berisi titik setelah dilakukan trim, N adalah jumlah titik awal.
void trimPolygon(viewport view, point *ArrNodes, point *nArrNodes, int *n){
	int total = *n;
	int found = 0;
	point np1, np2;
	int count = 0;
	int stat;
	//Trim terhadap Xmin
	for (int i = 0; i+1 < total; i++){
		trimLineXMin(view,ArrNodes[i],ArrNodes[i+1],&np1,&np2,&stat);
		if(stat == 0){
			
		}
	}
	//Trim terhadap Xmax
	for (int i = 0; i+1 < total; i++){
		trimLineXMax(view,ArrNodes[i],ArrNodes[i+1],&np1,&np2,&stat);
	}
	//Trim terhadap Ymin
	for (int i = 0; i+1 < total; i++){
		trimLineYmin(view,ArrNodes[i],ArrNodes[i+1],&np1,&np2,&stat);
	}
	//Trim terhadap Ymax
	for (int i = 0; i+1 < total; i++){
		trimLineYmax(view,ArrNodes[i],ArrNodes[i+1],&np1,&np2,&stat);
	}
}
