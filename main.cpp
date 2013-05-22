#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/glfw.h>
#include "Sprite.h"
#include "Rope.h"

void compileShader(GLuint shaderID, const char *shaderSource)
    {
    GLint result;
    int infoLogLength;
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE)
        {
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        char *buf = (char *)malloc(infoLogLength+1);
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, buf);
        fprintf(stderr,"%s\n", buf);
        free(buf);
        }
    }

char * readFile(const char *fname)
    {
    fprintf(stderr,"Reading %s\n",fname);
    char buf[10000];
    int fd = open(fname,O_RDONLY);
    int n = read(fd,buf,sizeof(buf));
    buf[n] = '\0';
    close(fd);
    return strdup(buf);
    }

GLuint LoadShaders(const char *vertFile,const char * fragFile)
    {
    GLuint vShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    char * vShaderSource = readFile(vertFile);
    char * fShaderSource = readFile(fragFile);
    compileShader(vShaderID, vShaderSource);
    compileShader(fShaderID, fShaderSource);
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vShaderID);
    glAttachShader(programID, fShaderID);
    glLinkProgram(programID);
    GLint result;
    int infoLogLength;
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    if (result != GL_TRUE)
        {
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        char *buf = (char *)malloc(infoLogLength+1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, buf);
        fprintf(stderr,"%s\n", buf);
        free(buf);
        }
    glDeleteShader(vShaderID);
    glDeleteShader(fShaderID);
    return programID;
    }


void checkError(const char *s)
    {
    GLenum errorNum = glGetError();
    const char * error;
    if (errorNum == GL_NO_ERROR)
        return;
    if (errorNum == GL_INVALID_ENUM)
        error = "GL_INVALID_ENUM";
    else if (errorNum == GL_INVALID_VALUE)
        error = "GL_INVALID_VALUE";
    else if (errorNum == GL_INVALID_OPERATION)
        error = "GL_INVALID_OPERATION";
    else if (errorNum == GL_INVALID_FRAMEBUFFER_OPERATION)
        error = "GL_INVALID_FRAMEBUFFER_OPERATION";
    else if (errorNum == GL_OUT_OF_MEMORY)
        error = "GL_OUT_OF_MEMORY";
    fprintf(stderr,"%s: error %s\n",s,error);
    }

#define NUMSPRITES 24

int main(int argc,char **argv)
    {
    const char * spriteTex[] = { 
"bead1.tif", "bead2.tif", "bead3.tif", "bead4.tif", "bead5.tif", "bead6.tif", "bead7.tif", "bead8.tif",
"bead1.tif", "bead2.tif", "bead3.tif", "bead4.tif", "bead5.tif", "bead6.tif", "bead7.tif", "bead8.tif",
"bead1.tif", "bead2.tif", "bead3.tif", "bead4.tif", "bead5.tif", "bead6.tif", "bead7.tif", "bead8.tif"
 };
    Sprite * sprite[NUMSPRITES];
    Rope * rope, *rope2;
    if (!glfwInit())
        {
        fprintf(stderr,"Failed to initialize GLFW\n");
        exit(1);
        }
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    if (!glfwOpenWindow(1024,768, 0,0,0,0, 32,0, GLFW_WINDOW))
    if (!glfwOpenWindow(1024,768, 0,0,0,0, 32,0, GLFW_FULLSCREEN))
        {
        fprintf(stderr, "Failed to open GLFW window");
        exit(1);
        }
    glfwSetWindowTitle("OpenGL");
    glfwEnable(GLFW_STICKY_KEYS);
    glfwEnable(GLFW_MOUSE_CURSOR);
    
    GLuint programID = LoadShaders("vert.txt","frag.txt");
    glClearColor(1, 1, 1, 0);
    
    GLint vposLoc = glGetAttribLocation(programID,"vpos");
    GLint texcoordLoc = glGetAttribLocation(programID,"texcoord");
    GLint posLoc = glGetUniformLocation(programID,"pos");
    GLint samplerLoc = glGetUniformLocation(programID,"uTexture");
    srand48(1);
    for (int i=0; i < NUMSPRITES; i++)
        {
        sprite[i] = new Sprite(spriteTex[i],30,30,vposLoc,texcoordLoc,posLoc);
        sprite[i]->pos_[0] = drand48()*1024;
        sprite[i]->pos_[1] = drand48()*768;
        }
    rope = new Rope("rope.tif", 5, Vector2(10,20), Vector2(900,20),vposLoc,texcoordLoc,posLoc);
    rope2 = new Rope("rope.tif", 5, Vector2(130,760), Vector2(1020,760),vposLoc,texcoordLoc,posLoc);
    
    while ((glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS) && (glfwGetWindowParam(GLFW_OPENED)))
        {
        int mx,my,button;
        glfwGetMousePos(&mx,&my);
        button = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
        for (int i=0; i < NUMSPRITES; i++)
            sprite[i]->update(button,mx,767-my);
        rope->update(button,mx,767-my);
        rope2->update(button,mx,767-my);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUseProgram(programID);
        glUniform1i(samplerLoc, 0);
        for (int i=0; i < NUMSPRITES; i++)
            sprite[i]->draw();
        rope->draw();
        rope2->draw();
        glfwSwapBuffers();
        checkError("endframe");
        }

    glfwTerminate();
//	glDeleteBuffers(1, &vbuffer);
//	glDeleteVertexArrays(1, &VertexArrayID1);
    }

