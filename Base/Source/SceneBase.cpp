#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneBase::SceneBase():
m_cMap(NULL)
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
    // Blue background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Enable depth test
    //	glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    //	glDepthFunc(GL_LESS); 

    glEnable(GL_CULL_FACE);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

    // Get a handle for our uniform
    m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
    //m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
    //m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
    m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
    m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
    m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
    m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
    m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
    m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
    m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
    m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
    m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
    m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
    m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
    m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
    m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
    m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
    m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
    m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
    m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
    m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
    m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
    m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
    m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
    m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
    m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
    m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
    m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
    m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
    m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
    m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
    m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
    m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
    // Get a handle for our "colorTexture" uniform
    m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
    m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
    // Get a handle for our "textColor" uniform
    m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
    m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

    // Use our shader
    glUseProgram(m_programID);

    lights[0].type = Light::LIGHT_DIRECTIONAL;
    lights[0].position.Set(0, 20, 0);
    lights[0].color.Set(1, 1, 1);
    lights[0].power = 1;
    lights[0].kC = 1.f;
    lights[0].kL = 0.01f;
    lights[0].kQ = 0.001f;
    lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
    lights[0].cosInner = cos(Math::DegreeToRadian(30));
    lights[0].exponent = 3.f;
    lights[0].spotDirection.Set(0.f, 1.f, 0.f);

    lights[1].type = Light::LIGHT_DIRECTIONAL;
    lights[1].position.Set(1, 1, 0);
    lights[1].color.Set(1, 1, 0.5f);
    lights[1].power = 0.4f;
    //lights[1].kC = 1.f;
    //lights[1].kL = 0.01f;
    //lights[1].kQ = 0.001f;
    //lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
    //lights[1].cosInner = cos(Math::DegreeToRadian(30));
    //lights[1].exponent = 3.f;
    //lights[1].spotDirection.Set(0.f, 1.f, 0.f);

    glUniform1i(m_parameters[U_NUMLIGHTS], 1);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

    glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
    glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
    glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
    glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
    glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
    glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
    glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
    glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

    glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
    glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
    glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
    glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
    glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
    glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
    glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
    glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

    camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

    for (int i = 0; i < NUM_GEOMETRY; ++i)
    {
        meshList[i] = NULL;
    }
    meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
    meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
    meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 25.f);
    //meshList[GEO_QUAD]->textureID = LoadTGA("Image//calibri.tga");
    meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
    meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
    meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
    meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
    meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 0, 0), 18, 36, 1.f);
    meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 10.f);
    //meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1, 1, 1);
    //meshList[GEO_TORUS] = MeshBuilder::GenerateCylinder("torus", 36, 36, 5, 1);
    meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
    meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
    meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);

    meshList[GEO_TILEGROUND] = MeshBuilder::Generate2DMesh("GEO_TILEGROUND", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_TILEGROUND]->textureID = LoadTGA("Image//tile1_ground.tga");
    meshList[GEO_TILEHERO] = MeshBuilder::Generate2DMesh("GEO_TILEHERO", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_TILEHERO]->textureID = LoadTGA("Image//tile2_hero.tga");
    meshList[GEO_TILETREE] = MeshBuilder::Generate2DMesh("GEO_TILETREE", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_TILETREE]->textureID = LoadTGA("Image//tile3_tree.tga");
    meshList[GEO_TILESTRUCTURE] = MeshBuilder::Generate2DMesh("GEO_TILESTRUCTURE", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_TILESTRUCTURE]->textureID = LoadTGA("Image//tiles//rear_tile1.tga");
    meshList[GEO_TILEHERO_FRAME0] = MeshBuilder::Generate2DMesh("GEO_TILEHERO_FRAME0", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_TILEHERO_FRAME0]->textureID = LoadTGA("Image//tile2_hero_frame_0.tga");
    meshList[GEO_TILEHERO_FRAME1] = MeshBuilder::Generate2DMesh("GEO_TILEHERO_FRAME1", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_TILEHERO_FRAME1]->textureID = LoadTGA("Image//tile2_hero_frame_1.tga");
    meshList[GEO_TILEHERO_FRAME2] = MeshBuilder::Generate2DMesh("GEO_TILEHERO_FRAME2", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_TILEHERO_FRAME2]->textureID = LoadTGA("Image//tile2_hero_frame_2.tga");
    meshList[GEO_TILEHERO_FRAME3] = MeshBuilder::Generate2DMesh("GEO_TILEHERO_FRAME3", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_TILEHERO_FRAME3]->textureID = LoadTGA("Image//tile2_hero_frame_3.tga");

    meshList[GEO_TILE_KILLZONE] = MeshBuilder::Generate2DMesh("GEO_TILE_KILLZONE", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_TILE_KILLZONE]->textureID = LoadTGA("Image//door.tga");
    meshList[GEO_TILE_SAFEZONE] = MeshBuilder::Generate2DMesh("GEO_TILE_SAFEZONE", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_TILE_SAFEZONE]->textureID = LoadTGA("Image//tile11_safezone.tga");
    meshList[GEO_TILEENEMY_FRAME0] = MeshBuilder::Generate2DMesh("GEO_TILEENEMY_FRAME0", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_TILEENEMY_FRAME0]->textureID = LoadTGA("Image//tile20_enemy.tga");

	//Character Frame
	meshList[GEO_IDLE_FRAME1] = MeshBuilder::Generate2DMesh("MEWTWO_IDLE", Color(1, 1, 1), 0.0f, 0.0f, 25.f, 25.f);
	meshList[GEO_IDLE_FRAME1]->textureID = LoadTGA("Image//character//idle1.tga");

	meshList[GEO_IDLE_FRAME2] = MeshBuilder::Generate2DMesh("MEWTWO_IDLE2", Color(1, 1, 1), 0.0f, 0.0f, 25.f, 25.f);
	meshList[GEO_IDLE_FRAME2]->textureID = LoadTGA("Image//character//idle2.tga");

	meshList[GEO_WALKING_FRAME] = MeshBuilder::Generate2DMesh("MEWTWO_WALKING", Color(1, 1, 1), 0.0f, 0.0f, 25.f, 25.f);
	meshList[GEO_WALKING_FRAME]->textureID = LoadTGA("Image//character//Moving.tga");

	meshList[GEO_JUMPING_FRAME] = MeshBuilder::Generate2DMesh("MEWTWO_JUMPING", Color(1, 1, 1), 0.0f, 0.0f, 25.f, 25.f);
	meshList[GEO_JUMPING_FRAME]->textureID = LoadTGA("Image//character//Jumping.tga");

	meshList[GEO_SHOOT_FRAME1] = MeshBuilder::Generate2DMesh("MEWTWO_SHOOTING1", Color(1, 1, 1), 0.0f, 0.0f, 25.f, 25.f);
	meshList[GEO_SHOOT_FRAME1]->textureID = LoadTGA("Image//character//ShootFrame1.tga");

	meshList[GEO_SHOOT_FRAME2] = MeshBuilder::Generate2DMesh("MEWTWO_SHOOTING2", Color(1, 1, 1), 0.0f, 0.0f, 25.f, 25.f);
	meshList[GEO_SHOOT_FRAME2]->textureID = LoadTGA("Image//character//ShootFrame2.tga");

	meshList[GEO_SHOOT_FRAME3] = MeshBuilder::Generate2DMesh("MEWTWO_SHOOTING3", Color(1, 1, 1), 0.0f, 0.0f, 25.f, 25.f);
	meshList[GEO_SHOOT_FRAME3]->textureID = LoadTGA("Image//character//ShootFrame3.tga");

	meshList[GEO_SHOOT_FRAME4] = MeshBuilder::Generate2DMesh("MEWTWO_SHOOTING4", Color(1, 1, 1), 0.0f, 0.0f, 25.f, 25.f);
	meshList[GEO_SHOOT_FRAME4]->textureID = LoadTGA("Image//character//ShootFrame4.tga");


	//Boss Frames
	meshList[GEO_GENGAR_FRAME0] = MeshBuilder::Generate2DMesh("WALK1", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_GENGAR_FRAME0]->textureID = LoadTGA("Image//enemy//gengar1.tga");

	meshList[GEO_GENGAR_FRAME1] = MeshBuilder::Generate2DMesh("WALK1", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_GENGAR_FRAME1]->textureID = LoadTGA("Image//enemy//gengar2.tga");

	meshList[GEO_GENGAR_FRAME2] = MeshBuilder::Generate2DMesh("WALK2", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_GENGAR_FRAME2]->textureID = LoadTGA("Image//enemy//gengar3.tga");

	meshList[GEO_GENGAR_FRAME3] = MeshBuilder::Generate2DMesh("WALK3", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_GENGAR_FRAME3]->textureID = LoadTGA("Image//enemy//gengar4.tga");

	meshList[GEO_GENGAR_FRAME4] = MeshBuilder::Generate2DMesh("WALK4", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_GENGAR_FRAME4]->textureID = LoadTGA("Image//enemy//gengar5.tga");

	meshList[GEO_GENGAR_FRAME5] = MeshBuilder::Generate2DMesh("WALK5", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_GENGAR_FRAME5]->textureID = LoadTGA("Image//enemy//gengar6.tga");

	meshList[GEO_GENGAR_FRAME6] = MeshBuilder::Generate2DMesh("WALK6", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_GENGAR_FRAME6]->textureID = LoadTGA("Image//enemy//gengar7.tga");

	meshList[GEO_GENGAR_FRAME7] = MeshBuilder::Generate2DMesh("WALK7", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_GENGAR_FRAME7]->textureID = LoadTGA("Image//enemy//gengar8.tga");

    meshList[GEO_STILE1] = MeshBuilder::Generate2DMesh("GEO_STILE1", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_STILE1]->textureID = LoadTGA("Image//tiles//tile1.tga");

    meshList[GEO_STILE2] = MeshBuilder::Generate2DMesh("GEO_STILE1", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_STILE2]->textureID = LoadTGA("Image//tiles//tile2.tga");

	// Tiles
    meshList[GEO_GRASS] = MeshBuilder::Generate2DMesh("GEO_GRASS", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_GRASS]->textureID = LoadTGA("Image//tiles//grass.tga");

    meshList[GEO_DIRT] = MeshBuilder::Generate2DMesh("GEO_DIRT", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_DIRT]->textureID = LoadTGA("Image//tiles//dirt.tga");

    meshList[GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
    meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//background//snowBG.tga");

	meshList[GEO_FORESTBACKGROUND] = MeshBuilder::Generate2DMesh("GEO_CASTLEBG", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_FORESTBACKGROUND]->textureID = LoadTGA("Image//background//forest.tga");

	meshList[GEO_CAVE] = MeshBuilder::Generate2DMesh("GEO_CAVE", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_CAVE]->textureID = LoadTGA("Image//tiles//Cave3.tga");

	meshList[GEO_CAVEBACKGROUND] = MeshBuilder::Generate2DMesh("GEO_CAVEBG", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_CAVEBACKGROUND]->textureID = LoadTGA("Image//background//CaveBG2.tga");

	meshList[GEO_CASTLE] = MeshBuilder::Generate2DMesh("GEO_CASTLE", Color(1, 1, 1), 0.0f, 0.0f, 25.f, 25.f);
	meshList[GEO_CASTLE]->textureID = LoadTGA("Image//tiles//castle.tga");

	meshList[GEO_CASTLEGROUND] = MeshBuilder::Generate2DMesh("GEO_CASTLEGROUND", Color(1, 1, 1), 0.0f, 0.0f, 25.f, 25.f);
	meshList[GEO_CASTLEGROUND]->textureID = LoadTGA("Image//tiles//castleground.tga");
	
	meshList[GEO_CASTLEBACKGROUND] = MeshBuilder::Generate2DMesh("GEO_CASTLEBG", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_CASTLEBACKGROUND]->textureID = LoadTGA("Image//background//bg_castle.tga");

	meshList[GEO_SNOWBACKGROUND] = MeshBuilder::Generate2DMesh("GEO_SNOWBG", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_SNOWBACKGROUND]->textureID = LoadTGA("Image//background//SnowBG.tga");

	meshList[GEO_MINIBOSS_BG] = MeshBuilder::Generate2DMesh("GEO_MINIBOSS_BG", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_MINIBOSS_BG]->textureID = LoadTGA("Image//background//MiniBoss_BG.tga");


	//Projectile
    meshList[GEO_MISSILE] = MeshBuilder::Generate2DMesh("Missile", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_MISSILE]->textureID = LoadTGA("Image//BossProjectile.tga");

	//Monster
	meshList[GEO_GASTLY] = MeshBuilder::Generate2DMesh("gastly", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_GASTLY]->textureID = LoadTGA("Image//enemy//gastly.tga");
    meshList[GEO_HAUNTER] = MeshBuilder::Generate2DMesh("monster2", Color(1.000f, 0.843f, 0.000f), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_HAUNTER]->textureID = LoadTGA("Image//enemy//haunter.tga");
    meshList[GEO_MONSTER3] = MeshBuilder::Generate2DMesh("monster3", Color(0.580f, 0.000f, 0.827f), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_MINIBOSS] = MeshBuilder::Generate2DMesh("MiniBoss", Color(1.f, 0.000f, 0.f), 0.0f, 0.0f, 25.0f, 25.0f);

	// UI STUFFS
	meshList[GEO_UI] = MeshBuilder::GenerateQuad("UI", Color(0, 1, 0), 1.f);
	meshList[GEO_UI]->textureID = LoadTGA("Image//bg.tga");

	meshList[GEO_GAMENAME] = MeshBuilder::GenerateQuad("Game Name", Color(1, 1, 1), 1.f);
	meshList[GEO_GAMENAME]->textureID = LoadTGA("Image//gamename.tga");

	meshList[GEO_PAUSEUI] = MeshBuilder::GenerateQuad("PauseUI", Color(0, 1, 0), 1.f);
	meshList[GEO_PAUSEUI]->textureID = LoadTGA("Image//pauseui.tga");

	meshList[GEO_PLAYERHP] = MeshBuilder::GenerateQuad("Player HP", Color(0, 1, 0), 1.f);

	meshList[GEO_HPUI] = MeshBuilder::GenerateQuad("HP UI", Color(1, 1, 1), 1.f);
	meshList[GEO_HPUI]->textureID = LoadTGA("Image//hpui.tga");

	meshList[GEO_BACKGROUND2] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND2", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_BACKGROUND2]->textureID = LoadTGA("Image//heaven.tga");

	meshList[GEO_EDITBACKGROUND] = MeshBuilder::GenerateQuad("GEO_EDITBACKGROUND", Color(1, 1, 1), 1.f);
	meshList[GEO_EDITBACKGROUND]->textureID = LoadTGA("Image//editbg.tga");

	//Victory & death screen
	meshList[GEO_DEATHSCREEN] = MeshBuilder::GenerateQuad("DeathSCreen", Color(1, 1, 1), 1.f);
	meshList[GEO_DEATHSCREEN]->textureID = LoadTGA("Image//gameover.tga");

	meshList[GEO_VICTORY] = MeshBuilder::GenerateQuad("VictoryScreen", Color(1, 1, 1), 1.f);
	meshList[GEO_VICTORY]->textureID = LoadTGA("Image//victory.tga");

	// Buttons thing
	meshList[GEO_PLAY] = MeshBuilder::GenerateQuad("Play", Color(1, 1, 1), 1.f);
	meshList[GEO_PLAY]->textureID = LoadTGA("Image//buttons//play.tga");
	meshList[GEO_PLAYHOVER] = MeshBuilder::GenerateQuad("Play Hover ", Color(1, 1, 1), 1.f);
	meshList[GEO_PLAYHOVER]->textureID = LoadTGA("Image//buttons//playhover.tga");

	meshList[GEO_MENU] = MeshBuilder::GenerateQuad("Menu", Color(1, 1, 1), 1.f);
	meshList[GEO_MENU]->textureID = LoadTGA("Image//buttons//menu.tga");
	meshList[GEO_MENUHOVER] = MeshBuilder::GenerateQuad("Menu Hover ", Color(1, 1, 1), 1.f);
	meshList[GEO_MENUHOVER]->textureID = LoadTGA("Image//buttons//menuhover.tga");

	meshList[GEO_HIGHSCORE] = MeshBuilder::GenerateQuad("Highscore", Color(1, 1, 1), 1.f);
	meshList[GEO_HIGHSCORE]->textureID = LoadTGA("Image//buttons//highscore.tga");
	meshList[GEO_HIGHSCOREHOVER] = MeshBuilder::GenerateQuad("Highscore hover but", Color(1, 1, 1), 1.f);
	meshList[GEO_HIGHSCOREHOVER]->textureID = LoadTGA("Image//buttons//highscorehover.tga");

	meshList[GEO_EXIT] = MeshBuilder::GenerateQuad("Exit", Color(1, 1, 1), 1.f);
	meshList[GEO_EXIT]->textureID = LoadTGA("Image//buttons//exit.tga");
	meshList[GEO_EXITHOVER] = MeshBuilder::GenerateQuad("Exit Hover ", Color(1, 1, 1), 1.f);
	meshList[GEO_EXITHOVER]->textureID = LoadTGA("Image//buttons//exithover.tga");

	meshList[GEO_RESTART] = MeshBuilder::GenerateQuad("restart", Color(1, 1, 1), 1.f);
	meshList[GEO_RESTART]->textureID = LoadTGA("Image//buttons//restart.tga");
	meshList[GEO_RESTARTHOVER] = MeshBuilder::GenerateQuad("restart hover", Color(1, 1, 1), 1.f);
	meshList[GEO_RESTARTHOVER]->textureID = LoadTGA("Image//buttons//restarthover.tga");

	meshList[GEO_RESUME] = MeshBuilder::GenerateQuad("resume", Color(1, 1, 1), 1.f);
	meshList[GEO_RESUME]->textureID = LoadTGA("Image//buttons//resume.tga");
	meshList[GEO_RESUMEHOVER] = MeshBuilder::GenerateQuad("resume hover ", Color(1, 1, 1), 1.f);
	meshList[GEO_RESUMEHOVER]->textureID = LoadTGA("Image//buttons//resumehover.tga");

	meshList[GEO_OKAYHOVER] = MeshBuilder::GenerateQuad("Okay hover", Color(1, 1, 1), 1.f);
	meshList[GEO_OKAYHOVER]->textureID = LoadTGA("Image//buttons//okayhover.tga");

	meshList[GEO_HELP] = MeshBuilder::GenerateQuad("help", Color(1, 1, 1), 1.f);
	meshList[GEO_HELP]->textureID = LoadTGA("Image//buttons//help.tga");
	meshList[GEO_HELPHOVER] = MeshBuilder::GenerateQuad("help hover", Color(1, 1, 1), 1.f);
	meshList[GEO_HELPHOVER]->textureID = LoadTGA("Image//buttons//helphover.tga");

	meshList[GEO_INSTRUCTIONS] = MeshBuilder::GenerateQuad("instructions", Color(1, 1, 1), 1.f);
	meshList[GEO_INSTRUCTIONS]->textureID = LoadTGA("Image//instructions.tga");

	meshList[GEO_STARTARROW] = MeshBuilder::GenerateQuad("Start Arrow", Color(1, 1, 1), 1.f);
	meshList[GEO_STARTARROW]->textureID = LoadTGA("Image//selectarrow.tga");

    meshList[GEO_MON_HP_BAR] = MeshBuilder::Generate2DMesh("GEO_MON_HP_BAR", Color(0, 1, 0), 0.0f, 0.0f, 25.0f, 10.0f);
    //meshList[GEO_MON_HP_BAR]->textureID = LoadTGA("Image//tiles//grass.tga");

    meshList[GEO_NET] = MeshBuilder::Generate2DMesh("net", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_NET]->textureID = LoadTGA("Image//Projectiles//net.tga");
    meshList[GEO_N_SHOT] = MeshBuilder::Generate2DMesh("N_SHOT", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_N_SHOT]->textureID = LoadTGA("Image//Projectiles//NormalShot.tga");
    meshList[GEO_C_SHOT] = MeshBuilder::Generate2DMesh("C_SHOT", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_C_SHOT]->textureID = LoadTGA("Image//Projectiles//ChargeShot.tga");

	meshList[GEO_BOSS_PROJECTILE] = MeshBuilder::Generate2DMesh("Boss", Color(1, 1, 1), 0.0f, 0.0f, 25.f, 25.f);
	meshList[GEO_BOSS_PROJECTILE]->textureID = LoadTGA("Image//Projectiles//BossProjectile.tga");

    meshList[GEO_NET_ANIM] = MeshBuilder::GenerateSpriteAnimation("Net animation", 7, 7);
    meshList[GEO_NET_ANIM]->textureID = LoadTGA("Image//particle effects//capture_effect.tga");

    //SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_NET_ANIM]);
    //if (sa)
    //{
    //    sa->m_anim = new Animation();
    //    sa->m_anim->Set(0, 63, 0, 3.f, false); // startFrame, endFrame, repeat, Time, Enable
    //}

    //panels
    //player panels
    meshList[GEO_B_PANEL] = MeshBuilder::GenerateQuad("bPanel", Color(0, 1, 0), 1.f);
    meshList[GEO_B_PANEL]->textureID = LoadTGA("Image//panels//bPanel.tga");

    meshList[GEO_M_PANEL] = MeshBuilder::GenerateQuad("mPanel", Color(0, 1, 0), 1.f);
    meshList[GEO_M_PANEL]->textureID = LoadTGA("Image//panels//mPanel.tga");

    meshList[GEO_T_PANEL] = MeshBuilder::GenerateQuad("tPanel", Color(0, 1, 0), 1.f);
    meshList[GEO_T_PANEL]->textureID = LoadTGA("Image//panels//tPanel.tga");

    //enemy panels
    meshList[GEO_E_B_PANEL] = MeshBuilder::GenerateQuad("bPanel", Color(0, 1, 0), 1.f);
    meshList[GEO_E_B_PANEL]->textureID = LoadTGA("Image//panels//b_E_Panel.tga");

    meshList[GEO_E_M_PANEL] = MeshBuilder::GenerateQuad("mPanel", Color(0, 1, 0), 1.f);
    meshList[GEO_E_M_PANEL]->textureID = LoadTGA("Image//panels//m_E_Panel.tga");

    meshList[GEO_E_T_PANEL] = MeshBuilder::GenerateQuad("tPanel", Color(0, 1, 0), 1.f);
    meshList[GEO_E_T_PANEL]->textureID = LoadTGA("Image//panels//t_E_Panel.tga");

    meshList[GEO_PLAYER] = MeshBuilder::GenerateQuad("GEO_PLAYER", Color(0, 1, 0), 70.f);
    meshList[GEO_PLAYER]->textureID = LoadTGA("Image//character//Moving.tga");
    meshList[GEO_PLAYER_CHARGING] = MeshBuilder::GenerateQuad("GEO_PLAYER_CHARGING", Color(1, 0, 0), 70.f);
    meshList[GEO_PLAYER_CHARGING]->textureID = LoadTGA("Image//character//ShootFrame1.tga");
    meshList[GEO_PLAYER_MAXCHARGE] = MeshBuilder::GenerateQuad("GEO_PLAYER_MAXCHARGE", Color(0, 0, 1), 70.f);
    meshList[GEO_PLAYER_MAXCHARGE]->textureID = LoadTGA("Image//character//ShootFrame3.tga");

    meshList[GEO_BOSS] = MeshBuilder::GenerateQuad("GEO_BOSS", Color(0, 1, 0), 60.f);
    meshList[GEO_BOSS]->textureID = LoadTGA("Image//enemy//BOSS1.tga");

    meshList[GEO_BOSS_CHARGING] = MeshBuilder::GenerateQuad("GEO_BOSS_CHARGING", Color(1, 0, 0), 60.f);
    meshList[GEO_BOSS_CHARGING]->textureID = LoadTGA("Image//enemy//BOSS2.tga");

    meshList[GEO_BOSS_MAXCHARGE] = MeshBuilder::GenerateQuad("GEO_BOSS_MAXCHARGE", Color(0, 0, 1), 60.f);
    meshList[GEO_BOSS_MAXCHARGE]->textureID = LoadTGA("Image//enemy//BOSS3.tga");

    meshList[GEO_SNOW_TILE] = MeshBuilder::Generate2DMesh("GEO_SNOW_TILE", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_SNOW_TILE]->textureID = LoadTGA("Image//tiles//snow_tile.tga");

    meshList[GEO_SNOW_DIRT_TILE] = MeshBuilder::Generate2DMesh("GEO_SNOW_DIRT_TILE", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_SNOW_DIRT_TILE]->textureID = LoadTGA("Image//tiles//snowdirt_tile.tga");

    meshList[GEO_MINIBOSS_TILE1] = MeshBuilder::Generate2DMesh("GEO_MINIBOSS_TILE1", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_MINIBOSS_TILE1]->textureID = LoadTGA("Image//tiles//miniboss_stage_tile1.tga");

    meshList[GEO_MINIBOSS_TILE2] = MeshBuilder::Generate2DMesh("GEO_MINIBOSS_TILE2", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_MINIBOSS_TILE2]->textureID = LoadTGA("Image//tiles//miniboss_stage_tile2.tga");



    /************************************************************************************************************************
    //Sprite Animation
    *************************************************************************************************************************/
    //meshList[GEO_SPRITE_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("sprite flame", 1, 6);
    //meshList[GEO_SPRITE_ANIMATION]->textureArray[0] = LoadTGA("Image//flame_sprite.tga");
    //SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
    //if (sa)
    //{
    //    sa->m_anim = new Animation();
    //    sa->m_anim->Set(0, 5, 0, 1.f, true); // startFrame, endFrame, repeat, Time, Enable
    //}

    //Define for particle use................>_<



	m_cMap = LoadMap();

}

