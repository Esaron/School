#ifndef VERTEX_H
#define VERTEX_H

#include "vector.h"

class Vertex
{
public:
    float x, y, z, h;

    Vertex() { x = y = z = 0; h = 1; }
    Vertex(float xx, float yy, float zz) { x = xx; y = yy; z = zz; h = 1.0; }
    Vertex(float xx, float yy, float zz, float hh) { x = xx; y = yy; z = zz; h = hh; }

    void Normalize() {
        x = x/h;
        y = y/h;
        z = z/h;
        h = 1;
    }

    Vector Cross(Vertex v) {
        Vector result;
        result.i = y*v.z-z*v.y;
        result.j = z*v.x-x*v.z;
        result.k = x*v.y-y*v.x;
        return result;
    }

    float Dot(Vertex v) {
        return x*v.x+y*v.y+z*v.z;
    }

    Vertex Sub(Vertex v) {
        Vertex result;
        result.x = x - v.x;
        result.y = y - v.y;
        result.z = z - v.z;
        result.h = h - v.h;
        return result;
    }
};

#endif