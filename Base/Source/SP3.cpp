#include "SP3.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
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
    if (m_cMap)
    {
        delete m_cMap;
        m_cMap = NULL;
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
	CurrLevel = LEVEL1;
    // Initialise and load the tile map
	m_cMap = LoadMap();
    m_cMap->Init(600, 800, 24, 32, 600, 800, 25);
	m_cMap->LoadMap("Map\\Map1.csv");
    

    // Initialise and load the REAR tile map
    m_cRearMap = LoadMap();
    m_cRearMap->Init(600, 800, 24, 32, 600, 1600);
    m_cRearMap->LoadMap("Image//MapDesign_Rear.csv");

	//theHero = new CPlayerInfo();
	Character = N_Character();
	AI = N_AI();
    SpawnObjects();

	Mtx44 perspective;
    perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    //perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
    projectionStack.LoadMatrix(perspective);

    rotateAngle = 0;

    bLightEnabled = true;

    sceneSoundEngine = createIrrKlangDevice();
    //source = sceneSoundEngine->addSoundSourceFromFile("music//etc.ogg")
    jump = sceneSoundEngine->addSoundSourceFromFile("music//jump.ogg");

    jumpsoundtimer = 0;

	firingDebounce = 0;
	Fire = false;
	chargeTime = 0;
	chargeFire = false;
	chargeDmg = 0;
	BossFiringDebounce = 0;
	winTimer = 0;

	for (int i = 0; i < 1; ++i)
	{
		Character->Movement->m_projectileList.push_back(new Projectile(m_cMap));
	}

	m_objectCount = 0;
	Play.Init(&m_goList, m_cMap);

	Main.Init(&Play, Character);

    m_particleCount = 0;
    MAX_PARTICLE = 420;
    m_gravity.Set(0, -9.8f, 0);

    for (unsigned i = 0; i <= 400; ++i)
    {
        ParticleObject *particle = new ParticleObject(ParticleObject_TYPE::P_WATER);
        particleList.push_back(particle);
    }

    /*spritemanager = new SpriteManager();
    spritemanager->Init(800, 600);*/
    Battle = new BattleStage();
    Battle->Init(800, 600, 25);


    battlestage = true;

	MiniBossAlive = true;

}

void SP3::Update(double dt)
{
    SceneBase::Update(dt);
    
	Main.Update(dt);

   // rotateAngle -= Application::camera_yaw;// += (float)(10 * dt);

    //camera.Update(dt);

    GameStateUpdate();

    if (jumpsoundtimer > 0)
        jumpsoundtimer -= dt;


	if (Main.gamestate == Main.Game)
	{
        if (!battlestage)
        {
            Scenetransition();
            //sprite update
            //spritemanager->update(dt);
            //battlestage update

            // Update the hero
            if (Application::IsKeyPressed('A'))
            {
                Moving = true;
                Character->Movement->MoveLeftRight(true, 1.0f);
            }

            if (Application::IsKeyPressed('D'))
            {
                Moving = true;
                Character->Movement->MoveLeftRight(false, 1.0f);
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
					//sceneSoundEngine->play2D(jump);
				}

			}

			Character->Movement->HeroUpdate(m_cMap);
			Character->Attribute->update(dt);
            Character->Update(dt);
		}
		
		else
		{
			//battlestage update
			Battle->Update(dt);
		}
		// Update the hero
		

            float ActionIncrease = 0;
            ActionIncrease += dt;

            firingDebounce += (float)dt;
            bool KeyUp = true;

            //Normal Projectile
            if (Application::IsKeyPressed('J') && firingDebounce > 2.f / fireRate)
            {
                firingDebounce = 0;
                Character->Movement->ProjectileUpdate(dt, 1, Character->Attribute->GetDmg(), Projectile::Bullet, m_cMap);
            }


            //Net
            if (Application::IsKeyPressed('L') && firingDebounce > 2.f / fireRate)
            {
                firingDebounce = 0;
                Character->Movement->ProjectileUpdate(dt, 1, 1, Projectile::Net, m_cMap);
            }

            //Charge Projectile
            if (Application::IsKeyPressed('K') && KeyUp && Character->Attribute->GetActionBar() >= 50)
            {
                chargeTime += 2 * dt;
                chargeDmg = chargeTime;
                if (chargeDmg > 1.5)
                {
                    chargeDmg = 1.5;
                }
                if (chargeTime > 2)
                {
                    chargeTime = 2;
                    chargeFire = true;
                }
                KeyUp = false;
            }
            if (!Application::IsKeyPressed('K') && KeyUp && !chargeFire)
            {
                if (chargeTime > 0)
                {
                    chargeFire = true;
                    chargeTime = 0;
                }
            }
            if (!Application::IsKeyPressed('K') && KeyUp && !chargeFire)
            {
                chargeTime = 0;
            }
            if (!Application::IsKeyPressed('K') && KeyUp && chargeFire)
            {
                Character->Attribute->ActionBar(-50);
                chargeFire = false;
                KeyUp = false;
                chargeTime = 0;
                Character->Movement->ProjectileUpdate(dt, 1, (Character->Attribute->GetDmg() *  chargeDmg), Projectile::ChargeBullet, m_cMap);
                std::cout << "Fire" << std::endl;
            }
        

            std::cout << AI->Monster->Movement->GetPos() << std::endl;

            for (std::vector<PROJECTILE::Projectile *>::iterator it = Character->Movement->m_projectileList.begin(); it != Character->Movement->m_projectileList.end(); ++it)
            {
                PROJECTILE::Projectile *projectile = (PROJECTILE::Projectile *)*it;
                if (projectile->active)
                {
                    //projectile->SetPos(projectile->GetPos() + projectile->GetVel() * dt);
                    projectile->Update(dt);
                    ProjectileCollision(dt, projectile);
                }
            }

            // ReCalculate the tile offsets
            tileOffset_x = (int)(Character->Movement->GetMapOffset_x() / m_cMap->GetTileSize());
            if (tileOffset_x + m_cMap->GetNumOfTiles_Width() > m_cMap->getNumOfTiles_MapWidth())
                tileOffset_x = m_cMap->getNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width();

            // if the hero enters the kill zone, then enemy goes into kill strategy mode

            MonsterUpdate(dt, m_cMap);
            //SpriteAnimationUpdate(dt);
            UpdateParticles(dt);
        }
       
		
		if (MiniBossAlive == false)
		{
			winTimer += (float)dt;
			if (winTimer > 5)
			{
				Main.gamestate = Main.Win;
				winTimer = 0;
			}
		}

	//if (Main.gamestate == Main.Restart)
	//{
	//	Main.playerDead = true;
	//	State = Game;
	//	Restart();
	//}

	if (Main.RestartGame)
	{
		Restart();
		Main.RestartGame = false;
	}

	if (Character->Attribute->GetCurrentHP() <= 0)
	{
		State = End;
		Main.gamestate = Main.End;
		Main.RestartGame = false;
	}

	if (Main.gamestate == Main.Menu)
	{
		State = Menu;
		Main.RestartGame = false;
	}

	if (Main.gamestate == Main.Win)
	{
		State = Win;
		Main.RestartGame = false;
	}
	
	//std::cout << Main.deadArrow << std::endl;
	//std::cout << fps << std::endl;
}

