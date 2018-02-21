#include "point_warna.h"
#include <vector>
#include <cstdio>
#include <bitset>

int CHECK_BIT(int var,int pos){
	std::bitset<4> bi(var);
	return bi[pos-1];
}

typedef struct{
	point p1,p2,p3,p4;
	double xmax,xmin,ymax,ymin;
} viewport;

//Mengisi nilai xmax xmin ymax ymin sebuah viewport
void initialize(viewport* view){
	view->xmax = view->p1.x;
	view->xmin = view->p1.x;
	view->ymax = view->p1.y;
	view->ymin = view->p1.y;
	if(view->p2.x >= view->xmax){
		view->xmax = view->p2.x;
	}
	if(view->p2.x < view->xmin){
		view->xmin = view->p2.x;
	}
	if(view->p3.x >= view->xmax){
		view->xmax = view->p3.x;
	}
	if(view->p3.x < view->xmin){
		view->xmin = view->p3.x;
	}
	if(view->p4.x >= view->xmax){
		view->xmax = view->p4.x;
	}
	if(view->p4.x < view->xmin){
		view->xmin = view->p4.x;
	}
	
	if(view->p2.y >= view->ymax){
		view->ymax = view->p2.y;
	}
	if(view->p2.y < view->ymin){
		view->ymin = view->p2.y;
	}
	if(view->p3.y >= view->ymax){
		view->ymax = view->p3.y;
	}
	if(view->p3.y < view->ymin){
		view->ymin = view->p3.y;
	}
	if(view->p4.y >= view->ymax){
		view->ymax = view->p4.y;
	}
	if(view->p4.y < view->ymin){
		view->ymin = view->p4.y;
	}
}

void translasiX(viewport &view, int x){
	view.xmax += x;
	view.xmin += x;
}

