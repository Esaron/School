// Project
// Computer Graphics - EECS 466 - Fall 2013

// Allow use of M_PI constant
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "glut.h"
#include "Camera.h"
#include "face.h"
#include "mesh.h"
#include "scene.h"
#include "util.h"
#include "GLConfig.h"
#include "ShaderHelper.h"
#include "glprocs.h"

using namespace std;

// User Interface Variables
int MouseX = 0;
int MouseY = 0;
bool MouseLeft = false;
bool MouseMiddle = false;
bool MouseRight = false;
bool PerspectiveMode = true;
bool ShowAxes = true;
bool SelectionMode = false;
int SelectedObject = -1;
bool ShowBoundingBoxes = true;
int WindowWidth = 300, WindowHeight = 300;

// Scene Content Variables
std::vector<Scene*> scenes;
int activeScene = 0;

void ShaderHelper::setParameters(Scene* scene, Mesh* mesh) {
    int ambientLocation, diffuseLocation, specularLocation;
    int exponentLocation, kaLocation, kdLocation, ksLocation;

    ambientLocation = getUniformVariable(this->p, "ambient");
    glUniform3fARB(ambientLocation, mesh->material.ambient.r, mesh->material.ambient.g, mesh->material.ambient.b);

    diffuseLocation = getUniformVariable(this->p, "diffuse");
    glUniform3fARB(diffuseLocation, mesh->material.diffuse.r, mesh->material.diffuse.g, mesh->material.diffuse.b);

    specularLocation = getUniformVariable(this->p, "specular");
    glUniform3fARB(specularLocation, mesh->material.specular.r, mesh->material.specular.g, mesh->material.specular.b);

    exponentLocation = getUniformVariable(this->p, "exponent");
    glUniform1fARB(exponentLocation, mesh->material.specularExponent);

    kaLocation = getUniformVariable(this->p, "ka");
    glUniform1fARB(kaLocation, mesh->material.kAmbient);

    kdLocation = getUniformVariable(this->p, "kd");
    glUniform1fARB(kdLocation, mesh->material.kDiffuse);

    ksLocation = getUniformVariable(this->p, "ks");
    glUniform1fARB(ksLocation, mesh->material.kSpecular);

}

void DisplayFunc()
{
    int length;
    Vertex* input;
    Vertex* output;
    Vertex temp,temp1,temp2,temp3;
    Vertex orig,xunit,yunit,zunit;

    // vertices to be used in drawing the coordiante axes
    orig.x=orig.y=orig.z=xunit.y=xunit.z=yunit.x=yunit.z=zunit.x=zunit.y=0.0;
    orig.h=xunit.x=xunit.h=yunit.y=yunit.h=zunit.z=zunit.h=1.0;

    scenes[activeScene]->getActiveCamera()->Reorient();

    for (int i = 0; i < scenes[activeScene]->numMeshes; i++) {
        scenes[activeScene]->meshes[i].showBoundingBox = ShowBoundingBoxes;
        scenes[activeScene]->meshes[i].showAxes = ShowAxes;
        scenes[activeScene]->meshes[i].selected = false;
    }
    if (scenes[activeScene]->meshes[SelectedObject].selectable) {
        scenes[activeScene]->meshes[SelectedObject].selected = true;
    }
    else {
        SelectedObject = -1;
    }

    // draw worlc coordinate frames
    if(ShowAxes)
    {
        glLineWidth(3.0);
        glBegin(GL_LINES);

        temp = Transform(scenes[activeScene]->getActiveCamera()->ViewingMatrix,orig);
        temp1 = Transform(scenes[activeScene]->getActiveCamera()->ProjectionMatrix,temp);
        temp = Transform(scenes[activeScene]->getActiveCamera()->ViewingMatrix,xunit);
        temp2= Transform(scenes[activeScene]->getActiveCamera()->ProjectionMatrix,temp);
        glColor3f(1, 0, 0);
        glVertex2f(temp1.x/temp1.h, temp1.y/temp1.h);
        glVertex2f(temp2.x/temp2.h, temp2.y/temp2.h);

        temp = Transform(scenes[activeScene]->getActiveCamera()->ViewingMatrix,yunit);
        temp2= Transform(scenes[activeScene]->getActiveCamera()->ProjectionMatrix,temp);
        glColor3f(0, 1, 0);
        glVertex2f(temp1.x/temp1.h, temp1.y/temp1.h);
        glVertex2f(temp2.x/temp2.h, temp2.y/temp2.h);

        temp = Transform(scenes[activeScene]->getActiveCamera()->ViewingMatrix,zunit);
        temp2= Transform(scenes[activeScene]->getActiveCamera()->ProjectionMatrix,temp);
        glColor3f(0, 0, 1);
        glVertex2f(temp1.x/temp1.h, temp1.y/temp1.h);
        glVertex2f(temp2.x/temp2.h, temp2.y/temp2.h);
        glEnd();
        glLineWidth(1.0);
    }

    scenes[activeScene]->meshes[0].polygonModeFace = GL_LINE;
    scenes[activeScene]->RenderAll();

    glutSwapBuffers();
}

