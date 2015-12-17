#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "mesh.h"
#include "Camera.h"
#include "util.h"
#include "glut.h"
#include "scene.h"
#include "ShaderHelper.h"
#include "glprocs.h"

using namespace std;

Mesh::Mesh() {
    renderType = GL_POLYGON;
    polygonModeSide = GL_FRONT_AND_BACK;
    polygonModeFace = GL_FILL;
    selectable = true;
    selected = false;
    pBoundingBox = new Vertex[8];
    // Load the identity for the initial modeling matrix
    ModelMatrix[0] = ModelMatrix[5] = ModelMatrix[10] = ModelMatrix[15] = 1;
    ModelMatrix[1] = ModelMatrix[2] = ModelMatrix[3] = ModelMatrix[4] =
        ModelMatrix[6] = ModelMatrix[7] = ModelMatrix[8] = ModelMatrix[9] =
        ModelMatrix[11] = ModelMatrix[12] = ModelMatrix[13] = ModelMatrix[14]= 0;
}

Mesh::Mesh(Material materialIn) {
    renderType = GL_POLYGON;
    polygonModeSide = GL_FRONT_AND_BACK;
    polygonModeFace = GL_FILL;
    selectable = true;
    selected = false;
    pBoundingBox = new Vertex[8];
    // Load the identity for the initial modeling matrix
    ModelMatrix[0] = ModelMatrix[5] = ModelMatrix[10] = ModelMatrix[15] = 1;
    ModelMatrix[1] = ModelMatrix[2] = ModelMatrix[3] = ModelMatrix[4] =
        ModelMatrix[6] = ModelMatrix[7] = ModelMatrix[8] = ModelMatrix[9] =
        ModelMatrix[11] = ModelMatrix[12] = ModelMatrix[13] = ModelMatrix[14]= 0;
    material = materialIn;
}

Mesh::Mesh(char* file, float s, float rx, float ry, float rz, float tx, float ty, float tz, Material materialIn) {
    renderType = GL_POLYGON;
    polygonModeSide = GL_FRONT_AND_BACK;
    polygonModeFace = GL_FILL;
    selectable = true;
    selected = false;
    pBoundingBox = new Vertex[8];
    // Load the identity for the initial modeling matrix
    ModelMatrix[0] = ModelMatrix[5] = ModelMatrix[10] = ModelMatrix[15] = 1;
    ModelMatrix[1] = ModelMatrix[2] = ModelMatrix[3] = ModelMatrix[4] =
        ModelMatrix[6] = ModelMatrix[7] = ModelMatrix[8] = ModelMatrix[9] =
        ModelMatrix[11] = ModelMatrix[12] = ModelMatrix[13] = ModelMatrix[14]= 0;
    Load(file, s, rx, ry, rz, tx, ty, tz);
    material = materialIn;
}

Mesh::~Mesh() {
    delete [] pBoundingBox;
    delete [] pVertexList;
    delete [] pFaceList;
}

