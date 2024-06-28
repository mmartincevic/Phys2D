#ifndef P2BODY_H
#define P2BODY_H

#include "p2bodyDef.h"
#include "p2transform.h"

namespace p2
{

    class body  {
    public:
        body(const p2::bodyDef* bd);
        ~body() {}

        inline const Vector2D& GetPosition() const
        {
            return m_bTran.p;
        }

    private:
        p2::bTran m_bTran;
    };
}

#endif