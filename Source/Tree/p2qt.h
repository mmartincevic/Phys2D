
#ifndef P2QUADTREE_H
#define P2QUADTREE_H

#include <cstdlib>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include <stdlib.h>

#include "../p2body.h"

#include "p2qtnode.h"
#include <SDL.h>


using namespace std;

#define LOWER_LEFT_QUAD 0
#define UPPER_LEFT_QUAD 1
#define LOWER_RIGHT_QUAD 2
#define UPPER_RIGHT_QUAD 3


namespace p2
{
    enum enclosure_status
    {
        NODE_NOT_IN_REGION,
        NODE_PARTIALLY_IN_REGION,
        NODE_CONTAINED_BY_REGION
    };

    template <typename T>
    class QT
    {
    public:

        QT<T>(Vector2D center, Vector2D range, unsigned bucketSize = 1, unsigned depth = 16);
        ~QT();

        //void            insert(Vector2D v, T data);
        void            insert(p2::body* p2Body, T data);
        bool            contains(Vector2D v);
        bool            remove(Vector2D v);
        void            draw(SDL_Renderer* renderer);
        std::string     print();
        vector <pair <Vector2D, T> > getObjectsInRegion(Vector2D minXY, Vector2D maxXY);

    private:

        QTNode<T>*  childNode(const Vector2D& v, QTNode<T>* node);
        Vector2D    newCenter(int direction, QTNode <T>* node);
        int         direction(const Vector2D& point, QTNode <T>* node);
        //void        insert(Vector2D v, T data, QTNode<T>* node, unsigned depth);
        void        insert(p2::body* p2Body, T data, QTNode<T>* node, unsigned depth);
        void        reduce(stack <QTNode<T>*>& node);
        void        draw(QTNode<T>* node, SDL_Renderer* renderer);
        void        print(QTNode <T>* node, stringstream& ss);
        void        addAllPointsToResults(QTNode<T>* node, vector <pair <Vector2D, T> >& results);
        bool        pointInRegion(const Vector2D& point, const Vector2D& minXY, const Vector2D& maxXY);
        enclosure_status getEnclosureStatus(const Vector2D& center, const Vector2D& range, const Vector2D& minXY, const Vector2D& maxXY);

        QTNode<T>* root;
        unsigned maxDepth, maxBucketSize;
    };


    template <typename T>
    p2::QT<T>::QT(Vector2D center, Vector2D range, unsigned bucketSize, unsigned depth)
    {
        root = new p2::QTNode <T>(center, range);
        maxDepth = depth;
        maxBucketSize = bucketSize;
    }

    template <typename T>
    p2::QT<T>::~QT()
    {
        delete root;
    }

    /*template <typename T>
    void p2::QT<T>::insert(Vector2D v, T data)
    {
        insert(v, data, root, 0);
    }*/

    template <typename T>
    void p2::QT<T>::insert(p2::body* p2Body, T data)
    {
        insert(p2Body, data, root, 0);
    }

    template <typename T>
    int p2::QT<T>::direction(const Vector2D& point, p2::QTNode<T>* node)
    {
        // get the quadrant that would contain the vertex
        // in reference to a given start node
        unsigned X = 0, Y = 0;
        X |= ((point.x >= node->center.x) << 1);
        Y |= ((point.y >= node->center.y) << 0);
        return (X | Y);
    }

    template <typename T>
    p2::QTNode<T>* p2::QT<T>::childNode(const Vector2D& v, p2::QTNode<T>* node)
    {
        // get the next node that would contain the vertex
        // in reference to a given start node
        unsigned dir = direction(v, node);
        if (node->child[dir]) {
            return node->child[dir];
        }
        // node not found, so create it 
        else {
            Vector2D r(node->range.x / 2.0, node->range.y / 2.0);
            node->child[dir] = new p2::QTNode<T>(newCenter(dir, node), r);
            return node->child[dir];
        }
    }

