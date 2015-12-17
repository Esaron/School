#ifndef CONFIG_H
#define CONFIG_H

#include "glut.h"
#include <vector>
#include "scene.h"

class GLConfig {
public:
    static GLuint displayModeBitmask;
    static int windowPosX;
    static int windowPosY;
    static int windowSizeX;
    static int windowSizeY;
    static char* windowTitle;

    static void load(int argc, char* argv[], GLuint displayModeBitmask,
        int windowSizeX, int windowSizeY, char* windowTitle, std::vector<Scene*> scenes);
};

#endif