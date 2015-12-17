#ifndef LIGHT_H
#define LIGHT_H

#include "color.h"
#include "vertex.h"

class Light
{
public:
    int type;
    Vertex location;
    Color color;

    Light() {
        type = 0;
        location = Vertex();
        color = Color();
    }

    Light(int typeIn, float xx, float yy, float zz, float rr, float gg, float bb) {
        type = typeIn;
        location = Vertex(xx, yy, zz);
        color = Color(rr, gg, bb);
    }

    Light(int typeIn, Vertex vertex, float rr, float gg, float bb) {
        type = typeIn;
        location = vertex;
        color = Color(rr, gg, bb);
    }

    Light(int typeIn, int xx, int yy, int zz, Color colorIn) {
        type = typeIn;
        location = Vertex(xx, yy, zz);
        color = colorIn;
    }

    Light(int typeIn, Vertex vertex, Color colorIn) {
        type = typeIn;
        location = vertex;
        color = colorIn;
    }
};

#endif