    template <typename T>
    Vector2D p2::QT<T>::newCenter(int direction, p2::QTNode<T>* node)
    {
        Vector2D v(node->center.x, node->center.y);
        switch (direction) {
        case LOWER_LEFT_QUAD:
            v.x -= node->range.x / 2.0;
            v.y -= node->range.y / 2.0;
            break;
        case UPPER_LEFT_QUAD:
            v.x -= node->range.x / 2.0;
            v.y += node->range.y / 2.0;
            break;
        case LOWER_RIGHT_QUAD:
            v.x += node->range.x / 2.0;
            v.y -= node->range.y / 2.0;
            break;
        case UPPER_RIGHT_QUAD:
            v.x += node->range.x / 2.0;
            v.y += node->range.y / 2.0;
            break;
        }
        return v;
    }

    template <typename T>
    void p2::QT<T>::insert(p2::body* p2Body, T data, p2::QTNode<T>* node, unsigned depth)
    {
        // by design, vertices are stored only in leaf nodes
        // newly created nodes are leaf nodes by default
        if (node->leaf) {
            // there is room in this node's bucket
            if (node->bucket.size() < maxBucketSize) {
                node->bucket.push_back({ p2Body, data });
            }
            // bucket is full, so push all vertices to next depth,
            // clear the current node's bucket and make it a stem
            else if (depth < maxDepth) {
                node->leaf = false;
                insert(p2Body, data, childNode(p2Body->GetPosition(), node), depth + 1);
                for (int i = 0; i < node->bucket.size(); ++i) {
                    insert(node->bucket[i].first, data, childNode(node->bucket[i].first->GetPosition(), node), depth + 1);
                }
                node->bucket.clear();
            }
        }
        else {
            insert(p2Body, data, childNode(p2Body->GetPosition(), node), depth + 1);
        }
    }

    template <typename T>
    bool p2::QT<T>::remove(Vector2D v)
    {
        stack <p2::QTNode<T>*> nodes;
        nodes.push(root);
        p2::QTNode<T>* top = nodes.top();
        unsigned dir;

        // navigate to leaf node containing the vertex to be deleted
        while (!top->leaf) {
            dir = direction(v, top);
            if (top->child[dir]) {
                nodes.push(top->child[dir]);
                top = nodes.top();
            }
            else {
                return false;
            }
        }
        // linearly search bucket for target vertex
        for (int i = 0; i < top->bucket.size(); ++i) {
            // vertex found, delete from bucket
            if (top->bucket[i].first == v) {
                top->bucket.erase(top->bucket.begin() + i);
                reduce(nodes);
                return true;
            }
            else {
                return false;
            }
        }
    }

    template <typename T>
    void p2::QT<T>::reduce(stack <p2::QTNode<T>*>& nodes)
    {
        // once a vertex is removed from a leaf node's bucket
        // check to see if that node's parent can consume it
        // and all of it's sibling nodes
        bool canReduce = true;
        nodes.pop();
        while (canReduce && !nodes.empty()) {
            canReduce = true;
            p2::QTNode<T>* top = nodes.top();
            int numKeys = 0;
            for (int i = 0; i < 4; ++i) {
                if (top->child[i] && !top->child[i]->leaf) {
                    canReduce = false;
                    return;
                }
                else if (top->child[i] && top->child[i]->leaf) {
                    numKeys += top->child[i]->bucket.size();
                }
            }
            canReduce &= (numKeys <= maxBucketSize);
            if (canReduce) {
                for (int i = 0; i < 4; ++i) {
                    if (top->child[i]) {
                        for (int j = 0; j < top->child[i]->bucket.size(); ++j) {
                            top->bucket.push_back(top->child[i]->bucket[j]);
                        }
                        delete top->child[i];
                        top->child[i] = NULL;
                    }
                }
                top->leaf = true;
            }
            nodes.pop();
        }
        return;
    }

    template <typename T>
    bool p2::QT<T>::contains(Vector2D v)
    {
        return false;
    }

    template <typename T>
    string p2::QT<T>::print()
    {
        stringstream ss("");
        print(root, ss);
        return ss.str();
    }

    template <typename T>
    void p2::QT<T>::print(p2::QTNode<T>* node, stringstream& ss)
    {
        for (int i = 0; i < 4; ++i) {
            if (node->child[i]) {
                print(node->child[i], ss);
                for (int i = 0; i < node->bucket.size(); i++) {
                    ss << '{' << node->bucket[i].first.x << ','
                        << node->bucket[i].first.y << '}' << ' ';
                }
            }
        }
        return;
    }

