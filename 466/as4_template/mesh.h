#ifndef MESH_H
#define MESH_H

#include "face.h"
#include "vertex.h"
#include "material.h"
#include "Camera.h"
#include "ShaderHelper.h"
#include "glut.h"

class Scene;

class Mesh
{
public:
    Vertex* pBoundingBox;
    Vertex* pVertexList;
    Vertex* normList;
    int VertexCount;
    int* normCount;
    Face* pFaceList;
    int FaceCount;
    float ModelMatrix[16];
    Material material;
    bool showAxes, showBoundingBox, selectable, selected;
    GLuint polygonModeSide, polygonModeFace, renderType;

    Mesh();
    Mesh(Material materialIn);
    Mesh(char* file, float s, float rx, float ry, float rz, float tx, float ty, float tz, Material materialIn);
    ~Mesh();

    void Load(char* file, float s, float rx, float ry, float rz, float tx, float ty, float tz);
    void WorldTranslate(float tx, float ty, float tz);
    void WorldRotate(float rx, float ry, float rz);
    void LocalRotate(float rx, float ry, float rz);
    void LocalScale(float s);
    void Render(Camera* cam);
    void Render(int gl_render_type, Camera* cam);
    void Render(ShaderHelper sh, Scene* scene, Camera* cam);
    void Render(ShaderHelper sh, Scene* scene, int gl_render_type, Camera* cam);
};

#endif