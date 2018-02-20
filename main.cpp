#include "rotasi.h"
#include "scaling.h"
#include "point_warna.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <algorithm>
#include "viewport.cpp"

// inisialisasi struct

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
vector<point> pp;
point fillPlane, fillPlane2;
vector<point> fillvector;
vector<point> trimResultXmax(100);
vector<point> trimResultXmin(100);
vector<point> trimResultYmax(100);
vector<point> trimResultYmin(100);
vector<pair<point,char> > colorTupleList;
int layarx = 1366;
int layary = 700;
char *fbp = 0;

color white = { 255, 255, 255, 0 };
color black = {	0, 0, 0, 0 };
color green = {	0, 255, 255, 0 };
color blue = { 0, 255, 0, 0 };

float degreeToRad(float degree) {
    return (degree * M_PI / 180);
}

void draw_dot(int x, int y, color* c) {
	if((x<1) || (x>layarx) || (y<1) || (y>layary)){
		return ;
	}
    long int position = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
       (y + vinfo.yoffset) * finfo.line_length;
    if(vinfo.bits_per_pixel == 32){
        *(fbp + position) = c->b;
        *(fbp + position + 1) = c->g;
        *(fbp + position + 2) = c->r;
        *(fbp + position + 3) = c->a;
    }
    else
    { //assume 16 bit color
        int b = c->b;
        int g = c->g;
        int r = c->r;
        unsigned short int t = r<<11 | g << 5 | b;
        *((unsigned short int*)(fbp + position)) = t;
    }
}

int draw_line(point p1, point p2, color* c) {
	if (p2.x < p1.x) {
		int temp = p1.x;
		p1.x = p2.x;
		p2.x = temp;
		temp = p1.y;
		p1.y = p2.y;
		p2.y = temp;
	}

   int y = p1.y;
   int x = p1.x;
   int dy = p2.y - p1.y;
   int dx = p2.x - p1.x;


    //kasus vertikal
    if (p1.x == p2.x) {
        for (int y = min(p1.y, p2.y); y <= max(p1.y, p2.y); y++) {
            draw_dot(x,y,c);
        }
    }

    //kasus_horizontal
    else if (p1.y == p2.y) {
        for(int x = p1.x; x <= p2.x; x++) {
            draw_dot(x,y,c);
        }
    }

	//kasus miring
    else {
        float grad = (float)(p2.y - p1.y)/(float)(p2.x - p1.x);

        //gradien > 0
        if (grad > 0) {
                //gradien <= 1
                if (grad <= 1) {
                    int dxdy = p2.y - p1.y + p1.x - p2.x;
                    int F = p2.y - p1.y + p1.x - p2.x;
                    for (int x = p1.x; x <= p2.x; x++) {
                        draw_dot(x,y,c);
                        if (F < 0) {
                            F += dy;
                        } else {
                            y++;
                            F += dxdy;
                        }
                    }
                }

                //gradien > 1
                else {
                    int x = p1.x;
                    int dx = p2.x - p1.x;
                    int dxdy = p2.x - p1.x + p1.y - p2.y;
                    int F = p2.x - p1.x + p1.y - p2.y;
                    for (int y = p1.y; y <= p2.y; y++) {

                        draw_dot(x,y,c);
                        if (F < 0) {
                            F += dx;
                        } else {
                            x++;
                            F += dxdy;
                        }
                    }
                }
		//gradien < 0
        } else {
                //gradien >= -1
                if (grad >= -1) {
                    int dy = p2.y - p1.y;
                    if (dy < 0) {
                        dy *= -1;
                    }
                    int dx = p2.x - p1.x;
                    int F = 2*dy - dx;
                    int y = p1.y;

			        for (x = p1.x; x <= p2.x; x++) {
                        draw_dot(x,y,c);
                        if (F > 0) {
                            y--;
                            F = F - 2*dx;
                        }
                        F = F + 2*dy;
			        }
                }
                //gradien < -1
                else {
                    int dx = p1.x - p2.x;
                    if (dx < 0) {
                        dx *= -1;
                    }
                    int dy = p1.y - p2.y;
                    int F = 2*dx - dy;
                    int x = p2.x;

			        for (y = p2.y; y <= p1.y; y++) {
                        draw_dot(x,y,c);
                        if (F > 0) {
                            x--;
                            F = F - 2*dy;
                        }
                        F = F + 2*dx;
			        }
                }
        }
   }
}

