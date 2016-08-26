#include "Collision.h"


Collision::Collision()
{
}


Collision::~Collision()
{
}

bool Collision::SphericalCollision(Vector3 object1_pos, Vector3 object1_scale, Vector3 object2_pos, Vector3 object2_scale,double dt)
{
    float distanceSquared = ((object1_pos) - (object2_pos)).LengthSquared();
    float combinedRadiusSquared = (object1_scale.x + object2_scale.x) * (object1_scale.x + object2_scale.x);
    if (distanceSquared < combinedRadiusSquared)
    {
        return true;
    }
    return false;
}

bool Collision::SphericalCollision(Vector3 object1_pos, Vector3 object1_scale, Vector3 object1_vel,
    Vector3 object2_pos, Vector3 object2_scale, Vector3 object2_vel, double dt)
{
    float distanceSquared = ((object1_pos + object1_vel * dt) - (object2_pos - object2_vel * dt)).LengthSquared();
    float combinedRadiusSquared = (object1_scale.x + object2_scale.x) * (object1_scale.x + object2_scale.x);
    if (distanceSquared < combinedRadiusSquared)
    {
        return true;
    }
    return false;
}