void Mesh::Load(char* file, float s, float rx, float ry, float rz, float tx, float ty, float tz) {
    int *normCount, i;
    Vertex v1,v2,crossP;
    float len;
    FILE* pMeshFile = fopen(file, "r");
    if(!pMeshFile)
        cout << "Failed to load " << file << "." << endl;
    else
        cout << "Successfully loaded " << file << "." << endl;

    char DataType;
    float a, b, c;

    // Scan the file and count the faces and vertices
    VertexCount = FaceCount = 0;
    while(!feof(pMeshFile))
    {
        fscanf(pMeshFile, "%c %f %f %f\n", &DataType, &a, &b, &c);
        if(DataType == 'v')
            VertexCount++;
        else if(DataType == 'f')
            FaceCount++;
    }
    pVertexList = new Vertex[VertexCount];
    pFaceList = new Face[FaceCount];
    normCount = (int *)malloc(sizeof(int)*VertexCount);
    normList = (Vertex *)malloc(sizeof(Vertex)*VertexCount);

    fseek(pMeshFile, 0L, SEEK_SET);

    cout << "Number of vertices: " << VertexCount << endl;
    cout << "Number of faces: " << FaceCount << endl;

    // Load and create the faces and vertices
    int CurrentVertex = 0, CurrentFace = 0;
    float MinimumX = 0, MaximumX = 0, MinimumY = 0, MaximumY = 0, MinimumZ = 0, MaximumZ = 0;
    while(!feof(pMeshFile))
    {
        fscanf(pMeshFile, "%c %f %f %f\n", &DataType, &a, &b, &c);
        if(DataType == 'v')
        {
            pVertexList[CurrentVertex].x = a;
            pVertexList[CurrentVertex].y = b;
            pVertexList[CurrentVertex].z = c;

            //ADD YOUR CODE HERE :Track maximum and minimum coordinates for use in bounding boxes
            if (MinimumX == 0 || a < MinimumX) {
                MinimumX = a;
            }
            if (MinimumY == 0 || b < MinimumY) {
                MinimumY = b;
            }
            if (MinimumZ == 0 || c < MinimumZ) {
                MinimumZ = c;
            }
            if (MaximumX == 0 || a > MaximumX) {
                MaximumX = a;
            }
            if (MaximumY == 0 || b > MaximumY) {
                MaximumY = b;
            }
            if (MaximumZ == 0 || c > MaximumZ) {
                MaximumZ = c;
            }

            CurrentVertex++;
        }
        else if(DataType == 'f')
        {
            // Convert to a zero-based index for convenience
            pFaceList[CurrentFace].v1 = (int)a - 1;
            pFaceList[CurrentFace].v2 = (int)b - 1;
            pFaceList[CurrentFace].v3 = (int)c - 1;
            CurrentFace++;
        }
    }

    //ADD YOUR CODE HERE: Initialize the bounding box vertices
    // 0 to 1, 2, 4
    // 1 to 0, 3, 5
    // 2 to 0, 3, 6
    // 3 to 1, 2, 7
    // 4 to 0, 5, 6
    // 5 to 1, 4, 7
    // 6 to 2, 4, 7
    // 7 to 3, 5, 6
    pBoundingBox[0].x = MinimumX;
    pBoundingBox[0].y = MinimumY;
    pBoundingBox[0].z = MinimumZ;
    pBoundingBox[0].h = 1;
    pBoundingBox[1].x = MinimumX;
    pBoundingBox[1].y = MaximumY;
    pBoundingBox[1].z = MinimumZ;
    pBoundingBox[1].h = 1;
    pBoundingBox[2].x = MinimumX;
    pBoundingBox[2].y = MinimumY;
    pBoundingBox[2].z = MaximumZ;
    pBoundingBox[2].h = 1;
    pBoundingBox[3].x = MinimumX;
    pBoundingBox[3].y = MaximumY;
    pBoundingBox[3].z = MaximumZ;
    pBoundingBox[3].h = 1;
    pBoundingBox[4].x = MaximumX;
    pBoundingBox[4].y = MinimumY;
    pBoundingBox[4].z = MinimumZ;
    pBoundingBox[4].h = 1;
    pBoundingBox[5].x = MaximumX;
    pBoundingBox[5].y = MaximumY;
    pBoundingBox[5].z = MinimumZ;
    pBoundingBox[5].h = 1;
    pBoundingBox[6].x = MaximumX;
    pBoundingBox[6].y = MinimumY;
    pBoundingBox[6].z = MaximumZ;
    pBoundingBox[6].h = 1;
    pBoundingBox[7].x = MaximumX;
    pBoundingBox[7].y = MaximumY;
    pBoundingBox[7].z = MaximumZ;
    pBoundingBox[7].h = 1;

    // Apply the initial transformations in order
    LocalScale(s);
    WorldRotate((float)(M_PI*rx/180.0), (float)(M_PI*ry/180.0), (float)(M_PI*rz/180.0));
    WorldTranslate(tx, ty, tz);	

    // The part below calculates the normals of each vertex
    for (i = 0;i < VertexCount;i++)
    {
        normList[i].x = normList[i].y = normList[i].z = 0.0;
        normCount[i] = 0;
    }

    for(i = 0;i < FaceCount;i++)
    {
        v1.x = pVertexList[pFaceList[i].v2].x - pVertexList[pFaceList[i].v1].x;
        v1.y = pVertexList[pFaceList[i].v2].y - pVertexList[pFaceList[i].v1].y;
        v1.z = pVertexList[pFaceList[i].v2].z - pVertexList[pFaceList[i].v1].z;
        v2.x = pVertexList[pFaceList[i].v3].x - pVertexList[pFaceList[i].v2].x;
        v2.y = pVertexList[pFaceList[i].v3].y - pVertexList[pFaceList[i].v2].y;
        v2.z = pVertexList[pFaceList[i].v3].z - pVertexList[pFaceList[i].v2].z;

        crossP.x = v1.y*v2.z - v1.z*v2.y;
        crossP.y = v1.z*v2.x - v1.x*v2.z;
        crossP.z = v1.x*v2.y - v1.y*v2.x;

        len = sqrt(crossP.x*crossP.x + crossP.y*crossP.y + crossP.z*crossP.z);

        crossP.x = -crossP.x/len;
        crossP.y = -crossP.y/len;
        crossP.z = -crossP.z/len;

        normList[pFaceList[i].v1].x = normList[pFaceList[i].v1].x + crossP.x;
        normList[pFaceList[i].v1].y = normList[pFaceList[i].v1].y + crossP.y;
        normList[pFaceList[i].v1].z = normList[pFaceList[i].v1].z + crossP.z;
        normList[pFaceList[i].v2].x = normList[pFaceList[i].v2].x + crossP.x;
        normList[pFaceList[i].v2].y = normList[pFaceList[i].v2].y + crossP.y;
        normList[pFaceList[i].v2].z = normList[pFaceList[i].v2].z + crossP.z;
        normList[pFaceList[i].v3].x = normList[pFaceList[i].v3].x + crossP.x;
        normList[pFaceList[i].v3].y = normList[pFaceList[i].v3].y + crossP.y;
        normList[pFaceList[i].v3].z = normList[pFaceList[i].v3].z + crossP.z;
        normCount[pFaceList[i].v1]++;
        normCount[pFaceList[i].v2]++;
        normCount[pFaceList[i].v3]++;
    }
    for (i = 0;i < VertexCount;i++)
    {
        int sign = -1;
        normList[i].x = (float)sign*normList[i].x / (float)normCount[i];
        normList[i].y = (float)sign*normList[i].y / (float)normCount[i];
        normList[i].z = (float)sign*normList[i].z / (float)normCount[i];
    }
}

