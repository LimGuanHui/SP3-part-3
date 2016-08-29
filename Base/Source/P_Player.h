#include "Panel.h"

class Player
{   
public:
    Player();
    ~Player();

    int gethp();
    void sethp(int hp);

    int getatk();
    void setatk(int atk);
private:
    int hp;
    int atk;
};

Player::Player()
{
}

Player::~Player()
{
}