void SP3::RenderGO(GameObject *go)
{
	//rotate code
	modelStack.Rotate(Math::RadianToDegree(atan2(-go->vel.x, go->vel.y)), 0, 0, 1);

	modelStack.PushMatrix();
	Vector3 temp;
	switch (go->type)
	{
	case(GameObject::GO_PLAY) :
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PLAY], false);
		break;

	case(GameObject::GO_PLAYHOVER) :
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PLAYHOVER], false);
		break;

	case(GameObject::GO_MENU) :
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_MENU], false);
		break;

	case(GameObject::GO_MENUHOVER) :
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_MENUHOVER], false);
		break;

	case(GameObject::GO_HIGHSCORE) :
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_HIGHSCORE], false);
		break;

	case(GameObject::GO_EXIT) :
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_EXIT], false);
		break;

	case(GameObject::GO_EXITHOVER) :
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_EXITHOVER], false);
		break;

	case(GameObject::GO_RESTART) :
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_RESTART], false);
		break;

	//case(GameObject::GO_RESTARTHOVER) :
	//	modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
	//	modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//	RenderMesh(meshList[GEO_RESTARTHOVER], false);
	//	break;

	case(GameObject::GO_OKAYHOVER) :
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_OKAYHOVER], false);
		break;

	case(GameObject::GO_HELP) :
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_HELP], false);
		break;

	case(GameObject::GO_HELPHOVER) :
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_HELPHOVER], false);
		break;

	case(GameObject::GO_RESUME) :
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_RESUME], false);
		break;

	case(GameObject::GO_RESUMEHOVER) :
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_RESUMEHOVER], false);
		break;

	default:
		break;
	}
	modelStack.PopMatrix();
}

void SP3::RenderBackground()
{
	if (CurrLevel == SP3::LEVEL1)
		Render2DMesh(meshList[GEO_BACKGROUND], false, 1.0f, 0.f, 0.f, false, false);
	else if (CurrLevel == SP3::LEVEL2)
		Render2DMesh(meshList[GEO_FORESTBACKGROUND], false, 1.0f, 0.f, 0.f, false, false);
	else if (CurrLevel == SP3::LEVEL3)
		Render2DMesh(meshList[GEO_CAVEBACKGROUND], false, 1.0f, 0.f, 0.f, false, false);
	else if (CurrLevel == SP3::LEVEL4)
		Render2DMesh(meshList[GEO_CASTLEBACKGROUND], false, 1.0f, 0.f, 0.f, false, false);
	else if (CurrLevel == SP3::LEVEL5)
		Render2DMesh(meshList[GEO_BACKGROUND], false, 1.0f, 0.f, 0.f, false, false);
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

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);

		}
	}

	switch (Main.gamestate)
	{
	case MainMenu::GameState::Menu:
		Main.RenderMenu(m_cMap);
		break;

	case MainMenu::GameState::Game:
        if (battlestage)
        {
            renderbattlestage();
        }
        else
        {
            RenderBackground();
            //RenderRearTileMap();
            // Render the tile map
            RenderTileMap();
            RenderCharacter();
            RenderList();
            //spritemanager->spriteRender();
            Main.RenderMenu(m_cMap);

            std::ostringstream ss;
            ss.str(string());
            ss.precision(7);
            ss << Character->getScore();
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 20, 25, 550);

        }
		break;

	case MainMenu::GameState::Pause:
		Main.RenderMenu(m_cMap);
		break;

	case MainMenu::GameState::Help:
		Main.RenderMenu(m_cMap);
		break;

	case MainMenu::GameState::End:
		Main.RenderMenu(m_cMap);
		break;

	case MainMenu::GameState::Win:
		Main.RenderMenu(m_cMap);
		break;
	}
    
    GameStateRenderText();

}

