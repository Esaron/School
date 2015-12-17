#include "util.h"
#include "scene.h"
#include "Camera.h"
#include "glprocs.h"

#define PrintOpenGLError()::PrintOGLError(__FILE__, __LINE__)

#define PI 3.14159265359
#define TWOPI 6.28318530718

using namespace std;

void error_exit(int status, char *text)
{

    // Print error message

    fprintf(stderr,"Internal Error %i: ", status);
    fprintf(stderr,text);
    printf("\nTerminating as Result of Internal Error.\nPress Enter to exit.\n");

    // Keep the terminal open

    int anyKey = getchar();

    // Exit program

    exit(status);
}

char* strToChar(string in) {
    char* out = new char[in.size()+1];
    out[in.size()] = 0;
    memcpy(out, in.c_str(), in.size());
    return out;
}

// Transform a point with an arbitrary matrix
Vertex Transform(float* matrix, Vertex& point)
{
    Vertex temp;
    temp.x = matrix[0]*point.x + matrix[4]*point.y + matrix[8]*point.z + matrix[12]*point.h;
    temp.y = matrix[1]*point.x + matrix[5]*point.y + matrix[9]*point.z + matrix[13]*point.h;
    temp.z = matrix[2]*point.x + matrix[6]*point.y + matrix[10]*point.z + matrix[14]*point.h;
    temp.h = matrix[3]*point.x + matrix[7]*point.y + matrix[11]*point.z + matrix[15]*point.h;
    return temp;
}

void matByMat(float mat[16], float mat2[16]) {
    float newMat[16];
    int i, j, k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            float temp = 0;
            for (k = 0; k < 4; k++) {
                temp += mat[k*4 + i] * mat2[j*4 + k];
            }
            newMat[j*4 + i] = temp;
        }
    }
    memcpy(mat, newMat, 16*sizeof(float));
}

int intersect_triangle(float orig[3], float dir[3],
                       float vert0[3], float vert1[3], float vert2[3],
                       float *t, float *u, float *v) {
                           float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
                           float det, inv_det;

                           // find vectors for two edges sharing vert0
                           SUB(edge1, vert1, vert0);
                           SUB(edge2, vert2, vert0);

                           // begin calculating determinant - also used to calculate mu parameter
                           CROSS(pvec, dir, edge2);

                           // If determinant is near zero, ray lies in plane of triangle
                           det = DOT(edge1, pvec);
                           //printf("det %f\n", det);

                           if (det > -EPSILON && det < EPSILON) {
                               return 0;
                           }
                           inv_det = 1/det;

                           // calculate distance from vert0 to ray origin
                           SUB(tvec, orig, vert0);

                           // calculate u parameter and test bounds
                           *u = DOT(tvec, pvec) * inv_det;
                           //printf("u %f\n", *u);
                           if (*u < 0.0 || *u > 1.0) {
                               return 0;
                           }

                           // prepare to test v parameter
                           CROSS(qvec, tvec, edge1);

                           // calculate v parameter and test bounds
                           *v = DOT(dir, qvec) * inv_det;
                           //printf("v %f\n", *v);
                           if (*v < 0.0 || *u + *v > 1.0) {
                               return 0;
                           }

                           // calculate t, ray intersects triangle
                           *t = DOT(edge2, qvec) * inv_det;
                           //printf("t %f\n", *t);
                           return 1;
}

// Select a new object by intersecting the selection ray with all object faces
int Select(int previous, Scene* pScene, Camera* pCamera, float x, float y)
{
    int select = previous;
    float zp = 10000, z;
    float s, t;
    Vertex temp1, temp2, temp3;

    for(int i = 0; i < pScene->numMeshes; i++)
    {
        for(int j = 0; j < pScene->meshes[i].FaceCount; j++)
        {
            // Project each vertex into the standard view volume
            temp1 = pScene->meshes[i].pVertexList[pScene->meshes[i].pFaceList[j].v1];
            temp1 = Transform(pScene->meshes[i].ModelMatrix, temp1);
            temp1 = Transform(pCamera->ViewingMatrix, temp1);
            temp1 = Transform(pCamera->ProjectionMatrix, temp1);
            temp1.Normalize();

            temp2 = pScene->meshes[i].pVertexList[pScene->meshes[i].pFaceList[j].v2];
            temp2 = Transform(pScene->meshes[i].ModelMatrix, temp2);
            temp2 = Transform(pCamera->ViewingMatrix, temp2);
            temp2 = Transform(pCamera->ProjectionMatrix, temp2);
            temp2.Normalize();

            temp3 = pScene->meshes[i].pVertexList[pScene->meshes[i].pFaceList[j].v3];
            temp3 = Transform(pScene->meshes[i].ModelMatrix, temp3);
            temp3 = Transform(pCamera->ViewingMatrix, temp3);
            temp3 = Transform(pCamera->ProjectionMatrix, temp3);
            temp3.Normalize();

            // Use parametric equations to find the intersection between the selection ray
            // and the current (transformed) face
            t = ((temp1.x - temp3.x)*(y - temp3.y) - (temp1.y - temp3.y)*(x - temp3.x))/
                ((temp1.x - temp3.x)*(temp2.y - temp3.y) + (temp1.y - temp3.y)*(temp3.x - temp2.x));
            if((temp1.x - temp3.x) != 0)
                s = (x - temp3.x + (temp3.x - temp2.x)*t)/(temp1.x - temp3.x);
            else
                s = (y - temp3.y+(temp3.y - temp2.y)*t)/(temp1.y - temp3.y);
            // Use the intersection if its valid (within the triangle) and closer
            if((s >= 0) && (t >= 0) && ((s + t) <= 1))
            {
                z = temp1.z*s + temp2.z*t + (1 - s - t)*temp3.z;
                if(z < zp)
                {
                    zp = z;
                    select = i;
                }
            }
        }
    }

    return select;
}

int PrintOGLError(char *file, int line)
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

//Gets the location of the uniform variable given with "name" in the memory
//and tests whether the process was successfull.
//Returns the location of the queried uniform variable
int getUniformVariable(GLuint program,char *name)
{
    int location = glGetUniformLocationARB(program, name);

    if (location == -1)
    {
        error_exit(1007, "No such uniform variable");
    }
    PrintOpenGLError();
    return location;
}

void SphereMap(double x, double y, double z, float *s, float *t)
{
    *t = acos(-y) / PI;
    if (x <= 0) {
        *s = acos(-z/(sin(PI*(*t)))) / TWOPI;
    }
    else {
        *s = (PI + acos(z/(sin(PI*(*t))))) / TWOPI;
    }
    if (_isnan(*s)) {
        *s = 0.5;
    }
}

void calcUV(float* s, float* t, Vertex norm) {
    SphereMap(norm.x, norm.y, norm.z, s, t);
}