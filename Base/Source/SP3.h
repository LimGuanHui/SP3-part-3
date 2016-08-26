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



class SP3 : public SceneBase
{
  //  enum UNIFORM_TYPE
  //  {
  //      U_MVP = 0,
  //      U_MODELVIEW,
  //      U_MODELVIEW_INVERSE_TRANSPOSE,
  //      U_MATERIAL_AMBIENT,
  //      U_MATERIAL_DIFFUSE,
  //      U_MATERIAL_SPECULAR,
  //      U_MATERIAL_SHININESS,
  //      U_LIGHTENABLED,
  //      U_NUMLIGHTS,
  //      U_LIGHT0_TYPE,
  //      U_LIGHT0_POSITION,
  //      U_LIGHT0_COLOR,
  //      U_LIGHT0_POWER,
  //      U_LIGHT0_KC,
  //      U_LIGHT0_KL,
  //      U_LIGHT0_KQ,
  //      U_LIGHT0_SPOTDIRECTION,
  //      U_LIGHT0_COSCUTOFF,
  //      U_LIGHT0_COSINNER,
  //      U_LIGHT0_EXPONENT,
  //      U_LIGHT1_TYPE,
  //      U_LIGHT1_POSITION,
  //      U_LIGHT1_COLOR,
  //      U_LIGHT1_POWER,
  //      U_LIGHT1_KC,
  //      U_LIGHT1_KL,
  //      U_LIGHT1_KQ,
  //      U_LIGHT1_SPOTDIRECTION,
  //      U_LIGHT1_COSCUTOFF,
  //      U_LIGHT1_COSINNER,
  //      U_LIGHT1_EXPONENT,
  //      U_COLOR_TEXTURE_ENABLED,
  //      U_COLOR_TEXTURE,
  //      U_TEXT_ENABLED,
  //      U_TEXT_COLOR,
  //      U_TOTAL,
  //  };
  //  enum GEOMETRY_TYPE
  //  {
  //      GEO_AXES,
  //      GEO_CROSSHAIR,
  //      GEO_LIGHTBALL,
  //      GEO_SPHERE,

  //      GEO_QUAD,
  //      GEO_CUBE,
  //      GEO_RING,
  //      GEO_CONE,

  //      GEO_BACKGROUND,

  //      GEO_TILEGROUND,
  //      GEO_TILEHERO,
  //      GEO_TILETREE,
  //      GEO_TILESTRUCTURE,
  //      GEO_TILEHERO_FRAME0,
  //      GEO_TILEHERO_FRAME1,
  //      GEO_TILEHERO_FRAME2,
  //      GEO_TILEHERO_FRAME3,
  //      GEO_TILE_KILLZONE,
  //      GEO_TILE_SAFEZONE,
  //      GEO_TILEENEMY_FRAME0,
  //      GEO_TILE_TREASURECHEST,
  //      GEO_OBJECT,
  //      GEO_TEXT,

  //      GEO_STANDING,
  //      GEO_WALK_FRAME1,
  //      GEO_WALK_FRAME2,
  //      GEO_WALK_FRAME3,
  //      GEO_WALK_FRAME4,
  //      GEO_WALK_FRAME5,
  //      GEO_WALK_FRAME6,
  //      GEO_WALK_FRAME7,
  //      GEO_WALK_FRAME8,
  //      GEO_WALK_FRAME9,
  //      GEO_WALK_FRAME10,
  //      GEO_WALK_FRAME11,

  //      //scrolling lvl
  //      GEO_STILE1,
  //      GEO_STILE2,
  //      GEO_MISSILE,

		//// lvl 2 tile 
		//GEO_GRASS,
		//GEO_DIRT,

  //      NUM_GEOMETRY,
  //  };

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