void SP3::Exit()
{
    SceneBase::Exit();

    particleList.clear();
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
				Render2DMesh(meshList[GEO_SNOW_DIRT_TILE], false, 1.0f, k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize() , false);
			}
			else if (m_cMap->theScreenMap[i][m] == 2)
			{
                Render2DMesh(meshList[GEO_SNOW_TILE], false, 1.0f, k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize() - 0.3f, false);
			}

			else if (m_cMap->theScreenMap[i][m] == 10)
			{
				Render2DMesh(meshList[GEO_TILE_KILLZONE], false, 1.0f, k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize(), false);
			}
            else if (m_cMap->theScreenMap[i][m] == 11)
            {
                Render2DMesh(meshList[GEO_TILE_KILLZONE], false, 1.0f, k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize(), false);
            }
            else if (m_cMap->theScreenMap[i][m] == 8)
            {
                Render2DMesh(meshList[GEO_MINIBOSS_TILE1], false, 1.0f, k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize(), false);
            }
            else if (m_cMap->theScreenMap[i][m] == 16)
            {
                Render2DMesh(meshList[GEO_MINIBOSS_TILE2], false, 1.0f, k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize(), false);
            }



			if (Level::LEVEL2)
			{
				if (m_cMap->theScreenMap[i][m] == 3)
				{
					Render2DMesh(meshList[GEO_GRASS], false, 1.0f, k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize(), false);
				}
				else if (m_cMap->theScreenMap[i][m] == 4)
				{
					Render2DMesh(meshList[GEO_DIRT], false, 1.0f, k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize(), false);
				}
				else if (m_cMap->theScreenMap[i][m] == 44)
				{
					Render2DMesh(meshList[GEO_DIRT], false, 1.0f, k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize(), false);
				}
			}
			
			if (Level::LEVEL3)
			{
				if (m_cMap->theScreenMap[i][m] == 5)
				{
					Render2DMesh(meshList[GEO_CAVE], false, 1.0f, k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize(), false);
				}
				else if (m_cMap->theScreenMap[i][m] == 55)
				{
					Render2DMesh(meshList[GEO_CAVE], false, 1.0f, k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize(), false);
				}
			}
			
			if (Level::LEVEL4)
			{
				if (m_cMap->theScreenMap[i][m] == 6)
				{
					Render2DMesh(meshList[GEO_CASTLE], false, 1.0f, k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize(), false);
				}
				else if (m_cMap->theScreenMap[i][m] == 7)
				{
					Render2DMesh(meshList[GEO_CASTLEGROUND], false, 1.0f, k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize(), false);
				}
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
                Render2DMesh(meshList[GEO_TILESTRUCTURE], false, 1.0f, k*m_cRearMap->GetTileSize() - rearWallFineOffset_x, 575 - i*m_cRearMap->GetTileSize(), false);
            }
			if (m_cRearMap->theScreenMap[i][m] == 93)
			{
				Render2DMesh(meshList[GEO_CAVEBACKGROUND], false, 1.f, k*m_cRearMap->GetTileSize() - rearWallFineOffset_x, 575 - i*m_cRearMap->GetTileSize(), false);
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
		/*ss.str(string());
		ss.precision(5);
		ss << Character->Attribute->GetCurrentHP();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 150, 16);*/
    }
        break;
    case SP3::Game:
        //On screen text
    {
        ss.str(string());
        ss.precision(5);
		ss << Character->Attribute->GetCurrentHP();
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 150, 16);
    }
        break;
    case SP3::End:
        if (lives <= 0)
        {
            /*ss.str(string());
            ss.precision(5);
            ss << "GAME OVER";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 20, 30);*/
        }
        else if (lives > 0)
        {
            //ss.str(string());
            //ss.precision(5);
            //ss << "You win!";
            //RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 20, 30);

			/*ss.str(string());
			ss.precision(5);
			ss << Character->Attribute->GetCurrentHP();
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 5, 30);*/
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

void SP3::Restart()
{
	CurrLevel = LEVEL1;
	m_cMap->LoadMap("Map\\Map1.csv");
	Character->Restart();
	Character->Attribute->SetCurrentHP(Character->Attribute->GetMaxHP());
	MiniBossAlive = true;
	SpawnObjects();

}

void SP3::Scenetransition()
{
    if (Character->Movement->TransitLevel)
    {
        CurrLevel = static_cast<Level>(CurrLevel + 1);
        switch (CurrLevel)
        {
		case SP3::LEVEL1:
			m_cMap->LoadMap("Map\\Map1.csv");
            break;
        case SP3::LEVEL2:
            m_cMap->LoadMap("Map\\MapMiniBoss.csv");
            break;
        case SP3::LEVEL3:	
            m_cMap->LoadMap("Map\\Map3.csv");
            break;
        case SP3::LEVEL4:
            m_cMap->LoadMap("Map\\Map4.csv");
            break;
		case SP3::LEVEL5:
			m_cMap->LoadMap("Map\\MapMiniBoss.csv");
			break;
        default:
            break;
        }
        SpawnObjects();
        Character->Movement->TransitLevel = false;
    }

	if (Character->Movement->TransitLevel2)
	{
		CurrLevel = static_cast<Level>(CurrLevel + 1);
		switch (CurrLevel)
		{
		case SP3::LEVEL2:
			m_cMap->LoadMap("Map\\Map2B.csv");
			break;
		case SP3::LEVEL3:
			m_cMap->LoadMap("Map\\Map3B.csv");
			break;
		case SP3::LEVEL4:
			m_cMap->LoadMap("Map\\Map4B.csv");
			break;
		case SP3::LEVEL5:
			m_cMap->LoadMap("Map\\MapMiniBoss.csv");
			break;
		default:
			break;
		}
		SpawnObjects();
		Character->Movement->TransitLevel2 = false;
	}
}

void SP3::SpawnObjects()
{
    Monster_List.clear();
	Character->Movement->m_projectileList.clear();
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
					  int x = k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x();
                      int y = 575 - i*m_cMap->GetTileSize();
					  Character->Movement->SetPos_x(k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x());
                      Character->Movement->SetPos_y(575 - i*m_cMap->GetTileSize());

            }
                break;
            case 12:
            {
                       Monster* newmon = N_Monster();
                       Monster_List.push_back(newmon);
                       float x = k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x();
                       float y = 575 - i*m_cMap->GetTileSize();
                       Vector3 temp = Vector3(x, y, 0);
                       newmon->Init(temp,Vector3(1,1,1),6 * m_cMap->GetTileSize(),5.f,m_cMap->GetTileSize(),Monster::GASTLY,m_cMap, true, true);
                       newmon->InitAttrib(25, 1,50,1);
            }
                break;
            
            case 13:
            {
                       Monster* newmon = N_Monster();
                       Monster_List.push_back(newmon);
                       float x = k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x();
                       float y = 575 - i*m_cMap->GetTileSize();
                       Vector3 temp = Vector3(x, y, 0);
                       newmon->Init(temp, Vector3(1, 1, 1), 6 * m_cMap->GetTileSize(), 5.f, m_cMap->GetTileSize(), Monster::MONSTER2, m_cMap, true, true);
                       newmon->InitAttrib(50, 1,50,1);
            }
                break;
            case 14:
            {
                       Monster* newmon = N_Monster();
                       Monster_List.push_back(newmon);
                       float x = k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x();
                       float y = 575 - i*m_cMap->GetTileSize();
                       Vector3 temp = Vector3(x, y, 0);
                       newmon->Init(temp, Vector3(1, 1, 1), 6 * m_cMap->GetTileSize(), 5.f, m_cMap->GetTileSize(), Monster::MONSTER3, m_cMap, true, true);
                       newmon->InitAttrib(75, 1,50,1);
            }
				break;
			case 15:
			{
					   Monster* newmon = N_Monster();
					   Monster_List.push_back(newmon);
					   float x = k*m_cMap->GetTileSize() - Character->Movement->GetMapFineOffset_x();
					   float y = 575 - i*m_cMap->GetTileSize();
					   Vector3 temp = Vector3(x, y, 0);
					   newmon->Init(temp, Vector3(3, 3, 1), 15 * m_cMap->GetTileSize(), 5.f, m_cMap->GetTileSize(), Monster::MINIBOSS, m_cMap, false, true);
					   newmon->InitAttrib(150, 50, 10, 1);
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
	if (Main.gamestate == Main.Game)
	{
		switch (projectile->type)
		{
		case Projectile::Bullet:
			Render2DMesh(meshList[GEO_N_SHOT], false, projectile->GetScale().x, projectile->GetPos().x, projectile->GetPos().y, projectile->Left);
			break;
		case Projectile::ChargeBullet:
			Render2DMesh(meshList[GEO_C_SHOT], false, projectile->GetScale().x, projectile->GetPos().x, projectile->GetPos().y - (m_cMap->GetTileSize() * projectile->GetScale().y * 0.5) + m_cMap->GetTileSize() * 0.5, projectile->Left);
			break;
		case Projectile::Net:
			Render2DMesh(meshList[GEO_NET], false, projectile->GetScale().x, projectile->GetPos().x, projectile->GetPos().y, projectile->Left);
			break;
		case Projectile::BossBullet:
			Render2DMesh(meshList[GEO_BOSS_PROJECTILE], false, projectile->GetScale().x, projectile->GetPos().x, projectile->GetPos().y, projectile->Left);
			break;
		}

	}
	
}

void SP3::RenderCharacter()
{
	if (Character->Movement->GetAnimationInvert() == false && Moving == true)
	{
		if (Character->Movement->GetAnimationCounter() == 1)
            Render2DMesh(meshList[GEO_WALK_FRAME1], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), false);
		else if (Character->Movement->GetAnimationCounter() == 2)
            Render2DMesh(meshList[GEO_WALK_FRAME2], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), false);
		else if (Character->Movement->GetAnimationCounter() == 3)
            Render2DMesh(meshList[GEO_WALK_FRAME3], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), false);
		else if (Character->Movement->GetAnimationCounter() == 4)
            Render2DMesh(meshList[GEO_WALK_FRAME4], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), false);
		else if (Character->Movement->GetAnimationCounter() == 5)
            Render2DMesh(meshList[GEO_WALK_FRAME5], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), false);
		else if (Character->Movement->GetAnimationCounter() == 6)
            Render2DMesh(meshList[GEO_WALK_FRAME6], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), false);
		else if (Character->Movement->GetAnimationCounter() == 7)
            Render2DMesh(meshList[GEO_WALK_FRAME7], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), false);
		else if (Character->Movement->GetAnimationCounter() == 8)
            Render2DMesh(meshList[GEO_WALK_FRAME8], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), false);
		else if (Character->Movement->GetAnimationCounter() == 9)
            Render2DMesh(meshList[GEO_WALK_FRAME9], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), false);
		else if (Character->Movement->GetAnimationCounter() == 10)
            Render2DMesh(meshList[GEO_WALK_FRAME10], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), false);
		else if (Character->Movement->GetAnimationCounter() == 11)
            Render2DMesh(meshList[GEO_WALK_FRAME11], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), false);
		else
            Render2DMesh(meshList[GEO_WALK_FRAME1], false, 1.0f, Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), false);
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

    Render2DMesh(meshList[GEO_MON_HP_BAR],false, 0.1f, (Character->Attribute->GetActionBar() / 100.f) * 2 , Character->Movement->GetPos_x() - 2.f, Character->Movement->GetPos_y(),false,false);

}

