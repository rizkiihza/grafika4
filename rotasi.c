#include "rotasi.h"
#include <math.h>

point rotasi(point p1, point p2, int d) {
    point result;
    double sind = sin(d);
    double cosd = cos(d);

    double deltaX = p2.x - p1.x;
    double deltaY = p2.y - p1.y;
    result.x = (deltaX * cosd) - (deltaY * sind) + p1.x;
    result.y = (deltaX * sind) + (deltaY * cosd) + p1.y;

    return result;
}

void rotasibanyak (point p0, point* pp, int d, int l) {
    point* p
}