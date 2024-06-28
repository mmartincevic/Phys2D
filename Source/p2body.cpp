#include "p2body.h"


p2::body::body(const p2::bodyDef* bd)
{
    m_bTran.p = bd->position;
    m_bTran.q.Set(bd->angle);
}