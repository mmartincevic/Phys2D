#ifndef P2SHAPEBOX_H
#define P2SHAPEBOX_H

#include "Utils/Vector2D.h"

namespace p2
{
    typedef struct
    {
        float width;
        float height;
        float mass;
        float momentOfInertia;
    } shapeBox;
};

#endif