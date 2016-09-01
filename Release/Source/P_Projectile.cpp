#include "P_Projectile.h"

P_Projectile::P_Projectile(Vector3 pos, Vector3 vel, SIDE side) :
pos(pos),
vel(vel),
side(side)
{
}

P_Projectile::~P_Projectile()
{
}

void P_Projectile::update(double dt)
{
    pos += vel * dt;
}