#include "SP3.h"
#include "GL\glew.h"

//#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
//#include "Utility.h"
#include "LoadTGA.h"
//#include <sstream>
#include "Strategy_Kill.h"

SP3::SP3()
: m_cMinimap(NULL)
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
, Moving(false)
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
    SceneBase::Init();


	lives = 3;
    // Initialise and load the tile map
	m_cMap = LoadMap();
    m_cMap->Init(600, 800, 24, 32, 600, 800, 25);
    m_cMap->LoadMap("Map\\Map1.csv");

    // Initialise and load the REAR tile map
    m_cRearMap = LoadMap();
    m_cRearMap->Init(600, 800, 24, 32, 600, 1600);
    m_cRearMap->LoadMap("Image//MapDesign_Rear.csv");

	theHero = new CPlayerInfo();
	Character = N_Character();
    // Initialise the hero's position
    SpawnObjects();

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

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
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

	firingDebounce = 0;
	Fire = false;
	chargeTime = 0;
	chargeFire = false;

    CurrLevel = LEVEL1;
}

void SP3::Update(double dt)
{
    SceneBase::Update(dt);
    
    rotateAngle -= Application::camera_yaw;// += (float)(10 * dt);

    camera.Update(dt);

    GameStateUpdate();

    if (jumpsoundtimer > 0)
        jumpsoundtimer -= dt;

    if (State == SP3::Game)
    {
        // Update the hero
		if (Application::IsKeyPressed('A'))
		{
			Character->Movement->MoveLeftRight(true, 1.0f);
			Moving = true;
		}
            
		if (Application::IsKeyPressed('D'))
		{
			Character->Movement->MoveLeftRight(false, 1.0f);
			Moving = true;
		}

		if (!Application::IsKeyPressed('A') && !Application::IsKeyPressed('D'))
		{
			Moving = false;
		}
			
        if (Application::IsKeyPressed(' '))
        {
            Character->Movement->SetToJumpUpwards(true);
            if (jumpsoundtimer <= 0)
            {
                jumpsoundtimer = 0.4f;
                sceneSoundEngine->play2D(jump);
            }

        }

        Character->Movement->HeroUpdate(m_cMap);
        Scenetransition();

		firingDebounce += (float)dt;

		bool KeyDown = false;

		if (Application::IsKeyPressed('J') && firingDebounce > 1.f / fireRate)
		{
			KeyDown = false;
			firingDebounce = 0;
			Character->Movement->ProjectileUpdate(2.f, dt, 1, Projectile::Bullet);
			std::cout << "Fire" << std::endl;
		}
		if (Application::IsKeyPressed('K') && !KeyDown)
		{
			chargeTime += 2 * dt;
			if (chargeTime > 1)
			{
				chargeFire = true;
				KeyDown = true;
			}
		}
		if (!Application::IsKeyPressed('K'))
		{
			chargeTime = 0;
		}
		if (KeyDown && chargeFire)
		{
			chargeFire = false;
			KeyDown = false;
			chargeTime = 0;
			Character->Movement->ProjectileUpdate(2.f, dt, 4, Projectile::ChargeBullet);
			//std::cout << "Fire" << std::endl;
		}

		//std::cout << check1 << " " << check2 << " " << firingDebounce << std::endl;

		for (std::vector<PROJECTILE::Projectile *>::iterator it = Character->Movement->m_projectileList.begin(); it != Character->Movement->m_projectileList.end(); ++it)
		{
			PROJECTILE::Projectile *projectile = (PROJECTILE::Projectile *)*it;
			if (projectile->active)
			{
				projectile->pos += projectile->vel * dt;
			}
		}

        // ReCalculate the tile offsets
        tileOffset_x = (int)(theHero->GetMapOffset_x() / m_cMap->GetTileSize());
        if (tileOffset_x + m_cMap->GetNumOfTiles_Width() > m_cMap->getNumOfTiles_MapWidth())
            tileOffset_x = m_cMap->getNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width();

        // if the hero enters the kill zone, then enemy goes into kill strategy mode
        int checkPosition_X = (int)((Character->Movement->GetMapOffset_x() + Character->Movement->GetPos_x()) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int)((Character->Movement->GetPos_y() + m_cMap->GetTileSize()) / m_cMap->GetTileSize());
        missileTriggerTimer += dt;

       /* if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 3 && missileTriggerTimer > 1.f)
        {
            missileTriggerTimer = 0;
            Missile* go = FetchMissile();
            go->Init(theHero->GetPos_x() - (32 * 11), theHero->GetPos_y());
        }*/

       // MissileUpdate(dt);

        fps = (float)(1.f / dt);
    }
    ProjectileCollision(dt);
	//std::cout << fps << std::endl;
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
	RenderCharacter();
    //Render missiles
    
    RenderList();
	

    GameStateRenderText();
}