void SP3::RenderList()
{
	for (std::vector<PROJECTILE::Projectile *>::iterator it = Character->Movement->m_projectileList.begin(); it != Character->Movement->m_projectileList.end(); ++it)
	{
		PROJECTILE::Projectile *projectile = (PROJECTILE::Projectile *)*it;
		if (projectile->active)
		{
			RenderProjectile(projectile);

		}
	}
    for (std::vector<Monster*>::iterator it = Monster_List.begin(); it != Monster_List.end(); ++it)
    {
        Monster* go = (Monster*)*it;
        switch (go->type)
        {
        case Monster::GASTLY:
            Render2DMesh(meshList[GEO_GASTLY], false, go->Movement->GetScale_X(), go->Movement->GetPos_X(), go->Movement->GetPos_Y(), !go->Movement->faceleft(),false);
            Render2DMesh(meshList[GEO_MON_HP_BAR], false, go->Attribute->GetCurrentHP() * pow(go->Attribute->GetMonsterMaxHealth(),-1),0.8, go->Movement->GetPos_X(), go->Movement->GetPos_Y() + (m_cMap->GetTileSize()),false,false);

            break;
        case Monster::MONSTER2:
            Render2DMesh(meshList[GEO_HAUNTER], false, go->Movement->GetScale_X(), go->Movement->GetPos_X(), go->Movement->GetPos_Y(), !go->Movement->faceleft());
            Render2DMesh(meshList[GEO_MON_HP_BAR], false, go->Attribute->GetCurrentHP() * pow(go->Attribute->GetMonsterMaxHealth(), -1), 0.8, go->Movement->GetPos_X(), go->Movement->GetPos_Y() + (m_cMap->GetTileSize()), false, false);
            break;
        case Monster::MONSTER3:
            Render2DMesh(meshList[GEO_MONSTER3], false, go->Movement->GetScale_X(), go->Movement->GetPos_X(), go->Movement->GetPos_Y(), !go->Movement->faceleft());
            Render2DMesh(meshList[GEO_MON_HP_BAR], false, go->Attribute->GetCurrentHP() * pow(go->Attribute->GetMonsterMaxHealth(), -1), 0.8, go->Movement->GetPos_X(), go->Movement->GetPos_Y() + (m_cMap->GetTileSize()), false, false);
            break;
		case Monster::MINIBOSS:
            //Render2DMesh(meshList[GEO_MINIBOSS], false, go->Movement->GetScale_X(), go->Movement->GetPos_X(), go->Movement->GetPos_Y(), !go->Movement->faceleft());
            Render2DMesh(meshList[GEO_MON_HP_BAR], false, (go->Attribute->GetCurrentHP() * pow(go->Attribute->GetMonsterMaxHealth(), -1)) * go->Movement->GetScale_X(), 0.8, go->Movement->GetPos_X(), go->Movement->GetPos_Y() + (m_cMap->GetTileSize() * go->Movement->GetScale_Y()), false, false);
			if (go->Movement->GetAnimationCounter() == 0)
				Render2DMesh(meshList[GEO_GENGAR_FRAME0], false, go->Movement->GetScale_X(), go->Movement->GetPos_X(), go->Movement->GetPos_Y(), !go->Movement->faceleft());
			else if (go->Movement->GetAnimationCounter() == 1)
				Render2DMesh(meshList[GEO_GENGAR_FRAME1], false, go->Movement->GetScale_X(), go->Movement->GetPos_X(), go->Movement->GetPos_Y(), !go->Movement->faceleft());
			else if (go->Movement->GetAnimationCounter() == 2)
				Render2DMesh(meshList[GEO_GENGAR_FRAME2], false, go->Movement->GetScale_X(), go->Movement->GetPos_X(), go->Movement->GetPos_Y(), !go->Movement->faceleft());
			else if (go->Movement->GetAnimationCounter() == 3)
				Render2DMesh(meshList[GEO_GENGAR_FRAME3], false, go->Movement->GetScale_X(), go->Movement->GetPos_X(), go->Movement->GetPos_Y(), !go->Movement->faceleft());
			else if (go->Movement->GetAnimationCounter() == 4)
				Render2DMesh(meshList[GEO_GENGAR_FRAME4], false, go->Movement->GetScale_X(), go->Movement->GetPos_X(), go->Movement->GetPos_Y(), !go->Movement->faceleft());
			else if (go->Movement->GetAnimationCounter() == 5)
				Render2DMesh(meshList[GEO_GENGAR_FRAME5], false, go->Movement->GetScale_X(), go->Movement->GetPos_X(), go->Movement->GetPos_Y(), !go->Movement->faceleft());
			else if (go->Movement->GetAnimationCounter() == 6)
				Render2DMesh(meshList[GEO_GENGAR_FRAME6], false, go->Movement->GetScale_X(), go->Movement->GetPos_X(), go->Movement->GetPos_Y(), !go->Movement->faceleft());
			else if (go->Movement->GetAnimationCounter() == 7)
				Render2DMesh(meshList[GEO_GENGAR_FRAME7], false, go->Movement->GetScale_X(), go->Movement->GetPos_X(), go->Movement->GetPos_Y(), !go->Movement->faceleft());

            break;
        default:
            break;
        }
    }

    
    //particles
    for (std::vector<ParticleObject*>::iterator it = particleList.begin();
        it != particleList.end(); ++it)
    {
        ParticleObject *particle = (ParticleObject *)*it;
        if (particle->active)
        {
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            /*modelStack.PushMatrix();
            modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
            modelStack.Rotate(particle->rotation, 0, 1, 0);
            modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);*/
            Render2DMesh(meshList[GEO_QUAD], false, particle->scale.x, particle->pos.x, particle->pos.y,false,false);
            //modelStack.PopMatrix();
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }
}

void SP3::ProjectileCollision(double dt, Projectile* projectile)
{    
    if (projectile->active)
    {
        //out of map
        if (projectile->GetPos().x > (m_cMap->getNumOfTiles_MapWidth() * (m_cMap->GetTileSize() + 2)) ||
            projectile->GetPos().x < (0 - (m_cMap->GetTileSize() + 2)))
        {
            projectile->active = false;
            return;
        }
        //collide with monster
        for (std::vector<Monster*>::iterator it2 = Monster_List.begin(); it2 != Monster_List.end(); ++it2)
        {
            Monster* go = (Monster*)*it2;
            int tsize = ((m_cMap->GetTileSize() * projectile->GetScale().x) - (6 * projectile->GetScale().x)) * 0.5;
            Vector3 pos1(projectile->GetPos().x + tsize, projectile->GetPos().y + tsize, 0);
            int tsize2 = ((m_cMap->GetTileSize() * go->Movement->GetScale_X()) - (6 * go->Movement->GetScale_X())) * 0.5;
            Vector3 pos2(go->Movement->GetPos_X() + tsize2, go->Movement->GetPos_Y() + tsize2, 0);

			int tsize3 = ((m_cMap->GetTileSize() * 1) - (6 * 1)) * 0.5;
			Vector3 pos3(Character->Movement->GetPos_x() + tsize2, Character->Movement->GetPos_y() + tsize2, 0);

			if (projectile->type == Projectile::BossBullet)
			{
				if (Collision::SphericalCollision(pos1, tsize, pos3, tsize3))
				{
					ProjectileCollisionResponse(projectile, it2);
					break;
				}
			}
			else
			{
				if (Collision::SphericalCollision(pos1, tsize, pos2, tsize2))
				{
					ProjectileCollisionResponse(projectile, it2);
					break;
				}
			}
			

        }
		//collide with Character
		/*for (std::vector<Monster*>::iterator it2 = Monster_List.begin(); it2 != Monster_List.end(); ++it2)
		{
			Monster* go = (Monster*)*it2;
			if (projectile->type == Projectile::BossBullet)
			{
				int tsize = ((m_cMap->GetTileSize() * projectile->GetScale().x) - (6 * projectile->GetScale().x)) * 0.5;
				Vector3 pos1(projectile->GetPos().x + tsize, projectile->GetPos().y + tsize, 0);
				int tsize2 = ((m_cMap->GetTileSize() * m_cMap->GetTileSize() - (6 * m_cMap->GetTileSize()))) * 0.5;
				Vector3 pos2(Character->Movement->GetPos_x() + tsize2, Character->Movement->GetPos_y() + tsize2, 0);
				if (Collision::SphericalCollision(pos1, tsize, pos2, tsize2))
				{
					ProjectileCollisionResponse(projectile, it2);
					break;
				}
			}
		}*/
		
        //collide with tile
        int m = 0;
        for (int i = 0; i < m_cMap->GetNumOfTiles_Height(); i++)
        {
            for (int k = 0; k < m_cMap->GetNumOfTiles_Width() + 1; k++)
            {
                m = tileOffset_x + k;
                if ((tileOffset_x + k) >= m_cMap->getNumOfTiles_MapWidth())
                    break;
                if (m_cMap->theScreenMap[i][m] != 0 &&
					m_cMap->theScreenMap[i][m] != 9 &&
                    m_cMap->theScreenMap[i][m] != 11 &&
                    m_cMap->theScreenMap[i][m] != 10 &&
                    m_cMap->theScreenMap[i][m] != 12 &&
                    m_cMap->theScreenMap[i][m] != 13 && 
					m_cMap->theScreenMap[i][m] != 14 &&
					m_cMap->theScreenMap[i][m] != 15)
                {
                    int tsize = ((m_cMap->GetTileSize() * projectile->GetScale().x) - (6 * projectile->GetScale().x)) * 0.5;
                    Vector3 pos1(projectile->GetPos().x + tsize, projectile->GetPos().y + tsize, 0);
                    Vector3 pos2(k*m_cMap->GetTileSize() + tsize, 575 - i*m_cMap->GetTileSize() + tsize, 0);
                    if (Collision::SphericalCollision(pos1, tsize, pos2, tsize))
                    {
                        projectile->active = false;
                        return;
                    }
                }

            }
        }

    }
    
}

void SP3::ProjectileCollisionResponse(Projectile* projectile,
    std::vector<Monster*>::iterator monsterlist_iterator)
{
    Monster* go = (Monster*)*monsterlist_iterator;
    switch (projectile->type)
    {
    case Projectile::Bullet:
        Character->Attribute->ActionBar(5);
        {            
            Mesh* lol = new Mesh(*meshList[GEO_NET_ANIM]);
            //spritemanager->NewSpriteAnimation(lol, go->Movement->GetPos(), go->Movement->GetScale(), 7, 7, 0, 64, 1.f, 0, false);
        }
		projectile->active = false;
        go->Attribute->ReceiveDamage(projectile->getdmg());
        break;
    case Projectile::ChargeBullet:
			go->Attribute->ReceiveDamage(projectile->getdmg());
        break;
    case Projectile::Net:
        if (go->Attribute->Capture())
        {
            //CreateParticles(10, go->Movement->GetPos(), 2, 20, ParticleObject_TYPE::NET);
            CreateParticles(20, go->Movement->GetPos(), 0.5, 15, ParticleObject_TYPE::NET);
            Character->IncreaseScore((go->Attribute->GetMonsterMaxHealth() - go->Attribute->GetCurrentHP()) * 3);
            Monster_List.erase(monsterlist_iterator);
            //particle animation here
			Character->Attribute->ActionBar(10);
			projectile->active = false;
			if (go->type == Monster::MINIBOSS)
			{
				MiniBossAlive = false;
				std::cout << "DEAD" << std::endl;
			}

            return;
        }
        go->Attribute->ReceiveDamage(projectile->getdmg());
		projectile->active = false;
        break;
	case Projectile::BossBullet:
		Character->Attribute->SetReceivedDamage(go->Attribute->GetMonsterDamage());
		projectile->active = false;
		break;
    default:
        break;
    }
        
    if (go->Attribute->GetCurrentHP() <= 0)
    {
        Monster_List.erase(monsterlist_iterator);   
		if (go->type == Monster::MINIBOSS)
		{
			MiniBossAlive = false;
			std::cout << "DEAD" << std::endl;
		}

    }
}

void SP3::MonsterUpdate(double dt, MapLoad* map)
{


    for (std::vector<Monster*>::iterator it = Monster_List.begin(); it != Monster_List.end(); ++it)
    {
        Monster* go = (Monster*)*it;
		if (go->type == Monster::MINIBOSS)
		{
			go->update(dt, Vector3(Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), 0), map, true);
		}
		else
		{
			go->update(dt, Vector3(Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), 0), map, false);
		}
        Vector3 dist(go->Movement->GetPos() - Vector3(Character->Movement->GetPos_x(), Character->Movement->GetPos_y(), 0));
		int tsize = ((m_cMap->GetTileSize() * 1 /*character scale*/) - (6 * 1 /*character scale*/)) * 0.5;
		Vector3 pos1(Character->Movement->GetPos_x() + tsize, Character->Movement->GetPos_y() + tsize, 0);
		Vector3 pos2(go->Movement->GetPos_X() + tsize, go->Movement->GetPos_Y() + tsize, 0);

        if ((dist.LengthSquared() / m_cMap->GetTileSize()) < (m_cMap->GetTileSize() * 2))
        {
            if (Collision::SphericalCollision(pos1, tsize, pos2, tsize))
            {
					Character->Attribute->SetReceivedDamage(go->Attribute->GetMonsterDamage());
            } 
        }

		if (go->type == Monster::MINIBOSS && go->active && go->Movement->Monstate == go->Movement->ATTACK)
		{

			BossFiringDebounce += dt;
			if (BossFiringDebounce > 5.f / fireRate)
			{
				BossFiringDebounce = 0;
				//int tsize2 = ((m_cMap->GetTileSize() * go->Movement->GetScale_X()) - (6 * go->Movement->GetScale_X())) * 0.5;
				Character->Movement->BossProjectileUpdate(dt, 1, 20, Projectile::BossBullet, m_cMap, Vector3(go->Movement->GetPos().x + (m_cMap->GetTileSize() * go->Movement->GetScale_X()) * 0.5, go->Movement->GetPos().y + m_cMap->GetTileSize() * 0.5, 0), go->Movement->faceleft());
				std::cout << "SHADOW BALL" << std::endl;
			}
		}

    }


}

