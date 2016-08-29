#include "P_Player.h"
#include "P_Enemy.h"
#include "SceneBase.h"
class BattleStage : public SceneBase
{
public:
    BattleStage();
    ~BattleStage();
    
    void Init(float ortho_x, float ortho_y, float panelsize);
    void Update(double dt);
    void RenderObjects(Mesh* tpanel, Mesh* mpanel, Mesh* bpanel, Mesh* player, Mesh* enemy);

    void exit();
    
private:
    std::vector<Panel *> Panel_List;
    float panelsize_x, panelsize_y;
    Player* player;
    ENEMY* enemy;
};
