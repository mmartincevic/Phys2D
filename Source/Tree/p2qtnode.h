#ifndef P2QUADTREENODE_H
#define P2QUADTREENODE_H

#include <cstdlib>
#include <vector>
#include <utility>
#include "../Utils/Vector2D.h"

namespace p2
{
    template <class T>
    class QTNode {

        public:
            QTNode <T>(Vector2D newCenter, Vector2D newRange) {
                child[0] = NULL;
                child[1] = NULL;
                child[2] = NULL;
                child[3] = NULL;
                center = newCenter;
                range = newRange;
                leaf = true;
            }
            ~QTNode() { for (int i = 0; i < 4; ++i) if (child[i]) { delete child[i]; } }

            Vector2D center, range;

            bool leaf;
            QTNode* child[4];
            std::vector <std::pair <Vector2D, T> > bucket;
    };
};
#endif