void ReshapeFunc(int x,int y)
{
    // Get the correct view proportionality for the new window size
    scenes[activeScene]->getActiveCamera()->ViewWidth = scenes[activeScene]->getActiveCamera()->ViewWidth*((float)x/WindowWidth);
    scenes[activeScene]->getActiveCamera()->ViewHeight = scenes[activeScene]->getActiveCamera()->ViewHeight*((float)y/WindowHeight);
    glViewport(0,0,x,y);
    WindowWidth = x;
    WindowHeight = y;
}

void MouseFunc(int button,int state,int x,int y)
{
    MouseX = x;
    MouseY = y;

    if(button == GLUT_LEFT_BUTTON)
        MouseLeft = !state;
    if(button == GLUT_MIDDLE_BUTTON)
        MouseMiddle = !state;
    if(button == GLUT_RIGHT_BUTTON)
        MouseRight = !state;

    if(MouseLeft && SelectionMode)
    {
        // Select a new object with (x,y) mapped back to the (-1.1,1.1)x(-1.1,1.1) view volume
        SelectedObject = Select(SelectedObject, scenes[activeScene], scenes[activeScene]->getActiveCamera(),
            2.2*(float)x/WindowWidth - 1.1, 1.1 - 2.2*(float)y/WindowHeight);
        glutPostRedisplay();
    }
}

void MotionFunc(int x, int y)
{
    if(MouseLeft && !SelectionMode)
        scenes[activeScene]->getActiveCamera()->Pan(((float) x - MouseX)/128, ((float) y - MouseY)/128);
    if(MouseMiddle && !SelectionMode)
        scenes[activeScene]->getActiveCamera()->Move(((float) MouseX - x)/32, ((float) y - MouseY)/32, 0);
    if(MouseRight && !SelectionMode)
        scenes[activeScene]->getActiveCamera()->Move(0, 0, ((float) y - MouseY)/32);

    if(MouseMiddle && SelectionMode)
    {
        // Move the Near Plane
        // ADD CODE HERE
        scenes[activeScene]->getActiveCamera()->MoveNearPlane(((float) y - MouseY)/32);
    }
    if(MouseRight && SelectionMode)
    {
        // Move the Far Plane
        // ADD CODE HERE
        scenes[activeScene]->getActiveCamera()->MoveFarPlane(((float) y - MouseY)/32);
    }

    MouseX = x;
    MouseY = y;

    glutPostRedisplay();
}

