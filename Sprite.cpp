#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/glfw.h>
#include "Texture2D.h"
#include "Sprite.h"

extern void checkError(const char *s);

Sprite * Sprite::grabbedSprite_ = 0;

static GLuint makeVertexArray(GLfloat width,GLfloat height,GLint vLoc,GLint tLoc)
    { 
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

    GLfloat vertex_data[] = { -0.5, -0.5,  0.5, -0.5,  -0.5, 0.5,  0.5, 0.5 };
    for (int i=0; i < 4; i++)
        {
        vertex_data[i*2] *= width;
        vertex_data[i*2+1] *= height;
        }
    GLuint vbuffer;
    glGenBuffers(1, &vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    glVertexAttribPointer(vLoc, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(vLoc);

    GLfloat texcoord_data[] = { 0, 0,  1, 0,  0, 1,  1, 1 };
    GLuint tcbuffer;
    glGenBuffers(1, &tcbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tcbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord_data), texcoord_data, GL_STATIC_DRAW);
    glVertexAttribPointer(tLoc, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(tLoc);

	glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vertexArrayID;
    }

Sprite::Sprite(const char *texfile,GLfloat width,GLfloat height,GLint vLoc,GLint tLoc,GLint posLoc)
    {
    tex_ = new Texture2D(texfile);
    vertexArray_ = makeVertexArray(width,height,vLoc,tLoc);
    pos_[0] = 0;
    pos_[1] = 0;
    width_ = width;
    height_ = height;
    posLoc_ = posLoc;
    grabbed_ = false;
    }


void Sprite::update(int button,int mx,int my)
    {
    if (grabbed_)
        {
        if (!button)
            {
            grabbed_ = false;
            grabbedSprite_ = 0;
            }
        else
            {
            pos_[0] = mx;
            pos_[1] = my;
            }
        }
    else if ((button) && (!prevButton_) && (!grabbedSprite_))
        {
        if ((mx > pos_[0] - width_/2.0) && (mx < pos_[0] + width_/2.0) &&
            (my > pos_[1] - height_/2.0) && (my < pos_[1] + height_/2.0))
            {
            grabbed_ = true;
            grabbedSprite_ = this;
            }
        }
    prevButton_ = button;
    }
    

void Sprite::draw(void)
    {
    tex_->apply();
    glUniform2fv(posLoc_, 1, pos_);
	glBindVertexArray(vertexArray_);   
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
