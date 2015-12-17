#ifndef TEXTURE_H
#define TEXTURE_H

#include "read_tga.h"
#include "glut.h"

class Texture {
public:
    TGA* tga;

    Texture(char* file);

    void loadTextureBase(GLuint textureLocation, GLuint shaderLocation);
    void loadRGBATexture(GLuint textureLocation, GLuint shaderLocation);
    void loadLuminanceTexture(GLuint textureLocation, GLuint shaderLocation);
};

#endif