void KeyboardFunc(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'O':
    case 'o':
        scenes[activeScene]->meshes[SelectedObject].LocalRotate(M_PI/32, 0, 0);
        break;
    case 'I':
    case 'i':
        scenes[activeScene]->meshes[SelectedObject].LocalRotate(-M_PI/32, 0, 0);
        break;
    case 'L':
    case 'l':
        scenes[activeScene]->meshes[SelectedObject].LocalRotate(0, M_PI/32, 0);
        break;
    case 'K':
    case 'k':
        scenes[activeScene]->meshes[SelectedObject].LocalRotate(0, -M_PI/32, 0);
        break;
    case ',':
        scenes[activeScene]->meshes[SelectedObject].LocalRotate(0, 0, M_PI/32);
        break;
    case 'M':
    case 'm':
        scenes[activeScene]->meshes[SelectedObject].LocalRotate(0, 0, -M_PI/32);
        break;
    case '6':
        scenes[activeScene]->meshes[SelectedObject].WorldTranslate(M_PI/32, 0, 0);
        break;
    case '4':
        scenes[activeScene]->meshes[SelectedObject].WorldTranslate(-M_PI/32, 0, 0);
        break;
    case '8':
        scenes[activeScene]->meshes[SelectedObject].WorldTranslate(0, M_PI/32, 0);
        break;
    case '2':
        scenes[activeScene]->meshes[SelectedObject].WorldTranslate(0, -M_PI/32, 0);
        break;
    case '9':
        scenes[activeScene]->meshes[SelectedObject].WorldTranslate(0, 0, M_PI/32);
        break;
    case '1':
        scenes[activeScene]->meshes[SelectedObject].WorldTranslate(0, 0, -M_PI/32);
        break;
    case ']':
        scenes[activeScene]->meshes[SelectedObject].WorldRotate(M_PI/32, 0, 0);
        break;
    case '[':
        scenes[activeScene]->meshes[SelectedObject].WorldRotate(-M_PI/32, 0, 0);
        break;
    case 39:	// Apostrophe
        scenes[activeScene]->meshes[SelectedObject].WorldRotate(0, M_PI/32, 0);
        break;
    case 59:	// Semicolon
        scenes[activeScene]->meshes[SelectedObject].WorldRotate(0, -M_PI/32, 0);
        break;
    case '/':
        scenes[activeScene]->meshes[SelectedObject].WorldRotate(0, 0, M_PI/32);
        break;
    case '.':
        scenes[activeScene]->meshes[SelectedObject].WorldRotate(0, 0, -M_PI/32);
        break;
    case '=':
        scenes[activeScene]->meshes[SelectedObject].LocalScale(1.05);
        break;
    case '-':
        scenes[activeScene]->meshes[SelectedObject].LocalScale(0.95);
        break;
    case 'A':
    case 'a':
        ShowAxes = !ShowAxes;
        break;
    case 'B':
    case 'b':
        ShowBoundingBoxes = !ShowBoundingBoxes;
        break;
    case 'N':
    case 'n':
        SelectionMode = !SelectionMode;
        if(SelectionMode)
            cout << "Selection Mode" << endl;
        else
            cout << "Camera Mode" << endl;
        break;
    case 'Q':
    case 'q':
        exit(1);
        break;
    case 'Y':
    case 'y':
        scenes[activeScene]->getActiveCamera()->MoveView(0.5);
        break;
    case 'U':
    case 'u':
        scenes[activeScene]->getActiveCamera()->MoveView(-0.5);
        break;
    case 'H':
    case 'h':
        scenes[activeScene]->getActiveCamera()->ScaleView(0.95);
        break;
    case 'J':
    case 'j':
        scenes[activeScene]->getActiveCamera()->ScaleView(1.05);
        break;
    case 'X':
    case 'x':
        char* scene;
        switch (activeScene) {
        case 0:
            scene = "scene1.dat";
            activeScene++;
            break;
        case 1:
            scene = "scene2.dat";
            activeScene++;
            break;
        case 2:
            scene = "scene.dat";
            activeScene = 0;
            break;
        }
        if (scenes.size() <= activeScene) {
            scenes.push_back(new Scene);
            scenes[activeScene]->Load(scene);
            scenes[activeScene]->addCamera(PERSPECTIVE);
            scenes[activeScene]->setActiveCamera(0);
        }
        break;
    case 'Z':
    case 'z':
        if (scenes[activeScene]->cams.size() > scenes[activeScene]->activeCamera+1) {
            scenes[activeScene]->setActiveCamera(scenes[activeScene]->activeCamera+1);
        }
        else {
            scenes[activeScene]->setActiveCamera(0);
        }
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    scenes.push_back(new Scene);
    scenes[0]->Load("scene.dat");
    scenes[0]->addCamera(PERSPECTIVE);
    scenes[0]->addCamera(PERSPECTIVE);
    scenes[0]->cams[1]->NearPlane = 0;
    scenes[0]->cams[1]->FarPlane = 10000;
    scenes[0]->cams[1]->Position.z = 10;
    scenes[0]->addCamera(ORTHOGRAPHIC);
    scenes[0]->cams[2]->NearPlane = 0;
    scenes[0]->cams[2]->FarPlane = 10000;
    scenes[0]->cams[2]->Position.x = 10;
    scenes[0]->cams[2]->Position.y = 10;
    scenes[0]->cams[2]->Position.z = 10;
    scenes[0]->cams[2]->Pitch = M_PI/3;
    scenes[0]->cams[2]->Yaw = M_PI/8;
    scenes[0]->setActiveCamera(0);
    scenes[0]->getActiveCamera()->ViewWidth = (float)WindowWidth/32;
    scenes[0]->getActiveCamera()->ViewHeight = (float)WindowHeight/32;


    scenes.push_back(new Scene);
    scenes[1]->Load("scene1.dat");
    scenes[1]->addCamera(PERSPECTIVE);
    scenes[1]->addCamera(PERSPECTIVE);
    scenes[1]->addCamera(PERSPECTIVE);
    scenes[1]->setActiveCamera(0);
    scenes[1]->getActiveCamera()->ViewWidth = (float)WindowWidth/32;
    scenes[1]->getActiveCamera()->ViewHeight = (float)WindowHeight/32;


    scenes.push_back(new Scene);
    scenes[2]->Load("scene2.dat");
    scenes[2]->addCamera(PERSPECTIVE);
    scenes[2]->addCamera(ORTHOGRAPHIC);
    scenes[2]->addCamera(PERSPECTIVE);
    scenes[2]->setActiveCamera(0);
    scenes[2]->getActiveCamera()->ViewWidth = (float)WindowWidth/32;
    scenes[2]->getActiveCamera()->ViewHeight = (float)WindowHeight/32;

    GLConfig::load(argc, argv, GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH, WindowWidth, WindowHeight, "EECS 466 Final Project", scenes);

    glutDisplayFunc(DisplayFunc);
    glutReshapeFunc(ReshapeFunc);
    glutMouseFunc(MouseFunc);
    glutMotionFunc(MotionFunc);
    glutKeyboardFunc(KeyboardFunc);

    // Initialize GL
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Enable illumination
    scenes[0]->useGLSL = true;
    scenes[0]->shaderHelper = ShaderHelper("sampleshader.vert", "sampleshader.frag");
    scenes[1]->useGLSL = true;
    scenes[1]->shaderHelper = ShaderHelper("sampleshader.vert", "sampleshader.frag");
    scenes[2]->useGLSL = true;
    scenes[2]->shaderHelper = ShaderHelper("sampleshader.vert", "sampleshader.frag");

    // Switch to main loop
    glutMainLoop();

    return 0;
}