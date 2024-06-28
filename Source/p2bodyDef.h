#ifndef P2_BODYDEF_H
#define P2_BODYDEF_H

#include "Utils/Vector2D.h"

namespace p2
{

    enum bodyType
    {
        STATIC = 0,
        RIGID,
        DYNAMIC
    };

    struct bodyDef
    {
        /// This constructor sets the body definition default values.
        bodyDef()
        {
            position = Vector2D(0.0f, 0.0f);
            angle = 0.0f;
            linearVelocity = Vector2D(0.0f, 0.0f);
            angularVelocity = 0.0f;
            linearDamping = 0.0f;
            angularDamping = 0.0f;
            allowSleep = true;
            awake = true;
            fixedRotation = false;
            bullet = false;
            type = p2::bodyType::STATIC;
            enabled = true;
            gravityScale = 1.0f;
        }

        /// The body type: static, rigid, or dynamic.
        p2::bodyType type;

        /// The world position of the body. Avoid creating bodies at the origin
        /// since this can lead to many overlapping shapes.
        Vector2D position;

        /// The world angle of the body in radians.
        float angle;

        /// The linear velocity of the body's origin in world co-ordinates.
        Vector2D linearVelocity;

        /// The angular velocity of the body.
        float angularVelocity;

        /// Linear damping is use to reduce the linear velocity. The damping parameter
        /// can be larger than 1.0f but the damping effect becomes sensitive to the
        /// time step when the damping parameter is large.
        /// Units are 1/time
        float linearDamping;

        /// Angular damping is use to reduce the angular velocity. The damping parameter
        /// can be larger than 1.0f but the damping effect becomes sensitive to the
        /// time step when the damping parameter is large.
        /// Units are 1/time
        float angularDamping;

        /// Set this flag to false if this body should never fall asleep. Note that
        /// this increases CPU usage.
        bool allowSleep;

        /// Is this body initially awake or sleeping?
        bool awake;

        /// Should this body be prevented from rotating? Useful for characters.
        bool fixedRotation;

        /// Is this a fast moving body that should be prevented from tunneling through
        /// other moving bodies? Note that all bodies are prevented from tunneling through
        /// kinematic and static bodies. This setting is only considered on dynamic bodies.
        /// @warning You should use this flag sparingly since it increases processing time.
        bool bullet;

        /// Does this body start out enabled?
        bool enabled;

        /// Use this to store application specific body data.
        //b2BodyUserData userData;

        /// Scale the gravity applied to this body.
        float gravityScale;
    };
};

#endif // !P2_BODYDEF_H