void Mesh::WorldTranslate(float tx, float ty, float tz) {
    ModelMatrix[12] += tx;
    ModelMatrix[13] += ty;
    ModelMatrix[14] += tz;
}

void Mesh::WorldRotate(float rx, float ry, float rz) {
    float temp[16];

    if(rx != 0)
    {
        float cosx = cos(rx), sinx = sin(rx);
        for(int i = 0; i < 16; i++)
            temp[i] = ModelMatrix[i];
        ModelMatrix[1] = temp[1]*cosx - temp[2]*sinx;
        ModelMatrix[2] = temp[2]*cosx + temp[1]*sinx;
        ModelMatrix[5] = temp[5]*cosx - temp[6]*sinx;
        ModelMatrix[6] = temp[6]*cosx + temp[5]*sinx;
        ModelMatrix[9] = temp[9]*cosx - temp[10]*sinx;
        ModelMatrix[10] = temp[10]*cosx + temp[9]*sinx;
        ModelMatrix[13] = temp[13]*cosx - temp[14]*sinx;
        ModelMatrix[14] = temp[14]*cosx + temp[13]*sinx;
    }

    if(ry != 0)
    {
        float cosy = cos(ry), siny = sin(ry);
        for(int i = 0; i < 16; i++)
            temp[i] = ModelMatrix[i];
        ModelMatrix[0] = temp[0]*cosy + temp[2]*siny;
        ModelMatrix[2] = temp[2]*cosy - temp[0]*siny;
        ModelMatrix[4] = temp[4]*cosy + temp[6]*siny;
        ModelMatrix[6] = temp[6]*cosy - temp[4]*siny;
        ModelMatrix[8] = temp[8]*cosy + temp[10]*siny;
        ModelMatrix[10] = temp[10]*cosy - temp[8]*siny;
        ModelMatrix[12] = temp[12]*cosy + temp[14]*siny;
        ModelMatrix[14] = temp[14]*cosy - temp[12]*siny;
    }

    if(rz != 0)
    {
        float cosz = cos(rz), sinz = sin(rz);
        for(int i = 0; i < 16; i++)
            temp[i] = ModelMatrix[i];
        ModelMatrix[0] = temp[0]*cosz - temp[1]*sinz;
        ModelMatrix[1] = temp[1]*cosz + temp[0]*sinz;
        ModelMatrix[4] = temp[4]*cosz - temp[5]*sinz;
        ModelMatrix[5] = temp[5]*cosz + temp[4]*sinz;
        ModelMatrix[8] = temp[8]*cosz - temp[9]*sinz;
        ModelMatrix[9] = temp[9]*cosz + temp[8]*sinz;
        ModelMatrix[12] = temp[12]*cosz - temp[13]*sinz;
        ModelMatrix[13] = temp[13]*cosz + temp[12]*sinz;
    }
}

