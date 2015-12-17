#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

class Vector
{
public:
    float i, j, k;

    Vector() {
        i = j = k = 0;
    }

    Vector(float iIn, float jIn, float kIn) {
        i = iIn;
        j = jIn;
        k = kIn;
    }

    void Normalize() {
        float length = sqrt(pow(i,2)+pow(j,2)+pow(j,2));
        i = i/length;
        j = j/length;
        k = k/length;
    }
};

#endif