#ifndef _Vector2_h_
#define _Vector2_h_

/* with ideas liberally taken from OpenGL Performer's pfVec3 &
   Magic Software's MgcVector2 */

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <iostream>

class Vector2
    {
    public:
        GLfloat vec[2];

        Vector2(void);
        Vector2(GLfloat x, GLfloat y);
        Vector2(const GLfloat *v);
        Vector2(const Vector2 &v);
        Vector2& operator= (const Vector2& v);
        void set(GLfloat x, GLfloat y);

        GLfloat& operator[](int i);
        GLfloat operator[](int i) const;

        Vector2& operator+=(const Vector2& v);
        Vector2& operator-=(const Vector2& v);
        Vector2& operator*=(GLfloat s);
        Vector2& operator/=(GLfloat s);
        Vector2 operator+(const Vector2& v) const;
        Vector2 operator-(const Vector2& v) const;
        Vector2 operator*(GLfloat s) const;
        Vector2 operator/(GLfloat s) const;
        Vector2 operator-(void) const;
        bool operator==(const Vector2& v) const;
        bool operator!=(const Vector2& v) const;
        
        GLfloat length(void) const;
        GLfloat lengthSquared(void) const;
        void normalize(void);
        GLfloat dot(const Vector2& v) const;
        GLfloat distance(const Vector2& v) const;
        GLfloat distanceSquared(const Vector2& v) const;
        
        static const Vector2 Zero;
        static const Vector2 X_Axis;
        static const Vector2 Y_Axis;

    private:
    };


#if 0
inline std::ostream& operator<< (std::ostream& s, const Vector2& v)
        { return s << "(" << v.vec[0] << ", " << v.vec[1] << ")"; }
#endif

inline Vector2 operator* (GLfloat s, const Vector2& v) { return v*s; }

#endif
