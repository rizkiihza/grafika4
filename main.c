#include "rotasi.h"
#include <stdio.h>
#include <math.h>

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
    res = rotasi(p1,p2,(M_PI / 4));
    //printf("%f", sin());
    printf("%f %f\n", res.x, res.y);
    return 0;
}