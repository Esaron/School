#include <iostream>
#include "scene.h"
#include "util.h"
#include "Camera.h"
#include "glut.h"
#include "ShaderHelper.h"
#include "mesh.h"

using namespace std;

// Load a Scene (.dat) file
void Scene::Load(char *fn) {
    useGLSL = false;

    FILE *fp = fopen(fn,"r");
    if(!fp)
    {
        cout<< "Failed to load " << fn << endl;
        return;
    }
    else
    {
        cout << "Successfully loaded " << fn << endl;
    }

    numLights = 0, numMeshes = 0;
    if (!feof(fp))
    {
        fscanf(fp, "<%d> <%d>\n", &numLights, &numMeshes);
        lights = new Light[numLights];
        meshes = new Mesh[numMeshes];
        for (int i = 0; i < numLights; i++) {
            int type;
            float x, y, z, r, g, b;

            fscanf(fp, "L <%d> <%f %f %f> <%f %f %f>\n", &type, &x, &y, &z, &r, &g, &b);

            lights[i] = Light(type, x, y, z, r, g, b);
        }
        for (int i = 0; i < numMeshes; i++) {
            char meshFile[255];
            float scale, rotX, rotY, rotZ,
                x, y, z,
                ra, ga, ba,
                rd, gd, bd,
                rs, gs, bs,
                ka, kd, ks,
                specularExponent, indexOfRefraction,
                kReflective, kRefractive;

            fscanf(fp, "M < %s > <%f> <%f %f %f> <%f %f %f> <%f %f %f> <%f %f %f> <%f %f %f> <%f> <%f> <%f> <%f> <%f> <%f> <%f>\n",
                meshFile,
                &scale, &rotX, &rotY, &rotZ,
                &x, &y, &z,
                &ra, &ga, &ba,
                &rd, &gd, &bd,
                &rs, &gs, &bs,
                &ka, &kd, &ks,
                &specularExponent, &indexOfRefraction,
                &kReflective, &kRefractive);

            Material material = Material(ra, ga, ba,
                rd, gd, bd,
                rs, gs, bs,
                ka, kd, ks,
                specularExponent, indexOfRefraction,
                kReflective, kRefractive);

            meshes[i].Load(meshFile, scale, rotX, rotY, rotZ, x, y, z);
            meshes[i].material = material;
        }
    }
    fseek(fp, 0L, SEEK_SET);
    fclose(fp);

    ambientLight = Color(0.2, 0.2, 0.2);
}

void Scene::addCamera(int type) {
    cams.push_back(new Camera(type));
}

void Scene::removeCamera(int index) {
    if (index < 0 || index > cams.size()-1) {
        error_exit(1001, "Index must be between 0 and %i.");
    }
    cams.erase(cams.begin() + index);
    if (index == activeCamera == 0) {
        activeCamera = 0;
    }
    else if (index <= activeCamera) {
        activeCamera--;
    }
}

void Scene::setActiveCamera(int index) {
    activeCamera = index;
}

Camera* Scene::getActiveCamera() {
    return cams[activeCamera];
}

void Scene::RenderAll() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // draw objects
    if (useGLSL) {
        shaderHelper.updateLightPosition(lights[0].location, GL_LIGHT0);
        gluPerspective(60,(GLdouble) WindowWidth/WindowHeight,0.01,10000);
        for(int i = 0; i < numMeshes; i++)
        {
            meshes[i].Render(shaderHelper, this, getActiveCamera());
        }
    }
    else {
        for(int i = 0; i < numMeshes; i++)
        {
            meshes[i].Render(getActiveCamera());
        }
    }
}

void Scene::RenderAll(int gl_render_type) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // draw objects
    if (useGLSL) {
        shaderHelper.updateLightPosition(lights[0].location, GL_LIGHT0);
        gluPerspective(60,(GLdouble) WindowWidth/WindowHeight,0.01,10000);
        for(int i = 0; i < numMeshes; i++)
        {
            meshes[i].Render(shaderHelper, this, gl_render_type, getActiveCamera());
        }
    }
    else {
        for(int i = 0; i < numMeshes; i++)
        {
            meshes[i].Render(gl_render_type, getActiveCamera());
        }
    }
}