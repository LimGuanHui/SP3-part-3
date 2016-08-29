#include "Panel.h"

class ENEMY
{
public:
    ENEMY();
    ~ENEMY();

    void Init();
    void update();

    void gethp();
    void sethp();

    void getatk();
    void setatk();
        
private:
    int hp;
    int atk;
};

ENEMY::ENEMY()
{
}

ENEMY::~ENEMY()
{
}