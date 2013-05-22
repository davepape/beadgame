#ifndef _springs_h_
#define _springs_h_

#include "Vector2.h"

#define MAX_POINTMASSES 10000
#define MAX_SPRINGS 10000

class PointMass2
    {
    public:
     PointMass2(const Vector2& pos,float m=1.0,bool save=true);
     void clearForces(void);
     void applyForce(const Vector2& force);
     void setForce(const Vector2& force);
     void update(float dt);
     void freezePosition(void);
     void unfreezePosition(void);
     void setDrag(float d=0);
     inline float mass(void) { return mass_; }
     inline const Vector2& position(void) const { return position_; }
     inline const Vector2& velocity(void) const { return velocity_; }
     inline const Vector2& force(void) const { return force_; }
     inline bool frozen(void) const { return frozen_; }
     inline float drag(void) const { return drag_; }
     void setMass(float m);
     void setPosition(const Vector2& pos);
     void setVelocity(const Vector2& vel);

     float mass_;
     Vector2 position_;
     Vector2 velocity_;
     Vector2 force_;
     bool frozen_;
     float drag_;
     static PointMass2 *allPoints_[MAX_POINTMASSES];
     static int numPoints_;
    };



class Spring2
    {
    public:
     Spring2(PointMass2* p0,PointMass2* p1,float k=1.0,float damping=0,float len=-1);
     void apply(void);
     inline const PointMass2& point0(void) { return *p0_; }
     inline const PointMass2& point1(void) { return *p1_; }
     inline float restLength(void) { return restLength_; }
     inline float springConstant(void) { return springConstant_; }
     void setRestLength(float len);
     void setSpring2Constant(float k);

     PointMass2 *p0_, *p1_;
     float restLength_, springConstant_, damping_;
     static Spring2 *allSpring2s_[MAX_SPRINGS];
     static int numSpring2s_;
    };


#endif
