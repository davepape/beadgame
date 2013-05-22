#include <math.h>
#include "Vector2.h"

const Vector2 Vector2::Zero(0.0, 0.0);
const Vector2 Vector2::X_Axis(1.0, 0.0);
const Vector2 Vector2::Y_Axis(0.0, 1.0);


Vector2::Vector2(void)
    {
    set(0, 0);
    }


Vector2::Vector2(GLfloat x, GLfloat y)
    {
    set(x, y);
    }


Vector2::Vector2(const GLfloat *v)
    {
    set(v[0], v[1]);
    }


Vector2::Vector2(const Vector2 &v)
    {
    set(v.vec[0], v.vec[1]);
    }


Vector2& Vector2::operator= (const Vector2& v)
    {
    set(v.vec[0], v.vec[1]);
    return *this;
    }


void Vector2::set(GLfloat x, GLfloat y)
    {
    vec[0] = x;
    vec[1] = y;
    }


GLfloat& Vector2::operator[](int i)
    {
    if ((i >= 0) && (i <= 1))
        return vec[i];
    else if (i < 0)
        return vec[0];
    else if (i > 1)
        return vec[1];
    }


GLfloat Vector2::operator[](int i) const
    {
    if ((i >= 0) && (i <= 1))
        return vec[i];
    else if (i < 0)
        return vec[0];
    else if (i > 1)
        return vec[1];
    }


Vector2& Vector2::operator+=(const Vector2& v)
    {
    vec[0] += v.vec[0];
    vec[1] += v.vec[1];
    return *this;
    }


Vector2& Vector2::operator-=(const Vector2& v)
    {
    vec[0] -= v.vec[0];
    vec[1] -= v.vec[1];
    return *this;
    }


Vector2& Vector2::operator*=(GLfloat s)
    {
    vec[0] *= s;
    vec[1] *= s;
    return *this;
    }


Vector2& Vector2::operator/=(GLfloat s)
    {
    vec[0] /= s;
    vec[1] /= s;
    return *this;
    }


Vector2 Vector2::operator+(const Vector2& v) const
    {
    return Vector2(vec[0]+v.vec[0], vec[1]+v.vec[1]);
    }


Vector2 Vector2::operator-(const Vector2& v) const
    {
    return Vector2(vec[0]-v.vec[0], vec[1]-v.vec[1]);
    }


Vector2 Vector2::operator-(void) const
    {
    return Vector2(-vec[0],-vec[1]);
    }


Vector2 Vector2::operator*(GLfloat s) const
    {
    return Vector2(vec[0]*s, vec[1]*s);
    }


Vector2 Vector2::operator/(GLfloat s) const
    {
    return Vector2(vec[0]/s, vec[1]/s);
    }


bool Vector2::operator==(const Vector2& v) const
    {
    return ((vec[0] == v.vec[0]) &&
            (vec[1] == v.vec[1]));
    }


bool Vector2::operator!=(const Vector2& v) const
    {
    return ((vec[0] != v.vec[0]) ||
            (vec[1] != v.vec[1]));
    }

        
GLfloat Vector2::length(void) const
    {
    return sqrt(lengthSquared());
    }


GLfloat Vector2::lengthSquared(void) const
    {
    return vec[0]*vec[0] + vec[1]*vec[1];
    }


void Vector2::normalize(void)
    {
    float len = length();
    if (len > 0)
        {
        vec[0] /= len;
        vec[1] /= len;
        }
    }


GLfloat Vector2::dot(const Vector2& v) const
    {
    return vec[0]*v.vec[0] + vec[1]*v.vec[1];
    }


GLfloat Vector2::distance(const Vector2& v) const
    {
    return sqrt(distanceSquared(v));
    }


GLfloat Vector2::distanceSquared(const Vector2& v) const
    {
    float dx, dy;
    dx = vec[0] - v.vec[0];
    dy = vec[1] - v.vec[1];
    return dx*dx + dy*dy;
    }