//void SP3::SpriteAnimationUpdate(double dt)
//{
//    /*SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_NET_ANIM]);
//    if (sa)
//    {
//    sa->Update(dt);*/
//    //sa->m_anim->animActive = true;
//    
//}

void SP3::RenderParticles()
{
    
}

void SP3::UpdateParticles(double dt)
{
    //if (m_particleCount < MAX_PARTICLE)
    //{
    //    int num_Particles = 1;
    //    for (int i = 0; i < num_Particles; i++)
    //    {
    //        ParticleObject* particle = GetParticle();
    //        particle->type = ParticleObject_TYPE::P_WATER;
    //        particle->scale.Set(1.5f, 1.5f, 1.5f);
    //        particle->vel.Set(Math::RandFloatMinMax(-5, 5), 0, Math::RandFloatMinMax(-5, 5));
    //        //particle->rotationSpeed = Math::RandFloatMinMax(20.0f, 40.0f);
    //        particle->pos.Set(Math::RandFloatMinMax(camera.position.x + 1200.0f, camera.position.x - 1200.0f), Math::RandFloatMinMax(camera.position.y + 500.0f, camera.position.y + 150.f),
    //            Math::RandFloatMinMax(camera.position.z + 1200.0f, camera.position.z - 1200.0f));
    //    }

    //}
    for (std::vector<ParticleObject*>::iterator it = particleList.begin();
        it != particleList.end(); ++it)
    {
        ParticleObject *particle = (ParticleObject *)*it;
        if (particle->active)
        {
            particle->update(dt);
        }
    }
}

