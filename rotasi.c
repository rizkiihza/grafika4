#include "rotasi.h"
#include <math.h>

point rotasi(point p1, point p2, int degree) {
    point result;
    double sind = sin(degree);
    double cosd = cos(degree);

    double deltaX = p2.x - p1.x;
    double deltaY = p2.y - p1.y;
    result.x = (deltaX * cosd) - (deltaY * sind) + p1.x;
    result.y = (deltaX * sind) + (deltaY * cosd) + p1.y;

    return result;
}

void rotasibanyak (point p0, point* pp, int degree, int banyak) {
    point* p = pp;
    int i;
    for(i = 0; i < banyak; i++) {
        pp[i] = rotasi(p0, pp[i], degree);
    }
}