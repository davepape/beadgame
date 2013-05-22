#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/glfw.h>
#include "Texture2D.h"
#include "Rope.h"

extern void checkError(const char *s);


Rope::Rope(const char *texfile,GLfloat width,const Vector2 &p1,const Vector2 &p2,GLint vLoc,GLint tLoc,GLint posLoc)
    {
    tex_ = new Texture2D(texfile);
    width_ = width;
    posLoc_ = posLoc;
    grabbedPoint_ = -1;
    for (int i=0; i < numPoints; i++)
        { 
        point_[i] = new PointMass2(p1 + (p2-p1)*((float)i)/numPoints);
        point_[i]->setDrag(1);
        }
    for (int i=0; i < numPoints-1; i++)
        spring_[i] = new Spring2(point_[i],point_[i+1],5.0);
    makeVertexArray(vLoc,tLoc);
    }


void Rope::update(int button,int mx,int my)
    {
    Vector2 mouse(mx,my);
    if (grabbedPoint_ > -1)
        {
        if (!button)
            {
            point_[grabbedPoint_]->unfreezePosition();
            grabbedPoint_ = -1;
            }
        else
            {
            point_[grabbedPoint_]->position_ = mouse;
            }
        }
    else if ((button) && (!prevButton_))
        {
        for (int i=0; i < numPoints; i++)
            {
            if (point_[i]->position_.distance(mouse) < width_*5.0)
                {
                grabbedPoint_ = i;
                }
            }
        if (grabbedPoint_ > -1)
            point_[grabbedPoint_]->freezePosition();
        }
    prevButton_ = button;
    for (int step=0; step < 100; step++)
        {
        for (int i=0; i < numPoints; i++)
            point_[i]->clearForces();
        for (int i=0; i < numPoints-1; i++)
            spring_[i]->apply();
        for (int i=0; i < numPoints; i++)
            point_[i]->update(0.0001);
        }
    updateVertexArray();
    }


void Rope::makeVertexArray(GLint vLoc,GLint tLoc)
    { 
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

    GLfloat vertex_data[numPoints*4];
    for (int i=0; i < numPoints; i++)
        {
        vertex_data[i*4] = point_[i]->position_[0];
        vertex_data[i*4+1] = point_[i]->position_[1] - width_/2.0;
        vertex_data[i*4+2] = point_[i]->position_[0];
        vertex_data[i*4+3] = point_[i]->position_[1] + width_/2.0;
        }
    glGenBuffers(1, &vbuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vLoc, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(vLoc);

    GLfloat texcoord_data[numPoints*4];
    for (int i=0; i < numPoints; i++)
        {
        texcoord_data[i*4] = i;
        texcoord_data[i*4+1] = 0;
        texcoord_data[i*4+2] = i;
        texcoord_data[i*4+3] = 1;
        }
    GLuint tcbuffer;
    glGenBuffers(1, &tcbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tcbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord_data), texcoord_data, GL_STATIC_DRAW);
    glVertexAttribPointer(tLoc, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(tLoc);

	glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    vertexArray_ =  vertexArrayID;
    }


void Rope::updateVertexArray(void)
    {
    GLfloat vertex_data[numPoints*4];
    for (int i=0; i < numPoints; i++)
        {
        vertex_data[i*4] = point_[i]->position_[0];
        vertex_data[i*4+1] = point_[i]->position_[1] - width_/2.0;
        vertex_data[i*4+2] = point_[i]->position_[0];
        vertex_data[i*4+3] = point_[i]->position_[1] + width_/2.0;
        }
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


void Rope::draw(void)
    {
    GLfloat pos[2] = {0,0};
    tex_->apply();
    glUniform2fv(posLoc_, 1, pos);
	glBindVertexArray(vertexArray_);   
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*numPoints);
    }