ParticleObject* SP3::GetParticle(void)
{
    for (std::vector<ParticleObject*>::iterator it = particleList.begin();
        it != particleList.end(); ++it)
    {
        ParticleObject *particle = (ParticleObject *)*it;
        if (!particle->active)
        {
            particle->active = true;
            m_particleCount++;
            return particle;
        }
    }

    for (unsigned i = 0; i <= 10; ++i)
    {
        ParticleObject *particle =
            new ParticleObject(ParticleObject_TYPE::P_WATER);
        particleList.push_back(particle);
    }
    ParticleObject *particle = particleList.back();
    particle->active = true;
    m_particleCount++;

    return particle;
}

void SP3::CreateParticles(int number, Vector3 position, float lifetime, float vel,ParticleObject_TYPE type)
{
    for (int i = 0; i < (number - 1); i++)
    {
        ParticleObject *particle = GetParticle();
        particle->pos = position;
        particle->lifetime = lifetime;
        particle->type = type;
        particle->vel = Vector3(Math::RandFloatMinMax(-vel, vel), Math::RandFloatMinMax(-vel, vel), 0);
        particle->scale = Vector3(0.1, 0.1, 1);
        particleList.push_back(particle);
    }
}

void SP3::renderbattlestage()
{
    //Battle->RenderObjects(meshList[GEO_AXES]/*top panel*/, meshList[GEO_M_PANEL]/*middle panel*/, meshList[GEO_B_PANEL]/*bottom panel*/,
    //    meshList[GEO_QUAD]/*player */, meshList[GEO_QUAD]/*enemy */, meshList[GEO_TEXT]);

    for (std::vector<Panel *>::iterator it = Battle->Panel_List.begin(); it != Battle->Panel_List.end(); ++it)
    {
        Panel* go = (Panel*)*it;
        switch (go->panel_pos)
        {
        case Panel::PanelPos::Top:
            Render2DMesh(meshList[GEO_T_PANEL], false, go->getscale().x, go->getscale().y, go->getpos().x, go->getpos().y, false, false);
            break;
        case Panel::PanelPos::Middle:
            Render2DMesh(meshList[GEO_M_PANEL], false, go->getscale().x, go->getscale().y, go->getpos().x, go->getpos().y, false, false);
            break;
        case Panel::PanelPos::Bottom:
            Render2DMesh(meshList[GEO_B_PANEL], false, go->getscale().x, go->getscale().y, go->getpos().x, go->getpos().y, false, false);
            break;
        default:
            break;
        }

    }
    //render character
    if (Battle->player->getcharging())
    {
        if (!Battle->player->getisFullyCharged())
            Render2DMesh(meshList[GEO_PLAYER_CHARGING], false,/*player x scale*/ 1, /*player y scale*/ 1, Battle->player->getpos().x, Battle->player->getpos().y, false, false);
        else
            Render2DMesh(meshList[GEO_PLAYER_MAXCHARGE], false,/*player x scale*/ 1, /*player y scale*/ 1, Battle->player->getpos().x, Battle->player->getpos().y, false, false);
    }
    else
        Render2DMesh(meshList[GEO_PLAYER], false,/*player x scale*/ 1, /*player y scale*/ 1, Battle->player->getpos().x, Battle->player->getpos().y, false, false);
    std::ostringstream ss;
    ss.str(string());
    ss.precision(3);
    ss << Battle->player->gethp();

    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 20, Battle->player->getpos().x - 25, Battle->player->getpos().y - 25);
    //render enemy
    Render2DMesh(meshList[GEO_QUAD], false,/*enemy x scale*/ 1, /*enemy y scale*/ 1, Battle->enemy->getpos().x, Battle->enemy->getpos().y, false, false);
    ss.str(string());
    ss.precision(3);
    ss << Battle->enemy->gethp();
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 20, Battle->enemy->getpos().x - 25, Battle->enemy->getpos().y - 25);

}