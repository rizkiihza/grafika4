#include "scaling.h"
#include <math.h>

point scalePoint(point p1, point p2, double kelipatan) {
   point output;

   int jarakx = abs(p2.x - p1.x);
   int jaraky = abs(p2.y - p1.y);

    if (p1. x < p2.x) {
        output.x = p1.x - jarakx * kelipatan;
    }
    else {
        output.y
    }
}

void scaleArrayPoint(point p1, point *pp, double kelipatan, int banyak)