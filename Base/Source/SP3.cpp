#include "SP3.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "Strategy_Kill.h"

SP3::SP3()
: m_cMinimap(NULL)
, m_cMap(NULL)
, tileOffset_x(0)
, tileOffset_y(0)
, m_cRearMap(NULL)
, rearWallOffset_x(0)
, rearWallOffset_y(0)
, rearWallTileOffset_x(0)
, rearWallTileOffset_y(0)
, rearWallFineOffset_x(0)
, rearWallFineOffset_y(0)
//, theEnemy(NULL)
, sceneSoundEngine(NULL)
{
}

SP3::~SP3()
{
    /*if (theEnemy)
    {
        delete theEnemy;
        theEnemy = NULL;
    }*/

    if (m_cMap)
    {
        delete m_cMap;
        m_cMap = NULL;
    }

    if (m_cMinimap)
    {
        delete m_cMinimap;
        m_cMinimap = NULL;
    }
    if (sceneSoundEngine != NULL)
    {
        sceneSoundEngine->drop();
    }
}

void SP3::Init()
{
    // Blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
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
    meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
    meshList[GEO_QUAD]->textureID = LoadTGA("Image//calibri.tga");
    meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
    meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
    meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
    meshList[GEO_OBJECT] = MeshBuilder::GenerateOBJ("OBJ1", "OBJ//chair.obj");//MeshBuilder::GenerateCube("cube", 1);
   // meshList[GEO_OBJECT]->textureID = LoadTGA("Image//chair.tga");
    meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
    meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 0, 0), 18, 36, 1.f);
    meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 10.f);
    //meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1, 1, 1);
    //meshList[GEO_TORUS] = MeshBuilder::GenerateCylinder("torus", 36, 36, 5, 1);
    meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
    meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
    meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);

    meshList[GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
    meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//background.tga");

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
    meshList[GEO_TILE_KILLZONE]->textureID = LoadTGA("Image//tile10_killzone.tga");
    meshList[GEO_TILE_SAFEZONE] = MeshBuilder::Generate2DMesh("GEO_TILE_SAFEZONE", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_TILE_SAFEZONE]->textureID = LoadTGA("Image//tile11_safezone.tga");
    meshList[GEO_TILEENEMY_FRAME0] = MeshBuilder::Generate2DMesh("GEO_TILEENEMY_FRAME0", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_TILEENEMY_FRAME0]->textureID = LoadTGA("Image//tile20_enemy.tga");


    meshList[GEO_WALK_FRAME1] = MeshBuilder::Generate2DMesh("WALK1", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_WALK_FRAME1]->textureID = LoadTGA("Image//tiles//walk1.tga");

    meshList[GEO_WALK_FRAME2] = MeshBuilder::Generate2DMesh("WALK2", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_WALK_FRAME2]->textureID = LoadTGA("Image//tiles//walk2.tga");

    meshList[GEO_WALK_FRAME3] = MeshBuilder::Generate2DMesh("WALK3", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_WALK_FRAME3]->textureID = LoadTGA("Image//tiles//walk3.tga");

    meshList[GEO_WALK_FRAME4] = MeshBuilder::Generate2DMesh("WALK4", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_WALK_FRAME4]->textureID = LoadTGA("Image//tiles//walk4.tga");

    meshList[GEO_WALK_FRAME5] = MeshBuilder::Generate2DMesh("WALK5", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_WALK_FRAME5]->textureID = LoadTGA("Image//tiles//walk5.tga");

    meshList[GEO_WALK_FRAME6] = MeshBuilder::Generate2DMesh("WALK6", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_WALK_FRAME6]->textureID = LoadTGA("Image//tiles//walk6.tga");

    meshList[GEO_WALK_FRAME7] = MeshBuilder::Generate2DMesh("WALK7", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_WALK_FRAME7]->textureID = LoadTGA("Image//tiles//walk7.tga");

    meshList[GEO_WALK_FRAME8] = MeshBuilder::Generate2DMesh("WALK8", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_WALK_FRAME8]->textureID = LoadTGA("Image//tiles//walk8.tga");

    meshList[GEO_WALK_FRAME9] = MeshBuilder::Generate2DMesh("WALK9", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_WALK_FRAME9]->textureID = LoadTGA("Image//tiles//walk9.tga");

    meshList[GEO_WALK_FRAME10] = MeshBuilder::Generate2DMesh("WALK10", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_WALK_FRAME10]->textureID = LoadTGA("Image//tiles//walk10.tga");

    meshList[GEO_WALK_FRAME11] = MeshBuilder::Generate2DMesh("WALK11", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_WALK_FRAME11]->textureID = LoadTGA("Image//tiles//walk11.tga");

    meshList[GEO_STANDING] = MeshBuilder::Generate2DMesh("standing", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_STANDING]->textureID = LoadTGA("Image//tiles//stand.tga");


    meshList[GEO_STILE1] = MeshBuilder::Generate2DMesh("GEO_STILE1", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_STILE1]->textureID = LoadTGA("Image//tiles//tile1.tga");

    meshList[GEO_STILE2] = MeshBuilder::Generate2DMesh("GEO_STILE1", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_STILE2]->textureID = LoadTGA("Image//tiles//tile2.tga");

	meshList[GEO_GRASS] = MeshBuilder::Generate2DMesh("GEO_GRASS", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_GRASS]->textureID = LoadTGA("Image//tiles//grass.tga");

	meshList[GEO_DIRT] = MeshBuilder::Generate2DMesh("GEO_DIRT", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
	meshList[GEO_DIRT]->textureID = LoadTGA("Image//tiles//dirt.tga");

    meshList[GEO_MISSILE] = MeshBuilder::Generate2DMesh("Missile", Color(1, 1, 1), 0.0f, 0.0f, 25.0f, 25.0f);
    meshList[GEO_MISSILE]->textureID = LoadTGA("Image//enemy//missile.tga");



	lives = 3;
    // Initialise and load the tile map
    m_cMap = new CMap();
    m_cMap->Init(600, 800, 24, 32, 600, 800);
    m_cMap->LoadMap("Map\\Map1.csv");

    // Initialise and load the REAR tile map
    m_cRearMap = new CMap();
    m_cRearMap->Init(600, 800, 24, 32, 600, 1600);
    m_cRearMap->LoadMap("Image//MapDesign_Rear.csv");

	theHero = new CPlayerInfo();

    // Initialise the hero's position
    SpawnCharacter();
    

    // Load the texture for minimap
    m_cMinimap = new CMinimap();
    m_cMinimap->SetBackground(MeshBuilder::GenerateMinimap("MINIMAP", Color(1, 1, 1), 1.f));
    m_cMinimap->GetBackground()->textureID = LoadTGA("Image//grass_darkgreen.tga");
    m_cMinimap->SetBorder(MeshBuilder::GenerateMinimapBorder("MINIMAP BORDER", Color(1, 1, 0), 1.f));
    m_cMinimap->SetAvatar(MeshBuilder::GenerateMinimapAvatar("MINIMAP AVATAR", Color(1, 1, 0), 1.f));

    // Set the strategy for the enemy
    /*theEnemy = new CEnemy();
    theEnemy->ChangeStrategy(NULL, false);
    theEnemy->SetPos_x(575);
    theEnemy->SetPos_y(100);*/

    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
    Mtx44 perspective;
    perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    //perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
    projectionStack.LoadMatrix(perspective);

    rotateAngle = 0;

    bLightEnabled = true;

	for (int i = 0; i < 10; i++)
	{
		Missile *missile = new Missile();
		MissileList.push_back(missile);
	}

    sceneSoundEngine = createIrrKlangDevice();
    //source = sceneSoundEngine->addSoundSourceFromFile("music//etc.ogg")
    jump = sceneSoundEngine->addSoundSourceFromFile("music//jump.ogg");

    jumpsoundtimer = 0;

    CurrLevel = LEVEL1;
}

void SP3::Update(double dt)
{
    if (Application::IsKeyPressed('1'))
        glEnable(GL_CULL_FACE);
    if (Application::IsKeyPressed('2'))
        glDisable(GL_CULL_FACE);
    if (Application::IsKeyPressed('3'))
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (Application::IsKeyPressed('4'))
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    rotateAngle -= Application::camera_yaw;// += (float)(10 * dt);

    camera.Update(dt);

    GameStateUpdate();

    if (jumpsoundtimer > 0)
        jumpsoundtimer -= dt;

    if (State == SP3::Game)
    {
        // Update the hero
        if (Application::IsKeyPressed('A'))
            this->theHero->MoveLeftRight(true, 1.0f);
        if (Application::IsKeyPressed('D'))
            this->theHero->MoveLeftRight(false, 1.0f);
        if (Application::IsKeyPressed(' '))
        {
            this->theHero->SetToJumpUpwards(true);
            if (jumpsoundtimer <= 0)
            {
                jumpsoundtimer = 0.4f;
                sceneSoundEngine->play2D(jump);
            }

        }

        theHero->HeroUpdate(m_cMap);
        Scenetransition();


        // ReCalculate the tile offsets
        tileOffset_x = (int)(theHero->GetMapOffset_x() / m_cMap->GetTileSize());
        if (tileOffset_x + m_cMap->GetNumOfTiles_Width() > m_cMap->getNumOfTiles_MapWidth())
            tileOffset_x = m_cMap->getNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width();

        // if the hero enters the kill zone, then enemy goes into kill strategy mode
        int checkPosition_X = (int)((theHero->GetMapOffset_x() + theHero->GetPos_x()) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int)((theHero->GetPos_y() + m_cMap->GetTileSize()) / m_cMap->GetTileSize());
        missileTriggerTimer += dt;

        if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 3 && missileTriggerTimer > 1.f)
        {
            missileTriggerTimer = 0;
            Missile* go = FetchMissile();
            go->Init(theHero->GetPos_x() - (32 * 11), theHero->GetPos_y());
        }
        MissileUpdate(dt);

        fps = (float)(1.f / dt);
    }
}

