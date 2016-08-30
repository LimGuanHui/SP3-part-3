#include "P_Enemy.h"
#include "MyMath.h"
#define movetimerdelay 0.5f
#define atktimerdelay 3.5f
#define atkdelay 1.f
ENEMY::ENEMY(std::vector<Panel *> *Panel_List) :
Panel_List(Panel_List)
{

}
ENEMY::~ENEMY()
{

}

void ENEMY::Init()
{
    panel_no = 4;
    movetimer = movetimerdelay;
    attacktimer = atktimerdelay;
    atk_delay = 0;
    isAtking = false;
    state = Move;
    panel_pos = Panel::PanelPos::Middle;
}
void ENEMY::update(double dt)
{
    for (std::vector<Panel *>::iterator it = Panel_List->begin(); it != Panel_List->end(); ++it)
    {
        Panel* go = (Panel*)*it;
        if (panel_no == go->getPanelNo() && go->who == Panel::BELONGS_TO::ENEMY)
        {
            panel_pos = go->panel_pos;
            pos = go->getpos();
            break;
        }
    }

    if (hp <= 0)
    {
        state = Die;
    }
    switch (state)
    {
    case ENEMY::Move:
        if (movetimer >= 0)
            movetimer -= dt;
        if (attacktimer >= 0)
            attacktimer -= dt;
        if (movetimer < 0)
        {
            movetimer = movetimerdelay;
            if (attacktimer < 0)
            {
                state = Attack;
                break;
            }
            int rand = panel_no;
            while (rand == panel_no)
                rand = Math::RandIntMinMax(0, 8);
            panel_no = rand;
        }
        break;
    case ENEMY::Attack:

        attacktimer = atktimerdelay;
        break;
    case ENEMY::Die:
        break;
    default:
        break;
    }
}

int ENEMY::gethp()
{
    return hp;
}
void ENEMY::sethp(int hp)
{
    this->hp = hp;
}

int ENEMY::getatk()
{
    return atk;
}
void ENEMY::setatk(int atk)
{
    this->atk = atk;
}

int ENEMY::getpanelNo()
{
    return panel_no;
}

void ENEMY::gethit(int dmgtaken)
{
    hp -= dmgtaken;
}

Vector3 ENEMY::getpos()
{
    return pos;
}

void ENEMY::Exit()
{
    Projecile_List.clear();
}