#include "vertex.h"

class Triangle
{
public:
    float a[3];
    float b[3];
    float c[3];

    Triangle() {}

    Triangle(float vert0[3], float vert1[3], float vert2[3]) {
        memcpy(a, vert0, 3*sizeof(float));
        memcpy(b, vert1, 3*sizeof(float));
        memcpy(c, vert2, 3*sizeof(float));
    }
};