void Mesh::LocalRotate(float rx, float ry, float rz) {
    float temp[16];

    if(rx != 0)
    {
        float cosx = cos(rx), sinx = sin(rx);
        for(int i = 0; i < 16; i++)
            temp[i] = ModelMatrix[i];
        ModelMatrix[4] = temp[4]*cosx + temp[8]*sinx;
        ModelMatrix[5] = temp[5]*cosx + temp[9]*sinx;
        ModelMatrix[6] = temp[6]*cosx + temp[10]*sinx;
        ModelMatrix[7] = temp[7]*cosx + temp[11]*sinx;
        ModelMatrix[8] = temp[8]*cosx - temp[4]*sinx;
        ModelMatrix[9] = temp[9]*cosx - temp[5]*sinx;
        ModelMatrix[10] = temp[10]*cosx - temp[6]*sinx;
        ModelMatrix[11] = temp[11]*cosx - temp[7]*sinx;
    }

    if(ry != 0)
    {
        float cosy = cos(ry), siny = sin(ry);
        for(int i = 0; i < 16; i++)
            temp[i] = ModelMatrix[i];
        ModelMatrix[0] = temp[0]*cosy - temp[8]*siny;
        ModelMatrix[1] = temp[1]*cosy - temp[9]*siny;
        ModelMatrix[2] = temp[2]*cosy - temp[10]*siny;
        ModelMatrix[3] = temp[3]*cosy - temp[11]*siny;
        ModelMatrix[8] = temp[8]*cosy + temp[0]*siny;
        ModelMatrix[9] = temp[9]*cosy + temp[1]*siny;
        ModelMatrix[10] = temp[10]*cosy + temp[2]*siny;
        ModelMatrix[11] = temp[11]*cosy + temp[3]*siny;
    }

    if(rz != 0)
    {
        float cosz = cos(rz), sinz = sin(rz);
        for(int i = 0; i < 16; i++)
            temp[i] = ModelMatrix[i];
        ModelMatrix[0] = temp[0]*cosz + temp[4]*sinz;
        ModelMatrix[1] = temp[1]*cosz + temp[5]*sinz;
        ModelMatrix[2] = temp[2]*cosz + temp[6]*sinz;
        ModelMatrix[3] = temp[3]*cosz + temp[7]*sinz;
        ModelMatrix[4] = temp[4]*cosz - temp[0]*sinz;
        ModelMatrix[5] = temp[5]*cosz - temp[1]*sinz;
        ModelMatrix[6] = temp[6]*cosz - temp[2]*sinz;
        ModelMatrix[7] = temp[7]*cosz - temp[3]*sinz;
    }
}

void Mesh::LocalScale(float s) {
    for(int i = 0; i <= 11; i++)
        ModelMatrix[i] = s*ModelMatrix[i];
}

void Mesh::Render(Camera* pDisplayCamera) {
    Render(renderType, pDisplayCamera);
}