    template <typename T>
    std::vector<std::pair <Vector2D, T>> p2::QT<T>::getObjectsInRegion(Vector2D minXY, Vector2D maxXY)
    {
        std::vector <std::pair <Vector2D, T> > results;
        queue <p2::QTNode<T>*> nodes;
        nodes.push(root);

        while (!nodes.empty()) {
            p2::QTNode<T>* top = nodes.front();
            if (top->leaf) {
                enclosure_status status = getEnclosureStatus(top->center, top->range, minXY, maxXY);
                switch (status) {
                    // this node is completely contained within the search region
                case NODE_CONTAINED_BY_REGION:
                    // add all elements to results
                    results.insert(results.end(), top->bucket.begin(), top->bucket.end());
                    break;

                    // this node is partially contained by the region
                case  NODE_PARTIALLY_IN_REGION:
                    // search through this leaf node's bucket
                    for (int i = 0; i < top->bucket.size(); ++i) {
                        // check if this point is in the region
                        if (pointInRegion(top->bucket[i].first, minXY, maxXY)) {
                            results.push_back(top->bucket[i]);
                        }
                    }
                    break;

                    // this node definitely has no points in the region
                case NODE_NOT_IN_REGION:
                    // do nothing
                    break;
                }
            }
            else {
                for (int i = 0; i < 4; ++i) {
                    if (top->child[i]) {
                        // check if this nodes children could have points in the region
                        enclosure_status status = getEnclosureStatus(top->child[i]->center, top->child[i]->range, minXY, maxXY);
                        switch (status) {
                            // this node is completely contained by region, add all points within
                        case NODE_CONTAINED_BY_REGION:
                            addAllPointsToResults(top->child[i], results);
                            break;

                            // this node might contain points in the region
                        case NODE_PARTIALLY_IN_REGION:
                            nodes.push(top->child[i]);
                            break;

                            // no points in region, discontinue searching this branch
                        case NODE_NOT_IN_REGION:
                            break;
                        }
                    }
                }
            }
            nodes.pop();
        }
        return results;
    }

    template <typename T>
    void p2::QT<T>::addAllPointsToResults(p2::QTNode<T>* node, std::vector<std::pair <Vector2D, T> >& results)
    {
        if (node->leaf) {
            results.insert(results.end(), node->bucket.begin(), node->bucket.end());
        }
        else {
            for (int i = 0; i < 4; ++i) {
                if (node->child[i]) {
                    addAllPointsToResults(node->child[i], results);
                }
            }
        }
    }

    template <typename T>
    bool p2::QT<T>::pointInRegion(const Vector2D& point, const Vector2D& minXY, const Vector2D& maxXY)
    {
        if ((point.x >= minXY.x) && (point.x < maxXY.x) && (point.y >= minXY.y) && (point.y < maxXY.y)) {
            return true;
        }
        else {
            return false;
        }
    }

    template <typename T>
    p2::enclosure_status p2::QT<T>::getEnclosureStatus(const Vector2D& center, const Vector2D& range, const Vector2D& minXY, const Vector2D& maxXY)
    {
        int enclosedPts = 0;
        enclosedPts += pointInRegion({ center.x - range.x, center.y - range.y }, minXY, maxXY);
        enclosedPts += pointInRegion({ center.x - range.x, center.y + range.y }, minXY, maxXY);
        enclosedPts += pointInRegion({ center.x + range.x, center.y - range.y }, minXY, maxXY);
        enclosedPts += pointInRegion({ center.x + range.x, center.y + range.y }, minXY, maxXY);

        if (enclosedPts == 4) {
            return NODE_CONTAINED_BY_REGION;
        }
        else if (enclosedPts > 0) {
            return NODE_PARTIALLY_IN_REGION;
        }
        else {
            Vector2D nodeMin(center.x - range.x, center.y - range.y);
            Vector2D nodeMax(center.x + range.x, center.y + range.y);
            enclosedPts += pointInRegion(minXY, nodeMin, nodeMax);
            enclosedPts += pointInRegion({ minXY.x, maxXY.y }, nodeMin, nodeMax);
            enclosedPts += pointInRegion(maxXY, nodeMin, nodeMax);
            enclosedPts += pointInRegion({ maxXY.x, minXY.y }, nodeMin, nodeMax);
            if (enclosedPts > 0) {
                return NODE_PARTIALLY_IN_REGION;
            }
        }
        return NODE_NOT_IN_REGION;
    }

