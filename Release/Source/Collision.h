#ifndef COLLISION_H
#define COLLISION_H

#include "Vector3.h"
class Collision
{
public:
    Collision();
    ~Collision();

    static bool SphericalCollision(Vector3 object1_pos, Vector3 object1_scale, Vector3 object2_pos, Vector3 object2_scale);
    static bool SphericalCollision(Vector3 object1_pos, Vector3 object1_scale, Vector3 object1_vel,
        Vector3 object2_pos, Vector3 object2_scale, Vector3 object2_vel, double dt);
};

#endif