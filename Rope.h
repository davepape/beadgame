#ifndef _Rope_h_
#define _Rope_h_

#include <GL/gl.h>
#include "Spring2.h"

class Rope
    {
    public:
        Rope(const char *texfile,GLfloat width,const Vector2 &p1,const Vector2 &p2,GLint vLoc,GLint tLoc,GLint posLoc);
        void update(int button,int mx,int my);
        void draw(void);
        void makeVertexArray(GLint vLoc,GLint tLoc);
        void updateVertexArray(void);
        
        static const int numPoints = 64;
        class Texture2D * tex_;
        GLint vertexArray_;
        GLuint vbuffer_;
        GLint posLoc_;
        PointMass2 *point_[numPoints];
        Spring2 *spring_[numPoints-1];
        GLfloat width_;
        int grabbedPoint_;
        int prevButton_;
    };

#endif
