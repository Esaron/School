#ifndef RAY_H
#define RAY_H

#include "vertex.h"
#include "vector.h"

class Ray {
public:
    Vertex origin;
    Vector direction;

    Ray() {
        origin = Vertex();
        direction = Vector();
    }

    Ray(Vertex originIn, Vector directionIn) {
        origin = originIn;
        direction = directionIn;
    }
};

#endif