#ifndef P_ENEMY_H
#define P_ENEMY_H

#include "Panel.h"
#include "P_Projectile.h"
class ENEMY
{
public:
    enum State
    {   
        Move,
        Attack,
        Die,
    };
    ENEMY(std::vector<Panel *> *Panel_List);
    ~ENEMY();

    void Init();
    void Exit();
    void update(double dt, Panel::PanelPos player_panel_pos);

    int gethp();
    void sethp(int hp);

    int getatk();
    void setatk(int atk);
    int getpanelNo();

    void gethit(int dmgtaken);
    Vector3 getpos();

    bool isAttacking();

    State state;
    Panel::PanelPos panel_pos;
    std::vector<P_Projectile* >Projecile_List;
    std::vector<Panel *> *Panel_List;
private:
    int hp;
    int atk;
    int panel_no;
    float movetimer;
    float attacktimer;
    float atk_delay;
    bool isAtking;
    Vector3 pos;
};

#endif