#include "P_Player.h"
#include "Application.h"
#define timerdelay 0.2f
Player::Player()
{

}
Player::~Player()
{

}
void Player::Init(std::vector<Panel *> *Panel_List)
{
    this->Panel_List = Panel_List;
    panel_no = 4;
    inputimer = timerdelay;
    charging = false;
    chargetimer = 0.f;
    maxcharge = 3.f;
    GunState = Normal;
    fireshot = false;
    panel_pos = Panel::PanelPos::Middle;
    pos = Vector3(0, 0, 0);
}
void Player::update(double dt)
{
    for (std::vector<Panel *>::iterator it = Panel_List->begin(); it != Panel_List->end(); ++it)
    {
        Panel* go = (Panel*)*it;
        if (panel_no == go->getPanelNo())
        {
            panel_pos = go->panel_pos;
            pos = go->getpos();
            break;
        }
    }
    if (inputimer >= 0)
        inputimer -= dt;
    if (Application::IsKeyPressed('W') && inputimer < 0)
    {
        inputimer = timerdelay;
        if (panel_no != 2 && panel_no != 5 && panel_no != 8)
            panel_no++;
    }
    else if (Application::IsKeyPressed('S') && inputimer < 0)
    {
        inputimer = timerdelay;
        if (panel_no != 0 && panel_no != 3 && panel_no != 6)
            panel_no--;

    }
    else if (Application::IsKeyPressed('A') && inputimer < 0)
    {
        inputimer = timerdelay;
        if (panel_no != 0 && panel_no != 1 && panel_no != 2)
            panel_no -= 3;
    }
    else if (Application::IsKeyPressed('D') && inputimer < 0)
    {
        inputimer = timerdelay;
        if (panel_no != 6 && panel_no != 7 && panel_no != 8)
            panel_no += 3;
    }
    if (Application::IsKeyPressed('K'))
    {
        charging = true;
        chargetimer += dt;
    }
    else
    {
        if (chargetimer >= maxcharge)
            GunState = Charge;
        else
            GunState = Normal;
        charging = false;
        chargetimer = 0;
        fireshot = true;
    }
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
    if (GunState == Charge)
        return atk * 2;
    return atk;
}
void Player::setatk(int atk)
{
    this->atk = atk;
}

int Player::getpanelNo()
{
    return panel_no;
}

bool Player::fire()
{
    if (fireshot)
    {
        fireshot = false;
        return true;
    }
    return fireshot;
}

Vector3 Player::getpos()
{
    return pos;
}

void Player::Exit()
{
    Projecile_List.clear();
}