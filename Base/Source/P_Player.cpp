#include "P_Player.h"

Player::Player()
{

}
Player::~Player()
{

}

int Player::gethp()
{
    return hp;
}
void Player::sethp(int hp)
{
    this->hp = hp;
}

int Player::getatk()
{
    return atk;
}
void Player::setatk(int atk)
{
    this->atk = atk;
}
