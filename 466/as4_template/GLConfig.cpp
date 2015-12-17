#include "GLConfig.h"
#include <vector>
#include "scene.h"

void GLConfig::load(int argc, char* argv[], GLuint displayModeBitmask,
                    int windowSizeX, int windowSizeY, char* windowTitle, std::vector<Scene*> scenes) {
                        // Initialize GLUT
                        glutInit(&argc, argv);
                        glutInitWindowSize(windowSizeX, windowSizeY);
                        glutInitDisplayMode(displayModeBitmask);
                        glutCreateWindow(windowTitle);
                        for (int i = 0; i < scenes.size(); i++) {
                            scenes[i]->WindowWidth = windowSizeX;
                            scenes[i]->WindowHeight = windowSizeY;
                        }
}