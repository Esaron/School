#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"

class Material {
public:
    Color ambient, diffuse, specular;
    float kAmbient, kDiffuse, kSpecular, indexOfRefraction, specularExponent, kReflective, kRefractive;

    Material() {
        ambient = Color();
        diffuse = Color();
        specular = Color();
        kAmbient = 1;
        kDiffuse = 1;
        kSpecular = 1;
        specularExponent = 1;
        indexOfRefraction = 1;
        kReflective = 1;
        kRefractive = 1;
    }

    Material(float ambientR, float ambientG, float ambientB,
        float diffuseR, float diffuseG, float diffuseB,
        float specularR, float specularG, float specularB,
        float kAmbientIn, float kDiffuseIn, float kSpecularIn,
        float specularExponentIn, float indexOfRefractionIn,
        float kReflectiveIn, float kRefractiveIn) {
            ambient = Color(ambientR, ambientG, ambientB);
            diffuse = Color(diffuseR, diffuseG, diffuseB);
            specular = Color(specularR, specularG, specularB);
            kAmbient = kAmbientIn;
            kDiffuse = kDiffuseIn;
            kSpecular = kSpecularIn;
            specularExponent = specularExponentIn;
            indexOfRefraction = indexOfRefractionIn;
            kReflective = kReflectiveIn;
            kRefractive = kRefractiveIn;
    }

    Material(Color ambientIn, Color diffuseIn, Color specularIn,
        float kAmbientIn, float kDiffuseIn, float kSpecularIn,
        float specularExponentIn, float indexOfRefractionIn,
        float kReflectiveIn, float kRefractiveIn) {
            ambient = ambientIn;
            diffuse = diffuseIn;
            specular = specularIn;
            kAmbient = kAmbientIn;
            kDiffuse = kDiffuseIn;
            kSpecular = kSpecularIn;
            specularExponent = specularExponentIn;
            indexOfRefraction = indexOfRefractionIn;
            kReflective = kReflectiveIn;
            kRefractive = kRefractiveIn;
    }
};

#endif