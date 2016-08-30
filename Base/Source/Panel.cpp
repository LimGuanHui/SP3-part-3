#include "Panel.h"


Panel::Panel()
{
}


Panel::~Panel()
{
}

void Panel::Init(int panelNo, Vector3 startpos, Vector3 scale, 
    float offset_X, float offset_Y, PanelType panel_type, BELONGS_TO who)
{
    this->panel_type = panel_type;
    this->who = who;
    panel_state = Empty;
    Scale = scale;
    PanelNo = panelNo;
    switch (PanelNo)
    {
    case 0:
        Pos = startpos;
        panel_pos = Bottom;
        break;
    case 1:
        Pos = Vector3(startpos.x, startpos.y + offset_Y, startpos.z);
        panel_pos = Middle;
        break;
    case 2:
        Pos = Vector3(startpos.x, startpos.y + offset_Y * 2, startpos.z);
        panel_pos = Top;
        break;
    case 3:
        Pos = Vector3(startpos.x + offset_X , startpos.y, startpos.z);
        panel_pos = Bottom;
        break;
    case 4:
        Pos = Vector3(startpos.x + offset_X, startpos.y + offset_Y, startpos.z);
        panel_pos = Middle;
        break;
    case 5:
        Pos = Vector3(startpos.x + offset_X, startpos.y + offset_Y * 2, startpos.z);
        panel_pos = Top;
        break;
    case 6:
        Pos = Vector3(startpos.x + offset_X * 2, startpos.y, startpos.z);
        panel_pos = Bottom;
        break;
    case 7:
        Pos = Vector3(startpos.x + offset_X * 2, startpos.y + offset_Y, startpos.z);
        panel_pos = Middle;
        break;
    case 8:
        Pos = Vector3(startpos.x + offset_X * 2 , startpos.y + offset_Y * 2, startpos.z);
        panel_pos = Top;
        break;
    default:
        break;
    }

}

void Panel::update(int *playerhp)
{

}

Vector3 Panel::getpos()
{
    return Pos;
}
Vector3 Panel::getscale()
{
    return Scale;
}

int Panel::getPanelNo()
{
    return PanelNo;
}

