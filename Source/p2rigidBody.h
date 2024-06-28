#ifndef P2RIGIDBODY_H
#define P2RIGIDBODY_H

#include "Utils/Vector2D.h"
#include "p2shapeBox.h"
#include "p2body.h"

namespace p2
{
    class rigidBody
    {
    public:
        rigidBody() {}
        ~rigidBody() = default;

    public:
        Vector2D position;
        Vector2D linearVelocity;
        Vector2D force;
        float angle;
        float angularVelocity;
        float torque;
        p2::shapeBox shape;
    };
};
#endif