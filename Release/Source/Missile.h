#ifndef MISSILE_H
#define MISSILE_H
class Missile
{
public:
    Missile();
    ~Missile();
    void Init(float x, float y);

    float x, y;
    bool active;
    bool collided;
    void update(float dt, float player_x, float player_y);
    
};

#endif