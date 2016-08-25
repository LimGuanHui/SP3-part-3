#ifndef SP3_H
#define SP3_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Minimap.h"
#include "Map.h"
#include "Vector2.h"
#include "PlayerInfo.h"
#include "Enemy.h"
#include "Missile.h"
#include <irrKlang.h>
using namespace irrklang;

// Goodies and Goodies Factory
#include "GoodiesFactory.h"
#include "Goodies.h"
#include "TreasureChest.h"

class SP3 : public Scene
{
    enum UNIFORM_TYPE
    {
        U_MVP = 0,
        U_MODELVIEW,
        U_MODELVIEW_INVERSE_TRANSPOSE,
        U_MATERIAL_AMBIENT,
        U_MATERIAL_DIFFUSE,
        U_MATERIAL_SPECULAR,
        U_MATERIAL_SHININESS,
        U_LIGHTENABLED,
        U_NUMLIGHTS,
        U_LIGHT0_TYPE,
        U_LIGHT0_POSITION,
        U_LIGHT0_COLOR,
        U_LIGHT0_POWER,
        U_LIGHT0_KC,
        U_LIGHT0_KL,
        U_LIGHT0_KQ,
        U_LIGHT0_SPOTDIRECTION,
        U_LIGHT0_COSCUTOFF,
        U_LIGHT0_COSINNER,
        U_LIGHT0_EXPONENT,
        U_LIGHT1_TYPE,
        U_LIGHT1_POSITION,
        U_LIGHT1_COLOR,
        U_LIGHT1_POWER,
        U_LIGHT1_KC,
        U_LIGHT1_KL,
        U_LIGHT1_KQ,
        U_LIGHT1_SPOTDIRECTION,
        U_LIGHT1_COSCUTOFF,
        U_LIGHT1_COSINNER,
        U_LIGHT1_EXPONENT,
        U_COLOR_TEXTURE_ENABLED,
        U_COLOR_TEXTURE,
        U_TEXT_ENABLED,
        U_TEXT_COLOR,
        U_TOTAL,
    };
    enum GEOMETRY_TYPE
    {
        GEO_AXES,
        GEO_CROSSHAIR,
        GEO_LIGHTBALL,
        GEO_SPHERE,
        GEO_SPHERE2,
        GEO_SPHERE3,
        GEO_SPHERE4,
        GEO_SPHERE5,
        GEO_SPHERE6,
        GEO_SPHERE7,
        GEO_SPHERE8,
        GEO_SPHERE9,
        GEO_QUAD,
        GEO_CUBE,
        GEO_RING,
        GEO_CONE,
        GEO_LEFT,
        GEO_RIGHT,
        GEO_TOP,
        GEO_BOTTOM,
        GEO_FRONT,
        GEO_BACK,
        GEO_GRASS_DARKGREEN,
        GEO_GRASS_LIGHTGREEN,
        GEO_BACKGROUND,
        GEO_TILEGROUND,
        GEO_TILEHERO,
        GEO_TILETREE,
        GEO_TILESTRUCTURE,
        GEO_TILEHERO_FRAME0,
        GEO_TILEHERO_FRAME1,
        GEO_TILEHERO_FRAME2,
        GEO_TILEHERO_FRAME3,
        GEO_TILE_KILLZONE,
        GEO_TILE_SAFEZONE,
        GEO_TILEENEMY_FRAME0,
        GEO_TILE_TREASURECHEST,
        GEO_OBJECT,
        GEO_TEXT,

        GEO_STANDING,
        GEO_WALK_FRAME1,
        GEO_WALK_FRAME2,
        GEO_WALK_FRAME3,
        GEO_WALK_FRAME4,
        GEO_WALK_FRAME5,
        GEO_WALK_FRAME6,
        GEO_WALK_FRAME7,
        GEO_WALK_FRAME8,
        GEO_WALK_FRAME9,
        GEO_WALK_FRAME10,
        GEO_WALK_FRAME11,

        //scrolling lvl
        GEO_STILE1,
        GEO_STILE2,
        GEO_MISSILE,

        NUM_GEOMETRY,
    };

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
    // Update Camera status
    virtual void UpdateCameraStatus(const unsigned char key, const bool status = true);
    // Update Weapon status
    virtual void UpdateWeaponStatus(const unsigned char key);
    virtual void Render();
    virtual void Exit();

    void RenderText(Mesh* mesh, std::string text, Color color);
    void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
    void RenderTextOnScreen2(Mesh* mesh, std::string text, Color color, float size, float x, float y);
    void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size = 1.0f, float x = 0.0f, float y = 0.0f, bool rotate = false);
    void RenderMesh(Mesh *mesh, bool enableLight);
    void RenderGround();
    void RenderSkybox();
    void RenderBackground();
    void Render2DMesh(Mesh *mesh, const bool enableLight, const float size = 1.0f, const float x = 0.0f, const float y = 0.0f, const bool rotate = false, const bool flip = false);
    Missile* FetchMissile();
    void MissileUpdate(float dt);

    void GameStateRenderText();
    void GameStateUpdate();
    void GameStateRender();

    enum WEAPON_ACTION
    {
        WA_NIL = 0,
        WA_FIRE,
        WA_RELOAD,
        WA_CHANGEWEAPON,
        WA_TOTAL,
    };

private:
    unsigned m_vertexArrayID;
    Mesh* meshList[NUM_GEOMETRY];
    unsigned m_programID;
    unsigned m_parameters[U_TOTAL];

    Camera3 camera;

    float rotateAngle;

    MS modelStack;
    MS viewStack;
    MS projectionStack;

    Light lights[2];

    bool bLightEnabled;

    float fps;

    // Handle to the minimap
    CMinimap* m_cMinimap;

    // Handle to the tilemaps
    CMap* m_cMap;
    void RenderTileMap();
    // Hero's information
    CPlayerInfo* theHero;

    // Codes for Scrolling
    int tileOffset_x, tileOffset_y;

    // Codes for Parallax Scrolling
    CMap* m_cRearMap;
    void RenderRearTileMap();
    int rearWallOffset_x, rearWallOffset_y;
    int rearWallTileOffset_x, rearWallTileOffset_y;
    int rearWallFineOffset_x, rearWallFineOffset_y;

    // Enemies
    CEnemy* theEnemy;

    // Goodies and Goodies Factory
    CGoodiesFactory theGoodiesFactory;
    CGoodies** theArrayOfGoodies;
    void RenderGoodies(void);

    std::vector<Missile*> MissileList;
    float missileTriggerTimer;

    int lives;

    GameState State = Menu;

    //sound
    ISoundEngine* sceneSoundEngine;
    /*ISoundSource* shootBulletSound;
    ISoundSource* win;
    ISoundSource* lose;*/
    ISoundSource* jump;
    float jumpsoundtimer;
};

#endif