void SP3::Exit()
{
    SceneBase::Exit();
    // Cleanup VBO
    /*for (int i = 0; i < NUM_GEOMETRY; ++i)
    {
        if (meshList[i])
            delete meshList[i];
    }
    glDeleteProgram(m_programID);
    glDeleteVertexArrays(1, &m_vertexArrayID);*/
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

    


}

/********************************************************************************
Render the rear tile map. This is a private function for use in this class only
********************************************************************************/
void SP3::RenderRearTileMap()
{
    rearWallOffset_x = (int)(Character->Movement->GetMapOffset_x() / 2);
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
            go->update(dt, Character->Movement->GetPos_x(), Character->Movement->GetPos_y());
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
    if (Character->Movement->TransitLevel)
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
        SpawnObjects();
        Character->Movement->TransitLevel = false;
    }
    
}

void SP3::SpawnObjects()
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
            switch (m_cMap->theScreenMap[i][m])
            {
            case 9:
            {
                      int x = k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x();
                      int y = 575 - i*m_cMap->GetTileSize();
                      Character->Movement->SetPos_x(k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x());
                      Character->Movement->SetPos_y(575 - i*m_cMap->GetTileSize());

            }
                break;
            case 10:
                break;
            case 11:
            {
                       Monster* newmon = N_Monster();
                       Monster_List.push_back(newmon);
                       float x = k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x();
                       float y = 575 - i*m_cMap->GetTileSize();
                       Vector3 temp = Vector3(x, y, 0);
                       newmon->Init(temp, 1.f, 1.f,1.f);
            }

                break;
            default:
                break;
            }
        }
    }

}

void SP3::RenderProjectile(PROJECTILE::Projectile *projectile)
{
	switch (projectile->type)
	{
	case Projectile::Bullet:
			Render2DMesh(meshList[GEO_MISSILE], false, projectile->GetScale().x, projectile->GetPos().x, projectile->GetPos().y, !projectile->Left);
			break;
	case Projectile::ChargeBullet:
			Render2DMesh(meshList[GEO_MISSILE], false, projectile->GetScale().x, projectile->GetPos().x, projectile->GetPos().y - (m_cMap->GetTileSize() * projectile->GetScale().y * 0.5) + m_cMap->GetTileSize() * 0.5 , !projectile->Left);
			break;
	case Projectile::Net:
			Render2DMesh(meshList[GEO_MISSILE], false, projectile->GetScale().x, projectile->GetPos().x, projectile->GetPos().y, !projectile->Left);
			break;
	}
	

}

