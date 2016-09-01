#include "Particle.h"

ParticleObject::ParticleObject(ParticleObject_TYPE typeValue)
: type(typeValue),
pos(0, 0, 0),
scale(1, 1, 1),
rotation(0),
rotationSpeed(0),
active(false),
renderOrder(0),
distFromPlayer(0),
PutInOrder(false),
speed(10.f),
lifetime(0)
{
}

ParticleObject::~ParticleObject(void)
{

}

void ParticleObject::update(double dt)
{
    lifetime -= dt;
    pos += vel * (float)dt * speed;
    if (lifetime <= 0)
    {
        active = false;
    }
}