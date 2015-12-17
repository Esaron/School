#ifndef SCENE_H
#define SCENE_H

#include "light.h"
#include "mesh.h"
#include "Camera.h"
#include "ShaderHelper.h"
#include <vector>

class Scene
{
public:
    ShaderHelper shaderHelper;
    int numLights;
    int numMeshes;
    Light* lights;
    Mesh* meshes;
    Color ambientLight;
    std::vector<Camera*> cams;
    int activeCamera;
    bool useGLSL;
    int WindowWidth, WindowHeight;

    void Load(char* file);
    void addCamera(int type);
    void removeCamera(int index);
    Camera* getActiveCamera();
    void setActiveCamera(int activeCamera);
    void RenderAll();
    void RenderAll(int gl_render_type);
};

#endif