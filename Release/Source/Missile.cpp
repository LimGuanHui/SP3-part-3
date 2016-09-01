#include "Missile.h"


Missile::Missile()
{
    x = 0;
    y = 0;
    active = false;
    collided = false;
}

void Missile::Init(float x, float y)
{
    this->x = x;
    this->y = y;
    active = true;
    collided = false;
}

Missile::~Missile()
{
}

void Missile::update(float dt, float player_x, float player_y)
{
    x += dt * 350.f;

    {
        float tempx = (player_x - x), tempy = (player_y - y);
        if (tempx < 0)
            tempx = (player_x - x) *-1;
        if (tempy < 0)
            tempy = (player_y - y)* -1;
        if (tempx < 32 && tempy < 32)
            collided = true;

        if (player_x - x > (32 * 16))
            active = false;
    }

}