void SP3::RenderText(Mesh* mesh, std::string text, Color color)
{
    if (!mesh || mesh->textureID <= 0)
        return;

    //	glDisable(GL_DEPTH_TEST);
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
        characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        mesh->Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    //	glEnable(GL_DEPTH_TEST);
}

void SP3::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
    //	glEnable(GL_DEPTH_TEST);
}

void SP3::RenderMeshIn2D(Mesh *mesh, const bool enableLight, const float size, const float x, const float y, const bool rotate)
{
    Mtx44 ortho;
    ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity();
    modelStack.PushMatrix();
    modelStack.LoadIdentity();
    modelStack.Translate(x, y, 0);
    modelStack.Scale(size, size, size);
    if (rotate)
        modelStack.Rotate(rotateAngle, 0, 0, 1);

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

    modelStack.PopMatrix();
    viewStack.PopMatrix();
    projectionStack.PopMatrix();

}

void SP3::Render2DMesh(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate, bool flip)
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
    modelStack.Scale(size, size, size);


    if (flip)
    {
        glDisable(GL_CULL_FACE);
        modelStack.Translate(32, 0, 0);
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
    if (flip)
        glEnable(GL_CULL_FACE);
    modelStack.PopMatrix();
    viewStack.PopMatrix();
    projectionStack.PopMatrix();
}

