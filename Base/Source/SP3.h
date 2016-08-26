#ifndef SP3_H
#define SP3_H

//#include "Scene.h"
//#include "Mtx44.h"
//#include "Camera3.h"
//#include "Mesh.h"
//#include "MatrixStack.h"
//#include "Light.h"
#include "SceneBase.h"
#include "Minimap.h"
#include "Vector2.h"
#include "PlayerInfo.h"
#include "Enemy.h"
#include "Missile.h"
#include <irrKlang.h>
using namespace irrklang;
#include "Character.h"
using namespace CHARACTER;
#include "Monster.h"
using namespace MONSTER;
#include "Projectile.h"
#include "Collision.h"


class SP3 : public SceneBase
{
    enum GameState
    {
        Menu,
        Game,
        End,

    };

public:
    SP3();
    ~SP3();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

    void RenderTextOnScreen2(Mesh* mesh, std::string text, Color color, float size, float x, float y);
    void RenderBackground();
    Missile* FetchMissile();
    void MissileUpdate(float dt);

	void RenderProjectile(PROJECTILE::Projectile* projectile);

    void GameStateRenderText();
    void GameStateUpdate();
    void GameStateRender();

    void Scenetransition();
    void SpawnObjects();

	void RenderCharacter();
    void RenderList();

    void ProjectileCollision(double dt);
    void MonsterUpdate(double dt);


    enum Level
    {
        LEVEL1,
        LEVEL2,
        LEVEL3,
        LEVEL4,
    };

	CCharacter* Character;

	MapLoad* LoadFile;

private:
    float rotateAngle;   

    // Handle to the minimap
    CMinimap* m_cMinimap;

    // Handle to the tilemaps
    
    void RenderTileMap();
    // Hero's information
    CPlayerInfo* theHero;

    // Codes for Scrolling
    int tileOffset_x, tileOffset_y;

    // Codes for Parallax Scrolling
    MapLoad* m_cRearMap;
    void RenderRearTileMap();
    int rearWallOffset_x, rearWallOffset_y;
    int rearWallTileOffset_x, rearWallTileOffset_y;
    int rearWallFineOffset_x, rearWallFineOffset_y;

    // Enemies
    std::vector<Monster*> Monster_List;

    std::vector<Missile*> MissileList;
    float missileTriggerTimer;

	float firingDebounce;
	static const int fireRate = 10;
	bool Fire;
	bool chargeFire;
	float chargeTime;

    int lives;
	bool Moving;

    GameState State = Menu;

    //sound
    ISoundEngine* sceneSoundEngine;
    /*ISoundSource* shootBulletSound;
    ISoundSource* win;
    ISoundSource* lose;*/
    ISoundSource* jump;
    float jumpsoundtimer;

    //level transition
    Level CurrLevel;
};

#endif