#ifndef P_PROJECTILE_H
#define P_PROJECTILE_H
#include "Vector3.h"

class P_Projectile
{
public:
    enum SIDE
    {
        ENEMY,
        PLAYER,
    };
    P_Projectile(Vector3 pos, Vector3 vel, SIDE side);
    ~P_Projectile();

    void update(double dt);
    SIDE side;
private:
    Vector3 pos;
    Vector3 vel;
    int panelpos;
    
};

#endif