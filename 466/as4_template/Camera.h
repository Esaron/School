#ifndef CAMERA_H
#define CAMERA_H

#define PERSPECTIVE 0
#define ORTHOGRAPHIC 1

#include "vector.h"
#include "vertex.h"

class Camera
{
public:
    Camera(int type);
    int type;
    void Move(float tx, float ty, float tz);
    void Pan(float y, float p);
    void ScaleView(float s);
    void MoveView(float d);
    // ADD CODE HERE: functionality to move far/near planes is needed
    void MoveFarPlane(float d);
    void MoveNearPlane(float d);
    bool Contains(Vertex v);
    void EnforceVectors();
    void Perspective();
    void Orthographic();
    void LookAt();
    Vertex Position;
    float Pitch, Yaw;
    Vector n;
    Vector v;
    Vector u;
    float FarPlane, NearPlane, ViewPlane, minDepth;
    float ViewWidth, ViewHeight;
    float ViewingMatrix[16];
    float ProjectionMatrix[16];
    Vertex* Clip(int count, Vertex* input, int* out_count);
    void Reorient();
};

#endif