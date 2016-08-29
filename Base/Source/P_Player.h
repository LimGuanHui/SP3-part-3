#ifndef P_PLAYER_H
#define P_PLAYER_H

#include "Panel.h"
#include "P_Projectile.h"
class Player
{   
public:
    enum gunstate
    {
        Normal,
        Charge,
    };
    
    Player();
    ~Player();

    void Init(std::vector<Panel *> *Panel_List);
    void Exit();
    void update(double dt);
    bool fire();

    int gethp();
    void sethp(int hp);

    int getatk();
    void setatk(int atk);

    int getpanelNo();
    
    Vector3 getpos();

    gunstate GunState;
    Panel::PanelPos panel_pos;
    std::vector<P_Projectile* >Projecile_List;
    std::vector<Panel *> *Panel_List;
private:
    int hp;
    int atk;
    int panel_no;
    float inputimer;
    bool charging;
    float chargetimer;
    float maxcharge;
    bool fireshot;

    Vector3 pos;
};

#endif