#ifndef P2PARTICLE_H
#define P2PARTICLE_H

#include "Utils/Vector2D.h"

namespace p2
{
    typedef struct
    {
        Vector2D position;
        Vector2D velocity;
        float mass;
    } particle;
};

#endif