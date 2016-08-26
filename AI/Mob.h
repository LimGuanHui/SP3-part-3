#ifndef MOB_H_
#define MOB_H_

#ifndef MOB_AI_EXPORTS
#define MOB_AI_API __declspec(dllexport) 
#else
#define MOB_AI_API __declspec(dllimport) 
#endif

#include "Vector3.h"

class MOB_AI_API Mob
{
public:
    Mob(float reactionDistance);
    ~Mob();
        
    enum State
    {
        Spawn,
        Idle,
        Attack,
        Die,
    };

    State state;
    float ReactDist;
    Vector3 Position;

        
    void Init();
    void update();
};

MOB_AI_API Mob* CreateMob(float reactionDistance);


#endif