#include "BattleStage.h"

BattleStage::BattleStage()
{

}
BattleStage::~BattleStage()
{
    exit();
}

void BattleStage::Init(float ortho_x, float ortho_y,float panelsize)
{
    panelsize_x = (ortho_x*0.5) * pow(3, -1);
    panelsize_y = (ortho_y*0.5) * pow(3, -1);
    float panelscale_x = 1;
    for (int i = 0; i < 7; i++)//player panel
    {
        Panel* NewPanel = new  Panel();
        NewPanel->Init(i, Vector3(panelsize_x *0.5, panelsize_y *0.5, 0),
            Vector3(panelscale_x* (panelsize_x * pow(panelsize_y, -1)), panelsize_y, 1),
            panelsize_x, panelsize_y, Panel::PanelType::Normal, Panel::BELONGS_TO::PLAYER);
        Panel_List.push_back(NewPanel);
    }
    float tempx = ortho_x * 0.5;
    float tempy = ortho_y * 0.5;
    for (int i = 0; i < 7; i++)//enemy panel
    {
        Panel* NewPanel = new  Panel();
        NewPanel->Init(i, Vector3(tempx + panelsize_x *0.5, tempy + panelsize_y *0.5, 0),
            Vector3(panelscale_x* (panelsize_x * pow(panelsize_y, -1)), panelsize_y, 1),
            panelsize_x, panelsize_y, Panel::PanelType::Normal, Panel::BELONGS_TO::ENEMY);
        Panel_List.push_back(NewPanel);
    }
    player = new Player();
    //set player stats here
    player->Init(&Panel_List);
    player->sethp(100);
    player->setatk(10);
    enemy = new ENEMY(&Panel_List);
    //set enemy stats here
    enemy->sethp(200);
    enemy->setatk(20);
}
void BattleStage::Update(double dt)
{
    if (player->fire())
    {
        //check if player and enemy on same column
        if (player->panel_pos == enemy->panel_pos)
        {
            enemy->gethit(player->getatk());
        }
    }
    player->update(dt);
    enemy->update(dt);
}

void BattleStage::RenderObjects(Mesh* tpanel, Mesh* mpanel, Mesh* bpanel, Mesh* player, Mesh* enemy )
{
    for (std::vector<Panel *>::iterator it = Panel_List.begin(); it != Panel_List.end(); ++it)
    {
        Panel* go = (Panel*)*it;
        switch (go->panel_pos)
        {
        case Panel::PanelPos::Top:
            Render2DMesh(tpanel, false, go->getscale().x, go->getscale().y, go->getpos().x, go->getpos().y, false, false);
            break;
        case Panel::PanelPos::Middle:
            Render2DMesh(mpanel, false, go->getscale().x, go->getscale().y, go->getpos().x, go->getpos().y, false, false);
            break;
        case Panel::PanelPos::Bottom:
            Render2DMesh(bpanel, false, go->getscale().x, go->getscale().y, go->getpos().x, go->getpos().y, false, false);
            break;
        default:
            break;
        }
        
    }
    //render character
    Render2DMesh(player, false,/*player x scale*/ 25, /*player y scale*/ 25, this->player->getpos().x, this->player->getpos().y, false, false);
    //render enemy
    Render2DMesh(enemy, false,/*enemy x scale*/ 25, /*enemy y scale*/ 25, this->enemy->getpos().x, this->enemy->getpos().y, false, false);

}

void BattleStage::exit()
{
    player->Exit();
    enemy->Exit();
    Panel_List.clear();
    delete player;
    delete enemy;
}