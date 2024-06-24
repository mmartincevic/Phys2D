#ifndef P2RIGIDBODY_H
#define P2RIGIDBODY_H

#include "Utils/Vector2D.h"
#include "p2shapeBox.h"

namespace p2
{
    typedef struct
    {
        Vector2D position;
        Vector2D linearVelocity;
        Vector2D force;
        float angle;
        float angularVelocity;
        float torque;
        p2::shapeBox shape;

    } rigidBody;
};
#endif