void Mesh::Render(int gl_render_type, Camera* pDisplayCamera) {
    glPolygonMode(polygonModeSide, polygonModeFace);
    int length;
    Vertex* input;
    Vertex* output;
    Vertex temp,temp1,temp2,temp3;
    Vertex orig,xunit,yunit,zunit;
    bool allOutNear = true, allOutFar = true;

    // Draw object coordinate frames
    if(showAxes)
    {
        glLineWidth(3.0);
        glBegin(GL_LINES);

        temp = Transform(ModelMatrix,orig);
        temp1= Transform(pDisplayCamera->ViewingMatrix,temp);
        temp2 = Transform(pDisplayCamera->ProjectionMatrix,temp1);
        temp = Transform(ModelMatrix,xunit);
        temp1= Transform(pDisplayCamera->ViewingMatrix,temp);
        temp3= Transform(pDisplayCamera->ProjectionMatrix,temp1);
        glColor3f(1, 0, 0);
        glVertex2f(temp2.x/temp2.h, temp2.y/temp2.h);
        glVertex2f(temp3.x/temp3.h, temp3.y/temp3.h);

        temp = Transform(ModelMatrix,yunit);
        temp1= Transform(pDisplayCamera->ViewingMatrix,temp);
        temp3= Transform(pDisplayCamera->ProjectionMatrix,temp1);
        glColor3f(1, 0, 0);
        glVertex2f(temp2.x/temp2.h, temp2.y/temp2.h);
        glVertex2f(temp3.x/temp3.h, temp3.y/temp3.h);

        temp = Transform(ModelMatrix,zunit);
        temp1= Transform(pDisplayCamera->ViewingMatrix,temp);
        temp3= Transform(pDisplayCamera->ProjectionMatrix,temp1);
        glColor3f(1, 0, 0);
        glVertex2f(temp2.x/temp2.h, temp2.y/temp2.h);
        glVertex2f(temp3.x/temp3.h, temp3.y/temp3.h);
        glEnd();
        glLineWidth(1.0);
    }
    input = new Vertex[8];
    for (int k=0; k<8; k++){
        input[k] = pBoundingBox[k];
    }

    for (int k=0; k<8; k++){
        temp	= Transform(ModelMatrix,input[k]);
        temp2	= Transform(pDisplayCamera->ViewingMatrix,temp);
        input[k]= Transform(pDisplayCamera->ProjectionMatrix,temp2);
        if (input[k].z >= pDisplayCamera->NearPlane) {
            allOutNear = false;
        }
        if (input[k].z <= pDisplayCamera->FarPlane) {
            allOutFar = false;
        }
    }


    if(showBoundingBox)
    {
        glLineWidth(1.5);
        glBegin(GL_LINES);
        if (selected) {
            glColor3f(1, 0, 0);
        }
        else {
            glColor3f(0, 0, 1);
        }
        // 0 to 1, 2, 4
        // 1 to 3, 5
        // 2 to 3, 6
        // 3 to 7
        // 4 to 5, 6
        // 5 to 7
        // 6 to 7

        glVertex2f(input[0].x/input[0].h, input[0].y/input[0].h);
        glVertex2f(input[1].x/input[1].h, input[1].y/input[1].h);

        glVertex2f(input[0].x/input[0].h, input[0].y/input[0].h);
        glVertex2f(input[2].x/input[2].h, input[2].y/input[2].h);

        glVertex2f(input[0].x/input[0].h, input[0].y/input[0].h);
        glVertex2f(input[4].x/input[4].h, input[4].y/input[4].h);

        glVertex2f(input[1].x/input[1].h, input[1].y/input[1].h);
        glVertex2f(input[3].x/input[3].h, input[3].y/input[3].h);

        glVertex2f(input[1].x/input[1].h, input[1].y/input[1].h);
        glVertex2f(input[5].x/input[5].h, input[5].y/input[5].h);

        glVertex2f(input[2].x/input[2].h, input[2].y/input[2].h);
        glVertex2f(input[3].x/input[3].h, input[3].y/input[3].h);

        glVertex2f(input[2].x/input[2].h, input[2].y/input[2].h);
        glVertex2f(input[6].x/input[6].h, input[6].y/input[6].h);

        glVertex2f(input[3].x/input[3].h, input[3].y/input[3].h);
        glVertex2f(input[7].x/input[7].h, input[7].y/input[7].h);

        glVertex2f(input[4].x/input[4].h, input[4].y/input[4].h);
        glVertex2f(input[5].x/input[5].h, input[5].y/input[5].h);

        glVertex2f(input[4].x/input[4].h, input[4].y/input[4].h);
        glVertex2f(input[6].x/input[6].h, input[6].y/input[6].h);

        glVertex2f(input[5].x/input[5].h, input[5].y/input[5].h);
        glVertex2f(input[7].x/input[7].h, input[7].y/input[7].h);

        glVertex2f(input[6].x/input[6].h, input[6].y/input[6].h);
        glVertex2f(input[7].x/input[7].h, input[7].y/input[7].h);
        glEnd();
        glLineWidth(1.0);

        delete [] input;
        input = NULL;
    }
    if (!(allOutNear && allOutFar)) {
        // Color the selected object yellow and others blue
        if(selected)
            glColor3f(material.ambient.r + material.diffuse.r + material.specular.r,
            material.ambient.g + material.diffuse.g + material.specular.g,
            material.ambient.b + material.diffuse.b + material.specular.b);
        else
            glColor3f(material.ambient.r + material.diffuse.r,
            material.ambient.g + material.diffuse.g,
            material.ambient.b + material.diffuse.b);
        // Clip and draw object faces
        for(int j = 0; j < FaceCount; j++)
        {
            input = new Vertex[3];
            input[0] = pVertexList[pFaceList[j].v1];
            input[1] = pVertexList[pFaceList[j].v2];
            input[2] = pVertexList[pFaceList[j].v3];

            for (int k=0; k<3; k++){
                temp	= Transform(ModelMatrix,input[k]);
                temp2	= Transform(pDisplayCamera->ViewingMatrix,temp);
                input[k]= Transform(pDisplayCamera->ProjectionMatrix,temp2);
            }

            output = pDisplayCamera->Clip(3, input, &length);

            glBegin(gl_render_type);
            for(int k = 0; k < length; k++) {
                glVertex2f(output[k].x/output[k].h, output[k].y/output[k].h);
            }
            glEnd();

            delete [] input;
            input = NULL;
            delete [] output;
            output = NULL;
        }
    }
}