void clear_screen(int xx, int yy, int width, int height, color *desired) {
    
    for(int x=xx; x<width; x++)
    {
        for(int y=0; y<height; y++)
        {
            long int position = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
               (y + vinfo.yoffset) * finfo.line_length;
            *(fbp + position) = desired->b;
            *(fbp + position + 1) = desired->g;
            *(fbp + position + 2) = desired->r;
            *(fbp + position + 3) = desired->a;
        }
    }
}

void fil(int x,int y,char prev, color &desired, color &replaced){
    char tm = 255;
	long int position = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y + vinfo.yoffset) * finfo.line_length;
	if ((x<1)||(y<1)||(x>1366)||(y>700) || ( ((*(fbp + position) ^ replaced.b) != 0) ||  ((*(fbp + position + 1) ^ replaced.g) != 0) || ((*(fbp + position + 2) ^ replaced.r) != 0))) {
		return;
	} else {
		draw_dot(x,y,&desired);
        // ignore 0, up 1, down 2, left 3, right 4, berdasarkan arah layar
		if (prev != 3){
            fil(x+1,y,4,desired,replaced);
        }
        if (prev != 4)  {
		    fil(x-1,y,3,desired,replaced);
        }
        if (prev != 1) {
		    fil(x,y-1,2,desired,replaced);
        }
        if (prev != 2)  {
		    fil(x,y+1,1,desired,replaced);	
        }
	}
}

void insertToVector(vector<point> &insertedVector, string nama_file, point shift) {
	
	FILE* charmap;

	charmap = fopen(nama_file.c_str(), "r");

	int jumlah_loop;
	//printf("Jumlah loop = %d\n", jumlah_loop);
	point tempCharPoint;
		
	int i = 0;
	int jumlah_titik;
	fscanf(charmap, "%d", &jumlah_titik);
	//printf("Jumlah titik pada loop %d = %d\n", current_loop, jumlah_titik);
	for (int k = 0; k < jumlah_titik; k++) {
		int x,y;
		fscanf(charmap, "%d  %d", &x, &y);
		//printf("%d %d ", x, y);
		tempCharPoint.x = x+shift.x;
		tempCharPoint.y = y+shift.y;
		insertedVector.push_back(tempCharPoint);
		//printf("%d %d\n", charpoints[k].absis, charpoints[k].ordinat);
	}
    insertedVector.push_back(insertedVector[0]);
	int jumlah_loop_warna;
	int xx,yy, numColour;
    char c;
    fscanf(charmap, "%d", &numColour);
    for (int i = 0; i < numColour; i++) {
        fscanf(charmap, "%d %d %c", &xx, &yy , &c);
        point tempPoint;
        tempPoint.x = xx + shift.x;
        tempPoint.y = yy + shift.y;
        pair<point, char> tempPair(tempPoint,c);
        colorTupleList.push_back(tempPair);
    }
	fclose;
}

void fillPolygon(pair<point,char> p, color &replaced) {
    if (p.second == 'g') {
        fil(p.first.x,p.first.y,0,green,replaced);
        printf("color %f %f\n",p.first.x,p.first.y);
    }
}


