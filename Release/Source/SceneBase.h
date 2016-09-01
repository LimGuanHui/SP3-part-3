#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include <vector>
#include "MapLoad.h"
#include "Particle.h"
using namespace MAPLOADER;


class SceneBase : public Scene
{
    
public:
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
    void Render2DMesh(Mesh *mesh, const bool enableLight, const float size = 1.0f, const float x = 0.0f, const float y = 0.0f, const bool rotate = false, const bool flip = false);
    void Render2DMesh(Mesh *mesh, const bool enableLight, const float sizex = 1.0f, const float sizey = 1.0f, const float x = 0.0f, const float y = 0.0f, const bool rotate = false, const bool flip = false);
    void RenderGO(GameObject *go);
	void RenderModelOnScreen(Mesh *mesh, bool enableLight, Vector3 size, float x, float y, float z, Vector3 rotation);
    void RenderMeshIn2D(Mesh *mesh, const bool enableLight, const float size, const float x, const float y, const bool rotate);

    
	//GameObject* FetchGO();
protected:
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

        GEO_QUAD,
        //GEO_CUBE,
        GEO_RING,
        GEO_CONE,

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
        //GEO_TILE_TREASURECHEST,
		//GEO_OBJECT,
        GEO_TEXT,

		GEO_IDLE_FRAME1,
		GEO_IDLE_FRAME2,
		GEO_WALKING_FRAME,
		GEO_JUMPING_FRAME,

		GEO_SHOOT_FRAME1,
		GEO_SHOOT_FRAME2,
		GEO_SHOOT_FRAME3,
		GEO_SHOOT_FRAME4,

		GEO_GENGAR_FRAME0,
		GEO_GENGAR_FRAME1,
		GEO_GENGAR_FRAME2,
		GEO_GENGAR_FRAME3,
		GEO_GENGAR_FRAME4,
		GEO_GENGAR_FRAME5,
		GEO_GENGAR_FRAME6,
		GEO_GENGAR_FRAME7,

        //scrolling lvl
        GEO_STILE1,
        GEO_STILE2,
        GEO_MISSILE,

        //lvl 1 tile
        GEO_SNOW_TILE,
        GEO_SNOW_DIRT_TILE,
		GEO_SNOWBACKGROUND,

        // lvl 2 tile 
        GEO_GRASS,
        GEO_DIRT,
		GEO_FORESTBACKGROUND,
		GEO_CAVE,
		GEO_CAVEBACKGROUND,
		GEO_CASTLE,
		GEO_CASTLEGROUND,
		GEO_CASTLEBACKGROUND,
		GEO_MINIBOSS_BG,

        //miniboss stage tile
        GEO_MINIBOSS_TILE1,
        GEO_MINIBOSS_TILE2,

        //monster
        GEO_GASTLY,//11
        GEO_HAUNTER,//12
        GEO_MONSTER3,//13
		GEO_MINIBOSS,

		// UI Stuffs
		GEO_UI,
		GEO_GAMENAME,
		GEO_PAUSEUI,
		GEO_BACKGROUND2,
		GEO_EDITBACKGROUND,
		GEO_PLAYERHP,
		GEO_HPUI,
		GEO_DEATHSCREEN,
		GEO_VICTORY,

		//buttons
		GEO_PLAY,
		GEO_PLAYHOVER,
		GEO_MENU,
		GEO_MENUHOVER,
		GEO_HIGHSCORE,
		GEO_HIGHSCOREHOVER,
		GEO_EXIT,
		GEO_EXITHOVER,
		GEO_RESTART,
		GEO_RESTARTHOVER,
		GEO_RESUME,
		GEO_RESUMEHOVER,
		GEO_OKAYHOVER,
		GEO_HELP,
		GEO_HELPHOVER,
		GEO_INSTRUCTIONS,
		GEO_STARTARROW,

        //player projectiles
        GEO_N_SHOT,
        GEO_C_SHOT,
        GEO_NET,
        GEO_NET_ANIM,
		GEO_BOSS_PROJECTILE,

        //Monster Hp bar
        GEO_MON_HP_BAR,

        //panel
        GEO_B_PANEL,
        GEO_M_PANEL,
        GEO_T_PANEL,

        GEO_E_B_PANEL,
        GEO_E_M_PANEL,
        GEO_E_T_PANEL,

        GEO_PLAYER,
        GEO_PLAYER_CHARGING,
        GEO_PLAYER_MAXCHARGE,

        GEO_BOSS,
        GEO_BOSS_CHARGING,
        GEO_BOSS_MAXCHARGE,

        NUM_GEOMETRY,
    };


	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[1];

	bool bLightEnabled;

	float fps;

    MapLoad* m_cMap;


};

#endif