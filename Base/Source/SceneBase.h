#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include <vector>

class SceneBase : public Scene
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
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TEXT,
		GEO_UI,
		GEO_PAUSEUI,
		GEO_HEALTHBAR,
		GEO_PRINCESS,
		GEO_BACKGROUND,
		GEO_EDITBACKGROUND,
		GEO_PLAYERHP,
		GEO_CHARACTER,
		GEO_CHARACTER2,
		GEO_MONSTER,
		GEO_MONSTER2,
		GEO_DEATHSCREEN,
		GEO_VICTORY,
        //panel
        GEO_B_PANEL,
        GEO_M_PANEL,
        GEO_T_PANEL,
        //platform
        GEO_PLAT_NORMAL,
        GEO_PLAT_SELECTOR,
		GEO_CASTLE,
		GEO_CASTLECENTER,
		GEO_CAVE,
		//buttons
		GEO_PLAY,
		GEO_PLAYHOVER,
		GEO_MENU,
		GEO_MENUHOVER,
		GEO_EDIT,
		GEO_EDITHOVER,
		GEO_LOAD,
		GEO_LOADHOVER,
		GEO_HIGHSCORE,
		GEO_HIGHSCOREHOVER,
		GEO_SAVE,
		GEO_SAVEHOVER,
		GEO_EXIT,
		GEO_EXITHOVER,
		GEO_RESTART,
		GEO_RESTARTHOVER,
		GEO_RESUME,
		GEO_RESUMEHOVER,
		//Character
		GEO_PROJECTILE,
        //collision
        GEO_HOTSPOT,
		NUM_GEOMETRY,

	};
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
	void RenderGO(GameObject *go);
	void RenderModelOnScreen(Mesh *mesh, bool enableLight, Vector3 size, float x, float y, float z, Vector3 rotation);

	GameObject* FetchGO();
protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[1];

	bool bLightEnabled;

	float fps;
};

#endif