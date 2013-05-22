#include <stdio.h>
#include "Spring2.h"

PointMass2 * PointMass2::allPoints_[MAX_POINTMASSES];
int PointMass2::numPoints_ = 0;

PointMass2::PointMass2(const Vector2& pos,float m,bool save)
    {
    mass_ = m;
    position_ = pos;
    velocity_.set(0,0);
    force_.set(0,0);
    frozen_ = false;
    drag_ = 0;
    if (save)
        {
        if (numPoints_ < MAX_POINTMASSES)
            allPoints_[numPoints_++] = this;
        else
            fprintf(stderr,"Warning: Max number of PointMass2es (%d) exceeded\n",MAX_POINTMASSES);
        }
    }


void PointMass2::clearForces(void)
    {
    force_.set(0,0);
    }


void PointMass2::applyForce(const Vector2& force)
    {
    force_ += force;
    }


void PointMass2::setForce(const Vector2& force)
    {
    force_ = force;
    }


void PointMass2::update(float dt)
    {
    if (frozen_)
        velocity_.set(0,0);
    else
        {
        applyForce(velocity_ * -drag_);
        velocity_ += force_ * (dt/mass_);
        position_ += velocity_ * dt;
        }
    }


void PointMass2::freezePosition(void)
    {
    frozen_ = true;
    }


void PointMass2::unfreezePosition(void)
    {
    frozen_ = false;
    }


void PointMass2::setMass(float m)
    {
    mass_ = m;
    }


void PointMass2::setPosition(const Vector2& pos)
    {
    position_ = pos;
    }


void PointMass2::setVelocity(const Vector2& vel)
    {
    velocity_ = vel;
    }

void PointMass2::setDrag(float d)
    {
    drag_ = d;
    }




/*************************************************************************/

Spring2 * Spring2::allSpring2s_[MAX_SPRINGS];
int Spring2::numSpring2s_ = 0;

Spring2::Spring2(PointMass2* p0,PointMass2* p1,float k,float damping,float len)
    {
    p0_ = p0;
    p1_ = p1;
    springConstant_ = k;
    if (len >= 0)
        restLength_ = len;
    else
        restLength_ = p0_->position().distance(p1_->position());
    damping_ = damping;
    if (numSpring2s_ < MAX_SPRINGS)
        allSpring2s_[numSpring2s_++] = this;
    else
        fprintf(stderr,"Warning: Max number of Spring2s (%d) exceeded\n",MAX_SPRINGS);
    }


void Spring2::setRestLength(float len)
    {
    restLength_ = len;
    }


void Spring2::setSpring2Constant(float k)
    {
    springConstant_ = k;
    }


void Spring2::apply(void)
    {
    Vector2 v = p1_->position() - p0_->position();
    float vlength = v.length();
    float forceMagnitude = springConstant_ * (vlength - restLength_);
    if (vlength > 0)
        v /= vlength;
    Vector2 f = v * forceMagnitude;
    p0_->applyForce(f);
    p1_->applyForce(-f);
    Vector2 dv = p1_->velocity() - p0_->velocity();
    f = damping_ * dv.dot(v) * v;
    p0_->applyForce(f);
    p1_->applyForce(-f);
    }


