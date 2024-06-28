#include "p2world.h"
#include "p2common.h"

p2::world::world()
{
    Vector2D origin(0, 0);
    Vector2D axis(800, 800);
    int bucketSize = 3;
    qtree = new p2::QT<int>(origin, axis, bucketSize);
}

void p2::world::Update(float deltaTime) {}

void p2::world::CreateBody(const p2::bodyDef* bodyDef)
{
    p2::body* bDef = new p2::body(bodyDef);
    qtree->insert(bDef, 1);
}