    template <typename T>
    void p2::QT<T>::draw(SDL_Renderer* renderer)
    {
        if (root) {
            draw(root, renderer);
        }
    }

    template <typename T>
    void p2::QT<T>::draw(p2::QTNode<T>* node, SDL_Renderer* renderer)
    {

        SDL_Rect rect;

        SDL_RenderDrawLine(renderer, node->center.x, node->center.y, node->center.x + node->range.x, node->center.y + node->range.y);
        SDL_RenderDrawLine(renderer, node->center.x, node->center.y, node->center.x + node->range.x, node->center.y - node->range.y);
        SDL_RenderDrawLine(renderer, node->center.x, node->center.y, node->center.x - node->range.x, node->center.y - node->range.y);
        SDL_RenderDrawLine(renderer, node->center.x, node->center.y, node->center.x - node->range.x, node->center.y + node->range.y);

       /* int startx = node->center.x + node->range.x;
        int starty = node->center.y + node->range.y;
        int endx = node->center.x - node->range.x;
        int endy = node->center.y - node->range.y;

        SDL_RenderDrawLine(renderer, node->center.x, node->center.y, node->center.x + node->range.x * 2, node->center.y);
        SDL_RenderDrawLine(renderer, node->center.x, node->center.y, node->center.x, node->center.y + node->range.y * 2);

        SDL_RenderDrawLine(renderer, 
            node->center.x, 
            node->center.y + node->range.y * 2, 
            node->center.x + node->range.x * 2, 
            node->center.y + node->range.y * 2);

        SDL_RenderDrawLine(renderer,
            node->center.x + node->range.x * 2,
            node->center.y,
            node->center.x + node->range.x * 2,
            node->center.y + node->range.y * 2);

        rect.x = static_cast<int>(node->center.x);
        rect.y = static_cast<int>(node->center.y);
        rect.w = node->range.x;
        rect.h = node->range.y;
        SDL_RenderDrawRect(renderer, &rect);*/



        rect.x = static_cast<int>(node->center.x + node->range.x);
        rect.y = static_cast<int>(node->center.y + node->range.y);
        rect.w = 20;
        rect.h = 20;
        SDL_RenderFillRect(renderer, &rect);

        for (int i = 0; i < node->bucket.size(); ++i) {
            //Vector2D vect(node->center.x, node->center.y);
            //Vector2D vect(node->bucket[i].first.x, node->bucket[i].first.y);
            Vector2D vect = node->bucket[i].first->GetPosition();
            rect.x = static_cast<int>(vect.x);
            rect.y = static_cast<int>(vect.y);
            rect.w = 20;
            rect.h = 20;
            SDL_RenderFillRect(renderer, &rect);
        }

        /*
        glBegin (GL_LINE_LOOP);
            glVertex2f (node->center.x + node->range.x, node->center.y + node->range.y);
            glVertex2f (node->center.x + node->range.x, node->center.y - node->range.y);
            glVertex2f (node->center.x - node->range.x, node->center.y - node->range.y);
            glVertex2f (node->center.x - node->range.x, node->center.y + node->range.y);
        glEnd();
        */
        /* glBegin(GL_LINES);

         glVertex2f(node->center.x, node->center.y);
         glVertex2f(node->center.x + node->range.x, node->center.y + node->range.y);

         glVertex2f(node->center.x, node->center.y);
         glVertex2f(node->center.x + node->range.x, node->center.y - node->range.y);

         glVertex2f(node->center.x, node->center.y);
         glVertex2f(node->center.x - node->range.x, node->center.y - node->range.y);

         glVertex2f(node->center.x, node->center.y);
         glVertex2f(node->center.x - node->range.x, node->center.y + node->range.y);

         for (int i = 0; i < node->bucket.size(); ++i) {
             glVertex2f(node->center.x, node->center.y);
             glVertex2f(node->bucket[i].first.x, node->bucket[i].first.y);
         }

         glEnd();*/

         for (int i = 0; i < 4; ++i)
         {
             if (node->child[i])
             {
                 draw(node->child[i], renderer);
             }
         }
    };
};

#endif