int main () {
    point p1, p2;
    p1.x = 650;
    p1.y = 350;
    p2.x = 0;
    p2.y = 200;
    point res;
    res = rotasi(p1,p2,(degreeToRad(45)));
    //printf("%f", sin());
    printf("%f %f\n", res.x, res.y);

    // test rotasi banyak
    point p[2];
    p[0].x = 10;
    p[0].y = 0;
    p[1].x = 0;
    p[1].y = 10;
    //rotasiBanyak(p1, p, degreeToRad(45), 2);
    printf("%f %f\n", p[0].x, p[0].y);
    printf("%f %f\n", p[1].x, p[1].y);

    int fbfd = 0;

	long int screensize = 0;

  	int x = 0, y = 0;
  	long int location = 0;

  	// Open the file for reading and writing
  	fbfd = open("/dev/fb0", O_RDWR);
  	if (fbfd == -1) {
  		perror("Error: cannot open framebuffer device");
		exit(1);
	}

	printf("The framebuffer device was opened successfully.\n");

	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("Error reading fixed information");
		exit(2);
	}

	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("Error reading variable information");
		exit(3);
	}

	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	// Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
			fbfd, 0);
	if ((int)fbp == -1) {
		perror("Error: failed to map framebuffer device to memory");
		exit(4);
	}
	printf("The framebuffer device was mapped to memory successfully.\n");
	//clear_screen(0,0,1366, 700, &black);
	

	//pivot 650,350
	p2.x = 650;
	p2.y = 300;
	
	point ptemp;
	insertToVector(pp,"pesawat_tampak_depan.txt",p1);
	int loop = 0;
    int increment = 0;
    // initialize viewport
    point pv1 = {x = 300, y = 200};
    point pv2 = {x = 600, y = 200};
    point pv3 = {x = 600, y = 500};
    point pv4 = {x = 300, y = 500};
    viewport view;
    view.p1 = pv1;
    view.p2 = pv2;
    view.p3 = pv3;
    view.p4 = pv4;
    initialize(&view);
	// draw_line(pv1.x,pv1.y,pv2.x,pv2.y,&white);
    //     draw_line(pv2.x,pv2.y,pv3.x,pv3.y,&white);
    //     draw_line(pv3.x,pv3.y,pv4.x,pv4.y,&white);
    //     draw_line(pv4.x,pv4.y,pv1.x,pv1.y,&white);

    
    // point ptest1 = {x = 200, y = 300};
    // point ptest2 = {x = 500, y = 400};
    // point ptest3 = {x = 250, y = 400};
    // point res1, res2, res3;

    // vector<point> input;
    // input.push_back(ptest1);
    // input.push_back(ptest2);
    // input.push_back(ptest3);
    // input.push_back(ptest1);

    // trimPolygon(view,input,trimResult,input.size()-1);
    
    for (int i = 0; i < trimResultXmin.size(); i++) {
        //printf("%f %f\n",result[i].x, result[i].y);
        draw_line(trimResultXmin[i], trimResultXmin[i+1], &green);
    }

    while (increment < 2){
    while (1) {
        for (int i = 0;i < pp.size();i++){
            printf("%f %f\n",pp[i].x,pp[i].y);
        }
        printf("Number of Point Before :%d\n",pp.size());
        printf("+++++++++++++++++++++++\n");
        trimPolygonXMin(view,pp,trimResultXmin,pp.size());
        trimPolygonXMax(view,trimResultXmin,trimResultXmax,trimResultXmin.size());
        trimPolygonYMin(view,trimResultXmax,trimResultYmin,trimResultXmax.size());
        int test = trimResultXmin.size();
		for (int i = 0; i < trimResultYmin.size()-1; i++) {
			draw_line(trimResultYmin[i], trimResultYmin[i+1], &white);
            printf("%f %f\n",trimResultXmax[i].x,trimResultXmax[i].y);
        }
        printf("%f %f\n",trimResultXmin[test-1].x,trimResultXmax[test-1].y);
        printf("Number of Point After :%d\n",trimResultXmax.size());
        printf("====================================\n");
        usleep(5000);
		draw_line(pv1, pv2,&white);
        draw_line(pv2, pv3, &white);
        draw_line(pv3, pv4, &white);
        draw_line(pv4, pv1, &white);
        draw_dot(p1.x,p1.y,&black);
        
        for (int i = 0; i < colorTupleList.size(); i++) {
            if (pointPos(view,colorTupleList[i].first) == 0) {
                fillPolygon(colorTupleList[i],black);
            }
        }
            //fil(p1.x,p1.y,0,green, black);
            // fil(fillPlane.x,fillPlane.y,0,green,black);
            // fil(fillPlane2.x,fillPlane2.y,0,green,black);
        
		
		// for (int i = 0; i < 30; i++){  
        //   draw_line(p1.x,p1.y,p2.x+i,p2.y+i,&white);    // Baling2
        // }
        
        // if (loop == 20) break;
		// // clear screen mini
        usleep(500000);
        // for (int i = 0; i < 30; i++){  
        //   draw_line(p1.x,p1.y,p2.x+i,p2.y+i,&green);    // Baling2
        // }
        clear_screen(view.xmin,view.ymin,view.xmax,view.ymax,&black);
        // if (p2.y > p1.y){
            
        // } else {
        //     clear_screen(0,p2.y+100,1366,p1.y+30,&black);
        // } 
        
        
		// for (int i = 0; i < pp.size()-1; i++) {
		// 	draw_line(trimResult[i].x, trimResult[i].y, trimResult[i+1].x, trimResult[i+1].y, &black);
		// }
        // //usleep(50000);
		// draw_line(trimResult[pp.size()-1].x,trimResult[pp.size()-1].y,trimResult[0].x,trimResult[0].y,&black);
		// // p2 = scalePoint(p1,p2,1.1);
        // // fillPlane = scalePoint(p1,fillPlane,1.1);
        // // fillPlane2 = scalePoint(p1,fillPlane2,1.1);
		// p2 = rotasi(p1,p2,degreeToRad(20));
		//scaleBanyak(p1, trimResult, 1.1, pp.size());
        for (int i = 0; i < pp.size(); i++) {
            pp[i].x -= 5;
            pp[i].y -= 5;
        }
        for (int i = 0; i < colorTupleList.size(); i++) {
            colorTupleList[i].first.x -= 5;
            colorTupleList[i].first.y -= 5;
        }
        p1.x -= 5;
        p1.y -= 5;
        p2.x -= 5;
        fillPlane.x -= 5;
        fillPlane2.x -= 5;
		loop++;
	}

    // while (1) {
	// 	for (int i = 0; i < pp.size()-1; i++) {
	// 		draw_line(pp[i].x, pp[i].y, pp[i+1].x, pp[i+1].y, &white);
	// 	}
	// 	draw_line(pp[pp.size()-1].x,pp[pp.size()-1].y,pp[0].x,pp[0].y,&white);
	// 	draw_dot(p1.x,p1.y,&black);
    //     fil(p1.x,p1.y,0,&green);
	// 	fil(fillPlane.x,fillPlane.y,0,&green);
    //     fil(fillPlane2.x,fillPlane2.y,0,&green);
	// 	for (int i = 0; i < 30; i++){  
    //       draw_line(p1.x,p1.y,p2.x+i,p2.y+i,&white);    // Baling2
    //     }
        
    //     if (loop == 10) break;
	// 	// clear screen mini
    //     usleep(50000);
    //     for (int i = 0; i < 30; i++){  
    //       draw_line(p1.x,p1.y,p2.x+i,p2.y+i,&green);    // Baling2
    //     }
    //     if (p2.y > p1.y){
    //         clear_screen(0,pp[3].y,1366,p2.y+100,&black);
    //     } else {
    //         clear_screen(0,p2.y+100,1366,pp[8].y,&black);
    //     } 
        
        
	// 	for (int i = 0; i < pp.size()-1; i++) {
	// 		draw_line(pp[i].x, pp[i].y, pp[i+1].x, pp[i+1].y, &black);
	// 	}
    //     //usleep(50000);
	// 	draw_line(pp[pp.size()-1].x,pp[pp.size()-1].y,pp[0].x,pp[0].y,&black);
	// 	p2 = scalePoint(p1,p2,0.9);
    //     fillPlane = scalePoint(p1,fillPlane,0.9);
    //     fillPlane2 = scalePoint(p1,fillPlane2,0.9);
	// 	p2 = rotasi(p1,p2,degreeToRad(20));
	// 	scaleBanyak(p1, pp, 0.9, pp.size());
	// 	loop--;
	// }
    increment++;
    }

    return 0;
}

