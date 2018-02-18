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
#include <pthread.h>

// inisialisasi struct
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
vector<point> pp;
point fillPlane, fillPlane2;
vector<point> fillvector;
int layarx = 1366;
int layary = 700;
char *fbp = 0;
color white = {
			 255,
			 255,
			 255,
			 0
	 };
color black = {
			0,0,254,0
	 };

color green = {
	0,255,255,0
};

color blue = {
	0,255,0,0
};


int min(int y1, int y2){
	if (y1 < y2) {
		return y1;
	} else {
		return y2;
	}
}

int max(int y1, int y2) {
	if (y2 < y1) {
		return y1;
	} else {
		return y2;
	}
}

point translasi(point *p, int dx, int dx){
	point result;
	result.x = p.x + dx;
	result.y = p.y + dy;

	return result;
}

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

int draw_line(int x1, int y1, int x2, int y2, color* c) {
	if (x2 < x1) {
		int temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

   int y = y1;
   int x = x1;
   int dy = y2 - y1;
   int dx = x2 - x1;


    //kasus vertikal
    if (x1 == x2) {
        for (int y = min(y1, y2); y <= max(y1, y2); y++) {
            draw_dot(x,y,c);
        }
    }

    //kasus_horizontal
    else if (y1 == y2) {
        for(int x = x1; x <= x2; x++) {
            draw_dot(x,y,c);
        }
    }

	//kasus miring
    else {
        float grad = (float)(y2-y1)/(float)(x2-x1);

        //gradien > 0
        if (grad > 0) {
                //gradien <= 1
                if (grad <= 1) {
                    int dxdy = y2 - y1 + x1 - x2;
                    int F = y2 - y1 + x1 - x2;
                    for (int x = x1; x <= x2; x++) {
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
                    int x = x1;
                    int dx = x2 -x1;
                    int dxdy = x2 - x1 + y1 - y2;
                    int F = x2 - x1 + y1 - y2;
                    for (int y = y1; y <= y2; y++) {

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
                    int dy = y2 - y1;
                    if (dy < 0) {
                        dy *= -1;
                    }
                    int dx = x2 - x1;
                    int F = 2*dy - dx;
                    int y = y1;

			        for (x = x1; x <= x2; x++) {
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
                    int dx = x1 - x2;
                    if (dx < 0) {
                        dx *= -1;
                    }
                    int dy = y1 - y2;
                    int F = 2*dx - dy;
                    int x = x2;

			        for (y = y2; y <= y1; y++) {
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

void clear_screen(int x, int y, int width, int height, color *desired) {


    for(x=0; x<width; x++)
    {
        for(y=0; y<height; y++)
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

long int pos(int x, int y){
	long int position = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y + vinfo.yoffset) * finfo.line_length;
	return position;
}

void fil(int x,int y,char prev, color* desired){

	long int position = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y + vinfo.yoffset) * finfo.line_length;
	if ((x<1)||(y<1)||(x>1366)||(y>700) || (*(fbp + position) == -1)) {
		return;
	} else {
		draw_dot(x,y,desired);
        // ignore 0, up 1, down 2, left 3, right 4, berdasarkan arah layar
		if (prev != 3){
            fil(x+1,y,4,desired);
        }
        if (prev != 4)  {
		    fil(x-1,y,3,desired);
        }
        if (prev != 1) {
		    fil(x,y-1,2,desired);
        }
        if (prev != 2)  {
		    fil(x,y+1,1,desired);
        }
	}
}

void insertToVector(char* nama_file) {

	FILE* charmap;

	charmap = fopen(nama_file, "r");

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
		tempCharPoint.x = x+650;
		tempCharPoint.y = y+350;
		pp.push_back(tempCharPoint);
		//printf("%d %d\n", charpoints[k].absis, charpoints[k].ordinat);
	}
	int jumlah_loop_warna;
	int x,y;
	fscanf(charmap, "%d %d", &x, &y);
	fillPlane.x = x+650;
	fillPlane.y = y+350;
  fscanf(charmap, "%d %d", &x, &y);
	fillPlane2.x = x+650;
	fillPlane2.y = y+350;
	fclose;
}

int main () {
    point p1, p2, p3;
    p1.x = 650;
    p1.y = 350;
    p3.x = 0;
    p3.y = 200;
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
	clear_screen(0,0,1366, 700, &black);


	//pivot 650,350
	p2.x = 650;
	p2.y = 300;

	point ptemp;
	insertToVector((char*)"pesawat_tampak_depan.txt");
	int loop = 0;
    int increment = 0;
    while (increment < 2){
    while (1) {
		for (int i = 0; i < pp.size()-1; i++) {
			draw_line(pp[i].x, pp[i].y, pp[i+1].x, pp[i+1].y, &white);
		}
		draw_line(pp[pp.size()-1].x,pp[pp.size()-1].y,pp[0].x,pp[0].y,&white);
		draw_dot(p1.x,p1.y,&black);
        fil(p1.x,p1.y,0,&green);
		fil(fillPlane.x,fillPlane.y,0,&green);
        fil(fillPlane2.x,fillPlane2.y,0,&green);
		for (int i = 0; i < 30; i++){
          draw_line(p1.x,p1.y,p2.x+i,p2.y+i,&white);    // Baling2
        }

        if (loop == 20) break;
		// clear screen mini
        usleep(50000);
        for (int i = 0; i < 30; i++){
          draw_line(p1.x,p1.y,p2.x+i,p2.y+i,&green);    // Baling2
        }
        if (p2.y > p1.y){
            clear_screen(0,p1.y-30,1366,p2.y+100,&black);
        } else {
            clear_screen(0,p2.y+100,1366,p1.y+30,&black);
        }


		for (int i = 0; i < pp.size()-1; i++) {
			draw_line(pp[i].x, pp[i].y, pp[i+1].x, pp[i+1].y, &black);
		}
        //usleep(50000);
		draw_line(pp[pp.size()-1].x,pp[pp.size()-1].y,pp[0].x,pp[0].y,&black);
		p2 = scalePoint(p1,p2,1.1);
        fillPlane = scalePoint(p3,fillPlane,1.1);
        fillPlane2 = scalePoint(p3,fillPlane2,1.1);
		p2 = rotasi(p1,p2,degreeToRad(20));
		scaleBanyak(p3, pp, 1.1, pp.size());
        for (int i = 0; i < pp.size(); i++) {
            pp[i].x += 10;
        }
        p1.x += 10;
        p2.x += 10;
        fillPlane.x += 10;
        fillPlane2.x += 10;
		loop++;
	}

    while (1) {
		for (int i = 0; i < pp.size()-1; i++) {
			draw_line(pp[i].x, pp[i].y, pp[i+1].x, pp[i+1].y, &white);
		}
		draw_line(pp[pp.size()-1].x,pp[pp.size()-1].y,pp[0].x,pp[0].y,&white);
		draw_dot(p1.x,p1.y,&black);
        fil(p1.x,p1.y,0,&green);
		fil(fillPlane.x,fillPlane.y,0,&green);
        fil(fillPlane2.x,fillPlane2.y,0,&green);
		for (int i = 0; i < 30; i++){
          draw_line(p1.x,p1.y,p2.x+i,p2.y+i,&white);    // Baling2
        }

        if (loop == 10) break;
		// clear screen mini
        usleep(50000);
        for (int i = 0; i < 30; i++){
          draw_line(p1.x,p1.y,p2.x+i,p2.y+i,&green);    // Baling2
        }
        if (p2.y > p1.y){
            clear_screen(0,pp[3].y,1366,p2.y+100,&black);
        } else {
            clear_screen(0,p2.y+100,1366,pp[8].y,&black);
        }


		for (int i = 0; i < pp.size()-1; i++) {
			draw_line(pp[i].x, pp[i].y, pp[i+1].x, pp[i+1].y, &black);
		}
        //usleep(50000);
		draw_line(pp[pp.size()-1].x,pp[pp.size()-1].y,pp[0].x,pp[0].y,&black);
		p2 = scalePoint(p1,p2,0.9);
        fillPlane = scalePoint(p1,fillPlane,0.9);
        fillPlane2 = scalePoint(p1,fillPlane2,0.9);
		p2 = rotasi(p1,p2,degreeToRad(20));
		scaleBanyak(p1, pp, 0.9, pp.size());
		loop--;
	}
    increment++;
}

    return 0;
}
