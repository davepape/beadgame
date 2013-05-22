#ifndef _Sprite_h_
#define _Sprite_h_

#include <GL/gl.h>

class Sprite
    {
    public:
        Sprite(const char *texfile,GLfloat width,GLfloat height,GLint vLoc,GLint tLoc,GLint posLoc);
        void update(int button,int mx,int my);
        void draw(void);

        class Texture2D * tex_;
        GLint vertexArray_;
        GLint posLoc_;
        GLfloat pos_[2];
        GLfloat width_, height_;
        bool grabbed_;
        int prevButton_;
        static Sprite * grabbedSprite_;
    };

#endif
