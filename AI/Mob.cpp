#include "Mob.h"

Mob::Mob(float reactionDistance)
{
    ReactDist = reactionDistance;
}
Mob::~Mob()
{

}

void Mob::Init()
{
    state = Spawn;
}
void Mob::update()
{
    switch (state)
    {
    case Mob::Spawn:
        break;
    case Mob::Idle:
        break;
    case Mob::Attack:
        break;
    case Mob::Die:
        break;
    default:
        break;
    }
}

Mob* CreateMob( float reactionDistance)
{
    return new Mob(reactionDistance);
}


