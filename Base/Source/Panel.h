#ifndef PANEL_H
#define PANEL_H

#include "Vector3.h"
#include <vector>
class Panel
{
public:
    enum PanelPos
    {
        Top,
        Middle,
        Bottom,
    };

    enum PanelType
    {
        Normal,
    };

    enum PanelState
    {
        Empty,
        Occupied,
        HasDMG,
        Occupied_and_HasDMG,
    };

    enum BELONGS_TO
    {
        PLAYER,
        ENEMY,
    };

    Panel();
    ~Panel();
    void Init(int panelNo, Vector3 startpos, Vector3 scale, 
        float offset_X, float offset_Y, PanelType panel_type, BELONGS_TO who);
    void update(int *playerhp);
    Vector3 getpos();
    Vector3 getscale();
    int getPanelNo();

    PanelPos panel_pos;
    PanelType panel_type;
    PanelState panel_state;
    BELONGS_TO who;
private:
    Vector3 Pos;
    Vector3 Scale;
    int PanelNo;
    
        
};



#endif