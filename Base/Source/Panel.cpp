#include "Panel.h"


Panel::Panel()
{
}


Panel::~Panel()
{
}

void Panel::Init(int panelNo, Vector3 startpos, float offset, PanelType panel_type)
{
    this->panel_type = panel_type;
    this->offset = offset;
    panel_state = Empty;
    Scale = Vector3(22.5, 15.f, 1);
    switch (panelNo)
    {
    case 0:
        Pos = startpos;
        PanelNo = panelNo;
        panel_pos = Bottom;
        break;
    case 1:
        Pos = Vector3(startpos.x, startpos.y + offset, startpos.z);
        PanelNo = panelNo;
        panel_pos = Middle;
        break;
    case 2:
        Pos = Vector3(startpos.x, startpos.y + offset * 2, startpos.z);
        PanelNo = panelNo;
        panel_pos = Top;
        break;
    case 3:
        Pos = Vector3(startpos.x + offset * 1.5f, startpos.y, startpos.z);
        PanelNo = panelNo;
        panel_pos = Bottom;
        break;
    case 4:
        Pos = Vector3(startpos.x + offset * 1.5f, startpos.y + offset, startpos.z);
        PanelNo = panelNo;
        panel_pos = Middle;
        break;
    case 5:
        Pos = Vector3(startpos.x + offset * 1.5f, startpos.y + offset * 2, startpos.z);
        PanelNo = panelNo;
        panel_pos = Top;
        break;
    case 6:
        Pos = Vector3(startpos.x + offset * 2 * 1.5f, startpos.y, startpos.z);
        PanelNo = panelNo;
        panel_pos = Bottom;
        break;
    case 7:
        Pos = Vector3(startpos.x + offset * 2 * 1.5f, startpos.y + offset, startpos.z);
        PanelNo = panelNo;
        panel_pos = Middle;
        break;
    case 8:
        Pos = Vector3(startpos.x + offset * 2 * 1.5f, startpos.y + offset * 2, startpos.z);
        PanelNo = panelNo;
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