void SP3::RenderCharacter()
{
    //meshList[GEO_WALK]
	if (Character->Movement->GetAnimationInvert() == false && Moving == true)
	{
		if (Character->Movement->GetAnimationCounter() == 1)
			Render2DMesh(meshList[GEO_WALK_FRAME1], false, 1.0f, Character->Movement->GetPos_x(),Character->Movement->GetPos_y());
		else if (Character->Movement->GetAnimationCounter() == 2)
			Render2DMesh(meshList[GEO_WALK_FRAME2], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y());
		else if (Character->Movement->GetAnimationCounter() == 3)
			Render2DMesh(meshList[GEO_WALK_FRAME3], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y());
		else if (Character->Movement->GetAnimationCounter() == 4)
			Render2DMesh(meshList[GEO_WALK_FRAME4], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y());
		else if (Character->Movement->GetAnimationCounter() == 5)
			Render2DMesh(meshList[GEO_WALK_FRAME5], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y());
		else if (Character->Movement->GetAnimationCounter() == 6)
			Render2DMesh(meshList[GEO_WALK_FRAME6], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y());
		else if (Character->Movement->GetAnimationCounter() == 7)
			Render2DMesh(meshList[GEO_WALK_FRAME7], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y());
		else if (Character->Movement->GetAnimationCounter() == 8)
			Render2DMesh(meshList[GEO_WALK_FRAME8], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y());
		else if (Character->Movement->GetAnimationCounter() == 9)
			Render2DMesh(meshList[GEO_WALK_FRAME9], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y());
		else if (Character->Movement->GetAnimationCounter() == 10)
			Render2DMesh(meshList[GEO_WALK_FRAME10], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y());
		else if (Character->Movement->GetAnimationCounter() == 11)
			Render2DMesh(meshList[GEO_WALK_FRAME11], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y());
		else
			Render2DMesh(meshList[GEO_WALK_FRAME1], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y());
	}
	else if (Character->Movement->GetAnimationInvert() == true && Moving == true)
	{
		if (Character->Movement->GetAnimationCounter() == 1)
			Render2DMesh(meshList[GEO_WALK_FRAME1], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), true, false);
		else if (Character->Movement->GetAnimationCounter() == 2)
			Render2DMesh(meshList[GEO_WALK_FRAME2], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), true, false);
		else if (Character->Movement->GetAnimationCounter() == 3)
			Render2DMesh(meshList[GEO_WALK_FRAME3], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), true, false);
		else if (Character->Movement->GetAnimationCounter() == 4)
			Render2DMesh(meshList[GEO_WALK_FRAME4], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), true, false);
		else if (Character->Movement->GetAnimationCounter() == 5)
			Render2DMesh(meshList[GEO_WALK_FRAME5], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), true, false);
		else if (Character->Movement->GetAnimationCounter() == 6)
			Render2DMesh(meshList[GEO_WALK_FRAME6], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), true, false);
		else if (Character->Movement->GetAnimationCounter() == 7)
			Render2DMesh(meshList[GEO_WALK_FRAME7], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), true, false);
		else if (Character->Movement->GetAnimationCounter() == 8)
			Render2DMesh(meshList[GEO_WALK_FRAME8], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), true, false);
		else if (Character->Movement->GetAnimationCounter() == 9)
			Render2DMesh(meshList[GEO_WALK_FRAME9], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), true, false);
		else if (Character->Movement->GetAnimationCounter() == 10)
			Render2DMesh(meshList[GEO_WALK_FRAME10], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), true, false);
		else if (Character->Movement->GetAnimationCounter() == 11)
			Render2DMesh(meshList[GEO_WALK_FRAME11], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), true, false);
		else
			Render2DMesh(meshList[GEO_WALK_FRAME1], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), true, false);
	}
	else if (Moving == false)
	{
		Render2DMesh(meshList[GEO_STANDING], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), !Character->Movement->facingRight, false);
	}
}

void SP3::RenderList()
{
    for (std::vector<Monster*>::iterator it = Monster_List.begin(); it != Monster_List.end(); ++it)
    {
        Monster* go = (Monster*)*it;
        Render2DMesh(meshList[GEO_GASTLY], false, 1.0f, go->Movement->GetPos_X(), go->Movement->GetPos_Y());
    }
    for (std::vector<Missile *>::iterator it = MissileList.begin(); it != MissileList.end(); ++it)
    {
        Missile *go = (Missile *)*it;
        if (go->active)
        {
            Render2DMesh(meshList[GEO_MISSILE], false, 1.0f, go->x, go->y, false, true);
        }
    }

    for (std::vector<PROJECTILE::Projectile *>::iterator it = Character->Movement->m_projectileList.begin(); it != Character->Movement->m_projectileList.end(); ++it)
    {
        PROJECTILE::Projectile *projectile = (PROJECTILE::Projectile *)*it;
        if (projectile->active)
        {
            RenderProjectile(projectile);
            
        }
    }
}

void SP3::ProjectileCollision(double dt)
{
    for (std::vector<PROJECTILE::Projectile *>::iterator it = Character->Movement->m_projectileList.begin(); it != Character->Movement->m_projectileList.end(); ++it)
    {
        PROJECTILE::Projectile *projectile = (PROJECTILE::Projectile *)*it;
        if (projectile->active)
        {
            for (std::vector<Monster*>::iterator it2 = Monster_List.begin(); it2 != Monster_List.end(); ++it2)
            {
				Monster* go = (Monster*)*it2;
                int tsize = ((m_cMap->GetTileSize() * projectile->GetScale().x) - (6 * projectile->GetScale().x)) * 0.5;
                Vector3 pos1(projectile->pos.x + tsize, projectile->pos.y + tsize, 0);
                Vector3 pos2(go->Movement->GetPos_X() + tsize, go->Movement->GetPos_Y() + tsize, 0);
                if (Collision::SphericalCollision(pos1, tsize, pos2, tsize, dt))
                {
                    projectile->active = false;
                    Monster_List.erase(it2);
                    break;
                }
            }
        }
    }

}