#include "Texture.h"
#include "read_tga.h"
#include "glut.h"
#include "glprocs.h"

using namespace std;

Texture::Texture(char* file) {
    tga = new TGA(file);
}

void Texture::loadTextureBase(GLuint textureLocation, GLuint shaderLocation) {
    glActiveTexture(shaderLocation);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, textureLocation);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void Texture::loadRGBATexture(GLuint textureLocation, GLuint shaderLocation) {
    loadTextureBase(textureLocation, shaderLocation);

    // Finaly build the mipmaps
    glTexImage2D (GL_TEXTURE_2D, 0, 4, tga->GetWidth(), tga->GetHeigth(), 0, GL_RGB, GL_UNSIGNED_BYTE, tga->GetPixels());

    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, tga->GetWidth(), tga->GetHeigth(), GL_RGBA, GL_UNSIGNED_BYTE, tga->GetPixels());
}

void Texture::loadLuminanceTexture(GLuint textureLocation, GLuint shaderLocation) {
    loadTextureBase(textureLocation, shaderLocation);

    // Finaly build the mipmaps
    glTexImage2D (GL_TEXTURE_2D, 0, GL_LUMINANCE, tga->GetWidth(), tga->GetHeigth(), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, tga->GetPixels());

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_LUMINANCE, tga->GetWidth(), tga->GetHeigth(), GL_LUMINANCE, GL_UNSIGNED_BYTE, tga->GetPixels());
}