void Mesh::Render(ShaderHelper sh, Scene* scene, Camera* pDisplayCamera) {
    Render(sh, scene, renderType, pDisplayCamera);
}

void Mesh::Render(ShaderHelper sh, Scene* scene, int gl_render_type, Camera* pDisplayCamera) {
    glPolygonMode(polygonModeSide, polygonModeFace);
    int length;
    Vertex* input;
    Vertex* output;
    Vertex temp,temp1,temp2,temp3;
    Vertex orig,xunit,yunit,zunit;
    bool allOutNear = true, allOutFar = true;
    int posLocation = glGetAttribLocationARB(sh.p, "pos");

    // Draw object coordinate frames
    if(showAxes)
    {
        glLineWidth(3.0);
        glBegin(GL_LINES);

        temp = Transform(ModelMatrix,orig);
        temp1= Transform(pDisplayCamera->ViewingMatrix,temp);
        temp2 = Transform(pDisplayCamera->ProjectionMatrix,temp1);
        temp = Transform(ModelMatrix,xunit);
        temp1= Transform(pDisplayCamera->ViewingMatrix,temp);
        temp3= Transform(pDisplayCamera->ProjectionMatrix,temp1);
        glColor3f(1, 0, 0);
        glVertex2f(temp2.x/temp2.h, temp2.y/temp2.h);
        glVertex2f(temp3.x/temp3.h, temp3.y/temp3.h);

        temp = Transform(ModelMatrix,yunit);
        temp1= Transform(pDisplayCamera->ViewingMatrix,temp);
        temp3= Transform(pDisplayCamera->ProjectionMatrix,temp1);
        glColor3f(1, 0, 0);
        glVertex2f(temp2.x/temp2.h, temp2.y/temp2.h);
        glVertex2f(temp3.x/temp3.h, temp3.y/temp3.h);

        temp = Transform(ModelMatrix,zunit);
        temp1= Transform(pDisplayCamera->ViewingMatrix,temp);
        temp3= Transform(pDisplayCamera->ProjectionMatrix,temp1);
        glColor3f(1, 0, 0);
        glVertex2f(temp2.x/temp2.h, temp2.y/temp2.h);
        glVertex2f(temp3.x/temp3.h, temp3.y/temp3.h);
        glEnd();
        glLineWidth(1.0);
    }
    input = new Vertex[8];
    for (int k=0; k<8; k++){
        input[k] = pBoundingBox[k];
    }

    for (int k=0; k<8; k++){
        temp	= Transform(ModelMatrix,input[k]);
        temp2	= Transform(pDisplayCamera->ViewingMatrix,temp);
        input[k]= Transform(pDisplayCamera->ProjectionMatrix,temp2);
        if (input[k].z >= pDisplayCamera->NearPlane) {
            allOutNear = false;
        }
        if (input[k].z <= pDisplayCamera->FarPlane) {
            allOutFar = false;
        }
    }


    if(showBoundingBox)
    {
        glLineWidth(1.5);
        glBegin(GL_LINES);
        if (selected) {
            glColor3f(1, 0, 0);
        }
        else {
            glColor3f(0, 0, 1);
        }
        // 0 to 1, 2, 4
        // 1 to 3, 5
        // 2 to 3, 6
        // 3 to 7
        // 4 to 5, 6
        // 5 to 7
        // 6 to 7

        glVertex2f(input[0].x/input[0].h, input[0].y/input[0].h);
        glVertex2f(input[1].x/input[1].h, input[1].y/input[1].h);

        glVertex2f(input[0].x/input[0].h, input[0].y/input[0].h);
        glVertex2f(input[2].x/input[2].h, input[2].y/input[2].h);

        glVertex2f(input[0].x/input[0].h, input[0].y/input[0].h);
        glVertex2f(input[4].x/input[4].h, input[4].y/input[4].h);

        glVertex2f(input[1].x/input[1].h, input[1].y/input[1].h);
        glVertex2f(input[3].x/input[3].h, input[3].y/input[3].h);

        glVertex2f(input[1].x/input[1].h, input[1].y/input[1].h);
        glVertex2f(input[5].x/input[5].h, input[5].y/input[5].h);

        glVertex2f(input[2].x/input[2].h, input[2].y/input[2].h);
        glVertex2f(input[3].x/input[3].h, input[3].y/input[3].h);

        glVertex2f(input[2].x/input[2].h, input[2].y/input[2].h);
        glVertex2f(input[6].x/input[6].h, input[6].y/input[6].h);

        glVertex2f(input[3].x/input[3].h, input[3].y/input[3].h);
        glVertex2f(input[7].x/input[7].h, input[7].y/input[7].h);

        glVertex2f(input[4].x/input[4].h, input[4].y/input[4].h);
        glVertex2f(input[5].x/input[5].h, input[5].y/input[5].h);

        glVertex2f(input[4].x/input[4].h, input[4].y/input[4].h);
        glVertex2f(input[6].x/input[6].h, input[6].y/input[6].h);

        glVertex2f(input[5].x/input[5].h, input[5].y/input[5].h);
        glVertex2f(input[7].x/input[7].h, input[7].y/input[7].h);

        glVertex2f(input[6].x/input[6].h, input[6].y/input[6].h);
        glVertex2f(input[7].x/input[7].h, input[7].y/input[7].h);
        glEnd();
        glLineWidth(1.0);

        delete [] input;
        input = NULL;
    }
    if (!(allOutNear && allOutFar)) {
        // Color the selected object yellow and others blue
        sh.setParameters(scene, this);
        // Clip and draw object faces
        for(int j = 0; j < FaceCount; j++)
        {
            input = new Vertex[3];
            input[0] = pVertexList[pFaceList[j].v1];
            input[1] = pVertexList[pFaceList[j].v2];
            input[2] = pVertexList[pFaceList[j].v3];

            Vertex normal[3];
            normal[0] = normList[pFaceList[j].v1];
            normal[1] = normList[pFaceList[j].v2];
            normal[2] = normList[pFaceList[j].v3];

            Vertex mvpos[3];

            for (int k=0; k<3; k++){
                temp	= Transform(ModelMatrix,input[k]);
                temp2	= Transform(pDisplayCamera->ViewingMatrix,temp);
                mvpos[k] = temp2;
                input[k]= Transform(pDisplayCamera->ProjectionMatrix,temp2);
                temp = Transform(ModelMatrix,normal[k]);
                normal[k] = Transform(pDisplayCamera->ViewingMatrix,temp);
            }

            output = pDisplayCamera->Clip(3, input, &length);

            glBegin(gl_render_type);
            for(int k = 0; k < length; k++) {
                glVertex2f(output[k].x/output[k].h, output[k].y/output[k].h);
                glVertexAttrib3fARB(posLocation, mvpos[k].x, mvpos[k].y, mvpos[k].z);
                glNormal3f(normal[k].x, normal[k].y, normal[k].z);
            }
            glEnd();

            delete [] input;
            input = NULL;
            delete [] output;
            output = NULL;
        }
    }
}