void SceneBase::Update(double dt)
{
   // camera.Update(dt);
	//Keyboard Section
	/*if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);

	if(Application::IsKeyPressed('7'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('8'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/


	
	fps = (float)(1.f / dt);
}

void SceneBase::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
    if (!mesh || mesh->textureID <= 0)
        return;

    //	glDisable(GL_DEPTH_TEST);
    Mtx44 ortho;
    ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity();
    modelStack.PushMatrix();
    modelStack.LoadIdentity();
    modelStack.Translate(x, y, 0);
    modelStack.Scale(size, size, size);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        Mtx44 characterSpacing;
        characterSpacing.SetToTranslation(i + 0.5f, 0.3f, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        mesh->Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    modelStack.PopMatrix();
    viewStack.PopMatrix();
    projectionStack.PopMatrix();
}

void SceneBase::RenderModelOnScreen(Mesh *mesh, bool enableLight, Vector3 size, float x, float y, float z, Vector3 rotation)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -100, 70); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);

	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, z);
	modelStack.Scale(size.x, size.y, size.z);
	modelStack.Rotate(rotation.x, 1, 0, 0);
	modelStack.Rotate(rotation.y, 0, 1, 0);
	modelStack.Rotate(rotation.z, 0, 0, 1);

	RenderMesh(mesh, enableLight);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneBase::RenderMesh(Mesh *mesh, bool enableLight)
{
    Mtx44 MVP, modelView, modelView_inverse_transpose;

    MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    if (enableLight && bLightEnabled)
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 1);
        modelView = viewStack.Top() * modelStack.Top();
        glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
        modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
        glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

        //load material
        glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
        glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
        glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
        glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
    }
    else
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    }
    if (mesh->textureID > 0)
    {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->textureID);
        glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    }
    else
    {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
    }
    mesh->Render();
    if (mesh->textureID > 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void SceneBase::Render2DMesh(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate, bool flip)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, 1);

	if (rotate)
	{
		glFrontFace(GL_CW);
		modelStack.Translate(m_cMap->GetTileSize(), 0, 0);
		modelStack.Rotate(180, 0, 1, 0);
	}


	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	if (rotate)
	{
		glFrontFace(GL_CCW);
	}
	/* if (flip)
	glEnable(GL_CULL_FACE);*/
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneBase::Render2DMesh(Mesh *mesh, const bool enableLight, const float sizex , const float sizey , const float x , const float y , const bool rotate, const bool flip)
{
    Mtx44 ortho;
    ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity();
    modelStack.PushMatrix();
    modelStack.LoadIdentity();
    modelStack.Translate(x, y, 0);
    modelStack.Scale(sizex, sizey, 1);

    if (rotate)
    {
        glFrontFace(GL_CW);
        modelStack.Translate(m_cMap->GetTileSize(), 0, 0);
        modelStack.Rotate(180, 0, 1, 0);
    }


    Mtx44 MVP, modelView, modelView_inverse_transpose;

    MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    if (mesh->textureID > 0)
    {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->textureID);
        glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    }
    else
    {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
    }
    mesh->Render();
    if (mesh->textureID > 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    if (rotate)
    {
        glFrontFace(GL_CCW);
    }
    /* if (flip)
    glEnable(GL_CULL_FACE);*/
    modelStack.PopMatrix();
    viewStack.PopMatrix();
    projectionStack.PopMatrix();
}


void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
}

void SceneBase::Exit()
{
	// Cleanup VBO
    for (int i = 0; i < NUM_GEOMETRY; ++i)
    {
        if (meshList[i])
            delete meshList[i];
    }
    
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}	