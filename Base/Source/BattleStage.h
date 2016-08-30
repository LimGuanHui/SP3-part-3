#include "P_Player.h"
#include "P_Enemy.h"
#include "SceneBase.h"
class BattleStage
{
public:
    BattleStage();
    ~BattleStage();
    
    void Init(float ortho_x, float ortho_y, float panelsize);
    void Update(double dt);
    void RenderObjects(Mesh* tpanel, Mesh* mpanel, Mesh* bpanel, 
        Mesh* player, Mesh* enemy, Mesh* text);

    float getpanel_sizeX();
    float getpanel_sizeY();

    void exit();
    Player* player;
    ENEMY* enemy;
    std::vector<Panel *> Panel_List;
private:
    
    float panelsize_x, panelsize_y;
    float custgauge, MAX_custgauge;

};