void translasiY(viewport &view, int y){
	view.ymax += y;
	view.ymin += y;
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


//Memotong garis terhadap XMin, Stat menunjukan potongan seperti apa.
void trimLineXMin(viewport view, point p1, point p2,point *np1,point *np2, int *stat){
	int pos1 = pointPos(view,p1);
	int pos2 = pointPos(view,p2);
	//printf("%d %d\n",(CHECK_BIT(pos1,3)),pos1);
	if((CHECK_BIT(pos1,4))&&(!CHECK_BIT(pos2,4))){
		np2->x = p2.x;
		np2->y = p2.y;
		np1->x = view.xmin;
		
		float grad = (p2.y - p1.y) / (p2.x - p1.x);
		float c = p2.y - (grad*p2.x);
		np1->y =  (grad*view.xmin) +  (c);
		*stat = 1;
		
	} else if ((!CHECK_BIT(pos1,4))&&(CHECK_BIT(pos2,4))){
		np1->x = p1.x;
		np1->y = p1.y;
		np2->x = view.xmin;
		
		float grad = (p2.y - p1.y) / (p2.x - p1.x);
		float c = p2.y - (grad*p2.x);
		np2->y =  (double)(grad*view.xmin) +  (double)(c);
		*stat = 2;
		
	} else if ((!CHECK_BIT(pos1,4))&&(!CHECK_BIT(pos2,4))){
		np2->x = p2.x;
		np2->y = p2.y;
		np1->x = p1.x;
		np1->y = p1.y;
		*stat = 0;
		
	} else if ((CHECK_BIT(pos1,4))&&(CHECK_BIT(pos2,4))){
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
		*stat = 2;
		
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
		*stat = 2;
		
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
		np2->x = view.ymax;
		
		float grad = (p2.y - p1.y) / (p2.x - p1.x);
		float c = p2.y - (grad*p2.x);
		np1->x = (double)(view.ymax / grad) - (double) (c);
		*stat = 2;
		
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
// ArrNodes[0] harus == ArrNodes[n] (contoh A-B-C-A akan membentuk segitiga)
// Baru sama XMin
void trimPolygonXMin(viewport view, vector<point> ArrNodes, vector<point> &nArrNodes, int n){
	int total = n;
	int found = 0;
	point temp;
	point np1, np2;
	int count = 0;
	int stat;
	//Trim terhadap Xmin
	for (int i = 0; i < (total-1); i++){
		trimLineXMin(view,ArrNodes[i],ArrNodes[i+1],&np1,&np2,&stat);
		if(stat == 0){
			nArrNodes[count] = np1;
			nArrNodes[count+1] = np2;
			count++;
		}else if (stat == 1){
			if(found == 1){
				count++;
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				found = 0;
				count ++;
			}else{
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				temp = np1;
				count++;
				found = 1;
			}
			
		}else if (stat == 2){
			if(found == 1){
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				nArrNodes[count+2] = temp;
				count += 2;;
				found = 0;
			}else{
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				temp = np2;
				count++;
				found = 1;
			}
		}
	}
	nArrNodes.resize(count+1);
	// for (int i = 0; i < total; i++) {
	// 	printf("%f %f, %f %f\n",ArrNodes[i].x, ArrNodes[i].y, nArrNodes[i].x, nArrNodes[i].y);
	// }
}

void trimPolygonXMax(viewport view, vector<point> ArrNodes, vector<point> &nArrNodes, int n){
	int total = n;
	int found = 0;
	point temp;
	point np1, np2;
	int count = 0;
	int stat;
	//Trim terhadap Xmin
	for (int i = 0; (i+1)< total; i++){
		trimLineXMax(view,ArrNodes[i],ArrNodes[i+1],&np1,&np2,&stat);
		if(stat == 0){
			nArrNodes[count] = np1;
			nArrNodes[count+1] = np2;
			count++;
		}else if (stat == 1){
			if(found == 1){
				count++;
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				found = 0;
				count++;
			}else{
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				temp = np1;
				count++;
				found = 1;
			}
			
		}else if (stat == 2){
			if(found == 1){
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				nArrNodes[count+2] = temp;
				count += 2;
				found = 0;
			}else{
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				temp = np1;
				count++;
				found = 1;
			}
		}
	}
	nArrNodes.resize(count+1);
	// for (int i = 0; i < total; i++) {
	// 	printf("%f %f, %f %f\n",ArrNodes[i].x, ArrNodes[i].y, nArrNodes[i].x, nArrNodes[i].y);
	// }
}

void trimPolygonYMin(viewport view, vector<point> ArrNodes, vector<point> &nArrNodes, int n){
	int total = n;
	int found = 0;
	point temp;
	point np1, np2;
	int count = 0;
	int stat;
	//Trim terhadap Xmin
	for (int i = 0; (i+1)< total; i++){
		trimLineYMin(view,ArrNodes[i],ArrNodes[i+1],&np1,&np2,&stat);
		if(stat == 0){
			nArrNodes[count] = np1;
			nArrNodes[count+1] = np2;
			count++;
		}else if (stat == 1){
			if(found == 1){
				count++;
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				found = 0;
				count++;
			}else{
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				temp = np1;
				count++;
				found = 1;
			}
			
		}else if (stat == 2){
			if(found == 1){
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				nArrNodes[count+2] = temp;
				count += 2;
				found = 0;
			}else{
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				temp = np1;
				count++;
				found = 1;
			}
		}
	}
	nArrNodes.resize(count+1);
	// for (int i = 0; i < total; i++) {
	// 	printf("%f %f, %f %f\n",ArrNodes[i].x, ArrNodes[i].y, nArrNodes[i].x, nArrNodes[i].y);
	// }
}

void trimPolygonYMax(viewport view, vector<point> ArrNodes, vector<point> &nArrNodes, int n){
	int total = n;
	int found = 0;
	point temp;
	point np1, np2;
	int count = 0;
	int stat;
	//Trim terhadap Xmin
	for (int i = 0; (i+1)< total; i++){
		trimLineYMax(view,ArrNodes[i],ArrNodes[i+1],&np1,&np2,&stat);
		if(stat == 0){
			nArrNodes[count] = np1;
			nArrNodes[count+1] = np2;
			count++;
		}else if (stat == 1){
			if(found == 1){
				count++;
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				found = 0;
				count++;
			}else{
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				temp = np1;
				count++;
				found = 1;
			}
			
		}else if (stat == 2){
			if(found == 1){
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				nArrNodes[count+2] = temp;
				count += 2;
				found = 0;
			}else{
				nArrNodes[count] = np1;
				nArrNodes[count+1] = np2;
				temp = np1;
				count++;
				found = 1;
			}
		}
	}
	nArrNodes.resize(count+1);
	// for (int i = 0; i < total; i++) {
	// 	printf("%f %f, %f %f\n",ArrNodes[i].x, ArrNodes[i].y, nArrNodes[i].x, nArrNodes[i].y);
	// }
}