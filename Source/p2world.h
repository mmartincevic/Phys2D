#ifndef P2WORLD_H
#define P2WORLD_H

#include "p2bodyDef.h"
#include "Tree/p2qt.h"

namespace p2
{
    class world
    {
    public:
        world();
        // TODO: Define destructor properly
        ~world() {}; // Sort this out later 
    
    public:
        void CreateBody(const p2::bodyDef* bodyDef);
        void Update(float deltaTime);
        inline void Draw(SDL_Renderer* renderer) { qtree->draw(renderer); }

    private:
        p2::QT<int>* qtree;
    };
}
#endif // !P2WORLD_H