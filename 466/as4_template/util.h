#ifndef UTIL
#define UTIL
#define EPSILON 0.000001
#define CROSS(dest,v1,v2) \
    dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
    dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
    dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1, v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
    dest[0]=v1[0]-v2[0]; \
    dest[1]=v1[1]-v2[1]; \
    dest[2]=v1[2]-v2[2];

#include "vertex.h"
#include <string>
#include "Camera.h"
#include "scene.h"

using namespace std;

void error_exit(int status, char *text);
char* strToChar(std::string in);
Vertex Transform(float* matrix, Vertex& point);
void matByMat(float mat[16], float mat2[16]);
int intersect_triangle(float orig[3], float dir[3],
                       float vert0[3], float vert1[3], float vert2[3],
                       float *t, float *u, float *v);
int Select(int previous, Scene* pScene, Camera* pCamera, float x, float y);
int PrintOGLError(char *file, int line);
int getUniformVariable(GLuint program,char *name);
void calcUV(float* s, float* t, Vertex norm);
void SphereMap(double x, double y, double z, float *s, float *t);

#endif