void SP3::RenderMesh(Mesh *mesh, bool enableLight)
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

void SP3::RenderBackground()
{
    // Render the crosshair
    Render2DMesh(meshList[GEO_BACKGROUND], false, 1.0f);
}

void SP3::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    Mtx44 perspective;
    perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    //perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
    projectionStack.LoadMatrix(perspective);

    // Camera matrix
    viewStack.LoadIdentity();
    viewStack.LookAt(
        camera.position.x, camera.position.y, camera.position.z,
        camera.target.x, camera.target.y, camera.target.z,
        camera.up.x, camera.up.y, camera.up.z
        );
    // Model matrix : an identity matrix (model will be at the origin)
    modelStack.LoadIdentity();

    // Render the background image
    RenderBackground();
    // Render the rear tile map
    RenderRearTileMap();
    // Render the tile map
    RenderTileMap();
    //Render missiles
    for (std::vector<Missile *>::iterator it = MissileList.begin(); it != MissileList.end(); ++it)
    {
        Missile *go = (Missile *)*it;
        if (go->active)
        {
            Render2DMesh(meshList[GEO_MISSILE], false, 1.0f, go->x, go->y, false, true);
        }
    }

    GameStateRenderText();
}

void SP3::Exit()
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

/********************************************************************************
Render the tile map. This is a private function for use in this class only
********************************************************************************/
void SP3::RenderTileMap()
{
    int m = 0;
    for (int i = 0; i < m_cMap->GetNumOfTiles_Height(); i++)
    {
        for (int k = 0; k < m_cMap->GetNumOfTiles_Width() + 1; k++)
        {
            m = tileOffset_x + k;
            // If we have reached the right side of the Map, then do not display the extra column of tiles.
            if ((tileOffset_x + k) >= m_cMap->getNumOfTiles_MapWidth())
                break;
            if (m_cMap->theScreenMap[i][m] == 1)
            {
                Render2DMesh(meshList[GEO_STILE1], false, 1.0f, k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == 2)
            {
                Render2DMesh(meshList[GEO_STILE2], false, 1.0f, k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize());
			} 
			else if (m_cMap->theScreenMap[i][m] == 3)
			{
				Render2DMesh(meshList[GEO_GRASS], false, 1.0f, k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == 4)
			{
				Render2DMesh(meshList[GEO_DIRT], false, 1.0f, k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == 5)
			{
				Render2DMesh(meshList[GEO_STILE1], false, 1.0f, k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == 6)
			{
				Render2DMesh(meshList[GEO_STILE1], false, 1.0f, k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == 7)
			{
				Render2DMesh(meshList[GEO_STILE2], false, 1.0f, k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize());
			}
            else if (m_cMap->theScreenMap[i][m] == 10)
            {
                Render2DMesh(meshList[GEO_TILE_KILLZONE], false, 1.0f, k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize());
            }
            else if (m_cMap->theScreenMap[i][m] == 11)
            {
                Render2DMesh(meshList[GEO_TILE_SAFEZONE], false, 1.0f, k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize());
            }
        }
    }


    //Render2DMesh(meshList[GEO_AXES], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y());


    if (theHero->GetAnimationInvert() == false)
    {
        if (theHero->GetAnimationCounter() == 1)
            Render2DMesh(meshList[GEO_WALK_FRAME1], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y());
        else if (theHero->GetAnimationCounter() == 2)
            Render2DMesh(meshList[GEO_WALK_FRAME2], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y());
        else if (theHero->GetAnimationCounter() == 3)
            Render2DMesh(meshList[GEO_WALK_FRAME3], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y());
        else if (theHero->GetAnimationCounter() == 4)
            Render2DMesh(meshList[GEO_WALK_FRAME4], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y());
        else if (theHero->GetAnimationCounter() == 5)
            Render2DMesh(meshList[GEO_WALK_FRAME5], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y());
        else if (theHero->GetAnimationCounter() == 6)
            Render2DMesh(meshList[GEO_WALK_FRAME6], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y());
        else if (theHero->GetAnimationCounter() == 7)
            Render2DMesh(meshList[GEO_WALK_FRAME7], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y());
        else if (theHero->GetAnimationCounter() == 8)
            Render2DMesh(meshList[GEO_WALK_FRAME8], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y());
        else if (theHero->GetAnimationCounter() == 9)
            Render2DMesh(meshList[GEO_WALK_FRAME9], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y());
        else if (theHero->GetAnimationCounter() == 10)
            Render2DMesh(meshList[GEO_WALK_FRAME10], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y());
        else if (theHero->GetAnimationCounter() == 11)
            Render2DMesh(meshList[GEO_WALK_FRAME11], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y());
        else
            Render2DMesh(meshList[GEO_WALK_FRAME1], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y());
    }
    else if (theHero->GetAnimationInvert() == true)
    {
        if (theHero->GetAnimationCounter() == 1)
            Render2DMesh(meshList[GEO_WALK_FRAME1], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
        else if (theHero->GetAnimationCounter() == 2)
            Render2DMesh(meshList[GEO_WALK_FRAME2], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
        else if (theHero->GetAnimationCounter() == 3)
            Render2DMesh(meshList[GEO_WALK_FRAME3], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
        else if (theHero->GetAnimationCounter() == 4)
            Render2DMesh(meshList[GEO_WALK_FRAME4], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
        else if (theHero->GetAnimationCounter() == 5)
            Render2DMesh(meshList[GEO_WALK_FRAME5], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
        else if (theHero->GetAnimationCounter() == 6)
            Render2DMesh(meshList[GEO_WALK_FRAME6], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
        else if (theHero->GetAnimationCounter() == 7)
            Render2DMesh(meshList[GEO_WALK_FRAME7], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
        else if (theHero->GetAnimationCounter() == 8)
            Render2DMesh(meshList[GEO_WALK_FRAME8], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
        else if (theHero->GetAnimationCounter() == 9)
            Render2DMesh(meshList[GEO_WALK_FRAME9], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
        else if (theHero->GetAnimationCounter() == 10)
            Render2DMesh(meshList[GEO_WALK_FRAME10], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
        else if (theHero->GetAnimationCounter() == 11)
            Render2DMesh(meshList[GEO_WALK_FRAME11], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
        else
            Render2DMesh(meshList[GEO_WALK_FRAME1], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
    }
    /*else if (!theHero->walking)
    {
    Render2DMesh(meshList[GEO_STANDING], false, 1.0f, theHero->GetPos_x(), theHero->GetPos_y(), false, !theHero->facingRight);
    }*/


    // Render the enemy
    /*int theEnemy_x = theEnemy->GetPos_x()-theHero->GetMapFineOffset_x();
    int theEnemy_y = theEnemy->GetPos_y();
    if ( ((theEnemy_x>=0) && (theEnemy_x<800)) &&
    ((theEnemy_y>=0) && (theEnemy_y<600)) )
    {
    Render2DMesh(meshList[GEO_TILEENEMY_FRAME0], false, 1.0f, theEnemy_x, theEnemy_y);
    }*/
}

/********************************************************************************
Render the rear tile map. This is a private function for use in this class only
********************************************************************************/
void SP3::RenderRearTileMap()
{
    rearWallOffset_x = (int)(theHero->GetMapOffset_x() / 2);
    rearWallOffset_y = 0;
    rearWallTileOffset_y = 0;
    rearWallTileOffset_x = (int)(rearWallOffset_x / m_cRearMap->GetTileSize());
    if (rearWallTileOffset_x + m_cRearMap->GetNumOfTiles_Width() > m_cRearMap->getNumOfTiles_MapWidth())
        rearWallTileOffset_x = m_cRearMap->getNumOfTiles_MapWidth() - m_cRearMap->GetNumOfTiles_Width();
    rearWallFineOffset_x = rearWallOffset_x % m_cRearMap->GetTileSize();

    int m = 0;
    for (int i = 0; i < m_cRearMap->GetNumOfTiles_Height(); i++)
    {
        for (int k = 0; k < m_cRearMap->GetNumOfTiles_Width() + 1; k++)
        {
            m = rearWallTileOffset_x + k;
            // If we have reached the right side of the Map, then do not display the extra column of tiles.
            if ((rearWallTileOffset_x + k) >= m_cRearMap->getNumOfTiles_MapWidth())
                break;
            if (m_cRearMap->theScreenMap[i][m] == 3)
            {
                Render2DMesh(meshList[GEO_TILESTRUCTURE], false, 1.0f, k*m_cRearMap->GetTileSize() - rearWallFineOffset_x, 575 - i*m_cRearMap->GetTileSize());
            }
        }
    }
}

Missile* SP3::FetchMissile()
{
    for (std::vector<Missile *>::iterator it = MissileList.begin(); it != MissileList.end(); ++it)
    {
        Missile *go = (Missile *)*it;
        if (!go->active)
        {
            go->active = true;
            return go;
        }
    }
}

void SP3::MissileUpdate(float dt)
{
    for (std::vector<Missile *>::iterator it = MissileList.begin(); it != MissileList.end(); ++it)
    {
        Missile *go = (Missile *)*it;
        if (go->active)
        {
            go->update(dt, theHero->GetPos_x(), theHero->GetPos_y());
            if (go->collided)
            {
                go->active = false;
                --lives;
            }
        }
    }
}

void SP3::GameStateRenderText()
{
    std::ostringstream ss;
    switch (State)
    {
    case SP3::Menu:
    {
                            ss.str(string());
                            ss.precision(5);
                            ss << "<1>Start Game";
                            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 30);
    }

        break;
    case SP3::Game:
        //On screen text
    {
                            /*ss.str(string());
                            ss.precision(5);
                            ss << "theEnemy: " << theEnemy->GetPos_x() << ", " << theEnemy->GetPos_y();
                            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);
                            std::ostringstream sss;
                            sss.precision(5);
                            sss << "mapOffset_x: " << theHero->GetMapOffset_x();
                            RenderTextOnScreen(meshList[GEO_TEXT], sss.str(), Color(0, 1, 0), 30, 0, 30);

                            ss.str(string());
                            ss.precision(5);
                            ss << "x: " << theHero->GetPos_x() << " Y:" << theHero->GetPos_y();
                            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 60);*/

                            ss.str(string());
                            ss.precision(5);
                            ss << "Lives: " << lives;
                            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 30);
    }
        break;
    case SP3::End:
        if (lives <= 0)
        {
            ss.str(string());
            ss.precision(5);
            ss << "GAME OVER";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 20, 30);
        }
        else if (lives > 0)
        {
            ss.str(string());
            ss.precision(5);
            ss << "You win!";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 20, 30);
        }

        break;
    default:
        break;
    }


}

void SP3::GameStateUpdate()
{
    switch (State)
    {
    case SP3::Menu:
        if (Application::IsKeyPressed('1'))
        {
            State = SP3::Game;
        }
        /*if (Application::IsKeyPressed('2'))
        {
        State = SceneCollision::End;
        }*/
        break;
    case SP3::Game:
        if (lives <= 0)
        {
            //sceneSoundEngine->play2D(lose);
            State = SP3::End;
        }
        break;
    case SP3::End:
        /*if (Application::IsKeyPressed('2'))
        {
        Exit();
        Init();
        }*/
        break;
    default:
        break;
    }
}

void SP3::GameStateRender()
{

}

void SP3::Scenetransition()
{
    if (theHero->TransitLevel)
    {
        CurrLevel = static_cast<Level>(CurrLevel + 1);
        switch (CurrLevel)
        {
        case SP3::LEVEL1:
            break;
        case SP3::LEVEL2:
            m_cMap->LoadMap("Map\\Map2.csv");
            break;
        case SP3::LEVEL3:
            m_cMap->LoadMap("Map\\Map3.csv");
            break;
        case SP3::LEVEL4:
            m_cMap->LoadMap("Map\\Map4.csv");
            break;
        default:
            break;
        }
        SpawnCharacter();
        theHero->TransitLevel = false;
    }
    
}
void SP3::SpawnCharacter()
{
    int m = 0;
    for (int i = 0; i < m_cMap->GetNumOfTiles_Height(); i++)
    {
        for (int k = 0; k < m_cMap->GetNumOfTiles_Width() + 1; k++)
        {
            m = tileOffset_x + k;
            // If we have reached the right side of the Map, then do not display the extra column of tiles.
            if ((tileOffset_x + k) >= m_cMap->getNumOfTiles_MapWidth())
                break;
            if (m_cMap->theScreenMap[i][m] == 9)
            {
                theHero->SetPos_x(k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x());
                theHero->SetPos_y(575 - i*m_cMap->GetTileSize());
            }
        }
    }

}