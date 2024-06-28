#ifndef P2TRANSFORM_H
#define P2TRANSFORM_H

#include <math.h>
#include "Utils/Vector2D.h"

#define p2Sqrt(x)   sqrtf(x)
#define p2Atan2(y, x)   atan2f(y, x)

inline bool b2IsValid(float x)
{
    return isfinite(x);

}
namespace p2
{
    struct bRot
    {
        bRot() = default;

        /// Initialize from an angle in radians
        explicit bRot(float angle)
        {
            /// TODO_ERIN optimize
            s = sinf(angle);
            c = cosf(angle);
        }

        /// Set using an angle in radians.
        void Set(float angle)
        {
            s = sinf(angle);
            c = cosf(angle);
        }

        /// Set to the identity rotation
        void SetIdentity()
        {
            s = 0.0f;
            c = 1.0f;
        }

        /// Get the angle in radians
        float GetAngle() const
        {
            return p2Atan2(s, c);
        }

        /// Get the x-axis
        Vector2D GetXAxis() const
        {
            return Vector2D(c, s);
        }

        /// Get the u-axis
        Vector2D GetYAxis() const
        {
            return Vector2D(-s, c);
        }

        /// Sine and cosine
        float s, c;
    };


    struct bTran
    {
        /// The default constructor does nothing.
        bTran() = default;

        /// Initialize using a position vector and a rotation.
        bTran(const Vector2D& position, const p2::bRot& rotation) : p(position), q(rotation) {}

        /// Set this to the identity transform.
        void SetIdentity()
        {
            //p.SetZero();
            q.SetIdentity();
        }

        void Set(const Vector2D& position, float angle)
        {
            p = position;
            q.Set(angle);
        }

        Vector2D p;
        p2::bRot q;
    };
};
#endif