#include "rotasi.h"
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

// inisialisasi struct
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *fbp = 0;

float degreeToRad(float degree) {
    return (degree * M_PI / 180);
}

int main () {
    point p1, p2;
    p1.x = 0;
    p1.y = 0;
    p2.x = 10;
    p2.y = 0;
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
    rotasiBanyak(p1, p, degreeToRad(45), 2);
    printf("%f %f\n", p[0].x, p[0].y);
    printf("%f %f\n", p[1].x, p[1].y);

    // int fbfd = 0;

	// long int screensize = 0;

  	// int x = 0, y = 0;
  	// long int location = 0;

  	// // Open the file for reading and writing
  	// fbfd = open("/dev/fb0", O_RDWR);
  	// if (fbfd == -1) {
  	// 	perror("Error: cannot open framebuffer device");
	// 	exit(1);
	// }

	// printf("The framebuffer device was opened successfully.\n");

	// // Get fixed screen information
	// if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
	// 	perror("Error reading fixed information");
	// 	exit(2);
	// }

	// // Get variable screen information
	// if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
	// 	perror("Error reading variable information");
	// 	exit(3);
	// }

	// printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	// // Figure out the size of the screen in bytes
	// screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	// // Map the device to memory
	// fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
	// 		fbfd, 0);
	// if ((int)fbp == -1) {
	// 	perror("Error: failed to map framebuffer device to memory");
	// 	exit(4);
	// }
	// printf("The framebuffer device was mapped to memory successfully.\n");

    return 0;
}