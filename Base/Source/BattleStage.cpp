#include "BattleStage.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
BattleStage::BattleStage()
{

}
BattleStage::~BattleStage()
{
    exit();
}

void BattleStage::Init(float ortho_x, float ortho_y, float panelsize)
{
    //SceneBase::Init();
    //panels
    //meshList[GEO_B_PANEL] = MeshBuilder::GenerateQuad("bPanel", Color(0, 1, 0), 1.f);
    //meshList[GEO_B_PANEL]->textureID = LoadTGA("Image//panels//bPanel.tga");

    //meshList[GEO_M_PANEL] = MeshBuilder::GenerateQuad("mPanel", Color(0, 1, 0), 1.f);
    //meshList[GEO_M_PANEL]->textureID = LoadTGA("Image//panels//mPanel.tga");

    //meshList[GEO_T_PANEL] = MeshBuilder::GenerateQuad("tPanel", Color(0, 1, 0), 1.f);
    //meshList[GEO_T_PANEL]->textureID = LoadTGA("Image//panels//tPanel.tga");

    panelsize_x = (ortho_x*0.5) * pow(3, -1);
    panelsize_y = (ortho_y*0.5) * pow(3, -1);
    float panelscale_x = 1;
    for (int i = 0; i < 9; i++)//player panel
    {
        Panel* NewPanel = new Panel();
        NewPanel->Init(i, Vector3(panelsize_x *0.5, panelsize_y *0.5, 0),
            Vector3(panelsize_x, panelsize_y, 1),
            panelsize_x, panelsize_y, Panel::PanelType::Normal, Panel::BELONGS_TO::PLAYER);
        Panel_List.push_back(NewPanel);
    }
    float tempx = ortho_x * 0.5;
    float tempy = ortho_y * 0.5;
    for (int i = 0; i < 9; i++)//enemy panel
    {
        Panel* NewPanel = new Panel();
        NewPanel->Init(i, Vector3(tempx + panelsize_x *0.5, panelsize_y *0.5, 0),
            Vector3(panelsize_x, panelsize_y, 1),
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
    enemy->Init();
    enemy->sethp(200);
    enemy->setatk(20);

    custgauge = 0;
    MAX_custgauge = 100;
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
    if (enemy->isAttacking())
    {
        //check if player and enemy on same column
        if (player->panel_pos == enemy->panel_pos)
        {
            player->gethit(enemy->getatk());
        }
    }
    player->update(dt);
    enemy->update(dt,player->panel_pos);
}

void BattleStage::RenderObjects(Mesh* tpanel, Mesh* mpanel, Mesh* bpanel, Mesh* player, Mesh* enemy, Mesh* text )
{    
}

float BattleStage::getpanel_sizeX()
{
    return panelsize_x;
}
float BattleStage::getpanel_sizeY()
{
    return panelsize_y;
}

void BattleStage::exit()
{
    player->Exit();
    enemy->Exit();
    Panel_List.clear();
    delete player;
    delete enemy;
}