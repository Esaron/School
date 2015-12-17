#ifndef SHADER_H
#define SHADER_H

#include "glut.h"
#include "vertex.h"

class Mesh;
class Scene;

class ShaderHelper {
public:
    ShaderHelper();
    ShaderHelper(char* vertFile, char* fragFile);

    GLuint vertShader, fragShader, p;

    int PrintOGLError(char *file, int line);
    char *shaderFileRead(char *fn);
    void setShaders(char* vertex, char* fragment);
    void updateLightPosition(Vertex lightPos, GLuint lightNum);
    void setUniform1fLocation(char* uniform, GLfloat value);
    void setUniform4fLocation(char* uniform, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4);
    void setAttrib4fLocation(char* attrib, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4);
    void setParameters(Scene* scene, Mesh* mesh);
};

#endif