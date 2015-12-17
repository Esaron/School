#include "ShaderHelper.h"
#include <iostream>
#include "glut.h"
#include <windows.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "glprocs.h"

using namespace std;

ShaderHelper::ShaderHelper() {
    p = -1;
}

ShaderHelper::ShaderHelper(char* vertFile, char* fragFile) {
    setShaders(shaderFileRead(vertFile), shaderFileRead(fragFile));
}

int ShaderHelper::PrintOGLError(char *file, int line)
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

//Read the shader files, given as parameter.
char* ShaderHelper::shaderFileRead(char *fn) {


    FILE *fp = fopen(fn,"r");
    if(!fp)
    {
        cout<< "Failed to load " << fn << endl;
        return " ";
    }
    else
    {
        cout << "Successfully loaded " << fn << endl;
    }

    char *content = NULL;

    int count=0;

    if (fp != NULL) 
    {
        fseek(fp, 0, SEEK_END);
        count = ftell(fp);
        rewind(fp);

        if (count > 0) 
        {
            content = (char *)malloc(sizeof(char) * (count+1));
            count = fread(content,sizeof(char),count,fp);
            content[count] = '\0';
        }
        fclose(fp);
    }
    return content;
}

void ShaderHelper::setShaders(char* vs, char* fs) 
{

    //create the empty shader objects and get their handles
    vertShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    fragShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    const char * vv = vs;
    const char * ff = fs;

    GLint       vertCompiled, fragCompiled;

    //set the shader's source code by using the strings read from the shader files.
    glShaderSourceARB(vertShader, 1, &vv,NULL);
    glShaderSourceARB(fragShader, 1, &ff,NULL);

    free(vs);free(fs);

    //Compile the shader objects
    glCompileShaderARB(vertShader);
    glCompileShaderARB(fragShader);

    glGetObjectParameterivARB(fragShader, GL_OBJECT_COMPILE_STATUS_ARB, &fragCompiled);
    glGetObjectParameterivARB(vertShader, GL_OBJECT_COMPILE_STATUS_ARB, &vertCompiled);
    if (!vertCompiled)
    {
        cout<<" vert not compiled"<<endl;
    }
    if (!fragCompiled)
    {
        cout<<" frag not compiled"<<endl;
    }

    //create an empty program object to attach the shader objects
    p = glCreateProgramObjectARB();

    //attach the shader objects to the program object
    glAttachObjectARB(p,vertShader);
    glAttachObjectARB(p,fragShader);

    /*
    **************
    Programming Tip:
    ***************
    Delete the attached shader objects once they are attached.
    They will be flagged for removal and will be freed when they are no more used.
    */
    glDeleteObjectARB(vertShader);
    glDeleteObjectARB(fragShader);

    //Link the created program.
    /*
    **************
    Programming Tip:
    ***************
    You can trace the status of link operation by calling 
    "glGetObjectParameterARB(p,GL_OBJECT_LINK_STATUS_ARB)"
    */
    glLinkProgramARB(p);


    //Start to use the program object, which is the part of the current rendering state
    glUseProgramObjectARB(p);
}

void ShaderHelper::updateLightPosition(Vertex lightPos, GLuint lightNum) {
    GLfloat position[] = {lightPos.x, lightPos.y, lightPos.z, 0.0};
    glLightfv(lightNum, GL_POSITION, position);
}

void ShaderHelper::setUniform1fLocation(char* uniform, GLfloat value) {
    glUniform1fARB(glGetUniformLocationARB(p, uniform), value);
}

void ShaderHelper::setUniform4fLocation(char* uniform, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4) {
    glUniform4fARB(glGetUniformLocationARB(p, uniform), value1, value2, value3, value4);
}

void ShaderHelper::setAttrib4fLocation(char* attrib, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4){
    glVertexAttrib4fARB(glGetAttribLocationARB(p, attrib), value1, value2, value3, value4);
}