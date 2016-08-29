#include "Movement.h"
#include <Windows.h>
#include <iostream>


namespace MOVEMENT
{
	CMovement::CMovement(void)
		: hero_inMidAir_Up(false)
		, hero_inMidAir_Down(false)
		, jumpspeed(0)
		, heroAnimationCounter(0)
		, heroAnimationInvert(false)
		, mapOffset_x(0)
		, mapOffset_y(0)
		, mapFineOffset_x(0)
		, mapFineOffset_y(0)
		, facingRight(true)
		, TransitLevel(false)
		, TransitLevel2(false)
	{

	}

	CMovement::~CMovement(void)
	{
	}

	// Initialise this class instance
	void CMovement::Init(void)
	{
		theHeroPosition.x = 0;
		theHeroPosition.y = 0;
	}

	// Returns true if the player is on ground
	bool CMovement::isOnGround(void)
	{
		if (hero_inMidAir_Up == false && hero_inMidAir_Down == false)
			return true;

		return false;
	}

	// Returns true if the player is jumping upwards
	bool CMovement::isJumpUpwards(void)
	{
		if (hero_inMidAir_Up == true && hero_inMidAir_Down == false)
			return true;

		return false;
	}

	// Returns true if the player is on freefall
	bool CMovement::isFreeFall(void)
	{
		if (hero_inMidAir_Up == false && hero_inMidAir_Down == true)
			return true;

		return false;
	}

	// Set the player's status to free fall mode
	void CMovement::SetOnFreeFall(bool isOnFreeFall)
	{
		if (isOnFreeFall == true)
		{
			hero_inMidAir_Up = false;
			hero_inMidAir_Down = true;
			jumpspeed = 0;
		}
	}

	// Set the player to jumping upwards
	void CMovement::SetToJumpUpwards(bool isOnJumpUpwards)
	{
		if (isOnJumpUpwards == true && isOnGround())
		{
			hero_inMidAir_Up = true;
			hero_inMidAir_Down = false;
			jumpspeed = 10;
		}
	}

	// Set position x of the player
	void CMovement::SetPos_x(int pos_x)
	{
		theHeroPosition.x = pos_x;
	}

	// Set position y of the player
	void CMovement::SetPos_y(int pos_y)
	{
		theHeroPosition.y = pos_y;
	}

	// Set Jumpspeed of the player
	void CMovement::SetJumpspeed(int jumpspeed)
	{
		this->jumpspeed = jumpspeed;
	}

	// Stop the player's movement
	void CMovement::SetToStop(void)
	{
		hero_inMidAir_Up = false;
		hero_inMidAir_Down = false;
		jumpspeed = 0;
	}

	/********************************************************************************
	Hero Move Up Down
	********************************************************************************/
	void CMovement::MoveUpDown(const bool mode, const float timeDiff)
	{
		if (mode)
		{
			theHeroPosition.y = theHeroPosition.y + (int)(5.0f * timeDiff);
		}
		else
		{
			theHeroPosition.y = theHeroPosition.y - (int)(5.0f * timeDiff);
		}
	}

	/********************************************************************************
	Hero Move Left Right
	********************************************************************************/
	void CMovement::MoveLeftRight(const bool mode, const float timeDiff)
	{
		if (mode)//left
		{
			facingRight = false;
			theHeroPosition.x = theHeroPosition.x - (int)(5.0f * timeDiff);
			heroAnimationInvert = true;
			heroAnimationCounter--;
			if (heroAnimationCounter < 1)
				heroAnimationCounter = 11;
		}
		else
		{
			facingRight = true;
			theHeroPosition.x = theHeroPosition.x + (int)(5.0f * timeDiff);
			heroAnimationInvert = false;
			heroAnimationCounter++;
			if (heroAnimationCounter>11)
				heroAnimationCounter = 1;
		}
	}


	// Get position x of the player
	int CMovement::GetPos_x(void)
	{
		return theHeroPosition.x;
	}

	// Get position y of the player
	int CMovement::GetPos_y(void)
	{
		return theHeroPosition.y;
	}

	// Get Jumpspeed of the player
	int CMovement::GetJumpspeed(void)
	{
		return jumpspeed;
	}

	// Get mapOffset_x
	int CMovement::GetMapOffset_x(void)
	{
		return mapOffset_x;
	}
	// Get mapOffset_y
	int CMovement::GetMapOffset_y(void)
	{
		return mapOffset_y;
	}

	// Get mapFineOffset_x
	int CMovement::GetMapFineOffset_x(void)
	{
		return mapFineOffset_x;
	}
	// Get mapFineOffset_y
	int CMovement::GetMapFineOffset_y(void)
	{
		return mapFineOffset_y;
	}

	// Update Jump Upwards
	void CMovement::UpdateJumpUpwards()
	{
		theHeroPosition.y -= jumpspeed;
		jumpspeed -= 1;
		if (jumpspeed == 0)
		{
			hero_inMidAir_Up = false;
			hero_inMidAir_Down = true;
		}
	}

	// Update FreeFall
	void CMovement::UpdateFreeFall()
	{
		theHeroPosition.y += jumpspeed;
		jumpspeed += 1;
	}

	// Set Animation Invert status of the player
	void CMovement::SetAnimationInvert(bool heroAnimationInvert)
	{
		this->heroAnimationInvert = heroAnimationInvert;
	}
	// Get Animation Invert status of the player
	bool CMovement::GetAnimationInvert(void)
	{
		return heroAnimationInvert;
	}

	// Set Animation Counter of the player
	void CMovement::SetAnimationCounter(int heroAnimationCounter)
	{
		this->heroAnimationCounter = heroAnimationCounter;
	}
	// Get Animation Counter of the player
	int CMovement::GetAnimationCounter(void)
	{
		return heroAnimationCounter;
	}

	// Constrain the position of the Hero to within the border
	void CMovement::ConstrainHero(const int leftBorder, const int rightBorder,
		const int topBorder, const int bottomBorder)
	{
		if (theHeroPosition.x < leftBorder)
		{
			theHeroPosition.x = leftBorder;
		}
		else if (theHeroPosition.x > rightBorder)
		{
			theHeroPosition.x = rightBorder;
		}

		if (theHeroPosition.y < topBorder)
			theHeroPosition.y = topBorder;
		else if (theHeroPosition.y > bottomBorder)
			theHeroPosition.y = bottomBorder;
	}


	/********************************************************************************
	Hero Update
	********************************************************************************/
	void CMovement::HeroUpdate(MapLoad* m_cMap)
	{// Update Hero's info
		int checkPosition_X = (int)((mapOffset_x + theHeroPosition.x) / m_cMap->GetTileSize());
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int)((mapOffset_y + theHeroPosition.y) / m_cMap->GetTileSize()) - 1;
        
        int tileOffset_x = (int)(GetMapOffset_x() / m_cMap->GetTileSize());

		checkPosition_X = Math::Clamp(checkPosition_X, 0, m_cMap->getNumOfTiles_MapWidth());
		checkPosition_Y = Math::Clamp(checkPosition_Y, 0, m_cMap->GetNumOfTiles_Height());

        if ((checkPosition_X + 1) > m_cMap->theScreenMap[0].size())
            return;

		if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 1 ||
			m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 2 ||
			m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 3 ||
			m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 4 ||
			m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 6 ||
			m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 7 ||
			m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 5)
			theHeroPosition.x = (checkPosition_X - tileOffset_x) * m_cMap->GetTileSize() - mapFineOffset_x;
		else if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 1 ||
			m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 2 ||
			m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 3 ||
			m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 4 ||
			m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 6 ||
			m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 7 ||
			m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 5)
		{
			if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 0)
				theHeroPosition.x = (checkPosition_X + 1 - tileOffset_x) * m_cMap->GetTileSize() - mapFineOffset_x;
			else if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 1 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 2 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 3 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 4 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 6 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 7 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 5)
				theHeroPosition.x = (checkPosition_X - 1 - tileOffset_x) * m_cMap->GetTileSize() - mapFineOffset_x;
		}

		// Update Hero's info
		if (isOnGround())
		{
			int xValue = (int)theHeroPosition.x % 25;
			if (m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X] == 1 ||
				m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X] == 2 ||
				m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X] == 3 ||
				m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X] == 4 ||
				m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X] == 5 ||
				m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X] == 6 ||
				m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X] == 7 ||
				(m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X + 1] == 1 ||
				m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X + 1] == 2 ||
				m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X + 1] == 3 ||
				m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X + 1] == 4 ||
				m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X + 1] == 6 ||
				m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X + 1] == 7 ||
				m_cMap->theScreenMap[checkPosition_Y + 1][checkPosition_X + 1] == 5) &&
				xValue != 0)
			{
			}
			else
			{
				jumpspeed = 0;
				hero_inMidAir_Up = false;
				hero_inMidAir_Down = true;
			}
		}
		else if (isJumpUpwards())
		{
			// Check if the hero can move up into mid air...
			checkPosition_X = (int)((mapOffset_x + theHeroPosition.x) / m_cMap->GetTileSize());
			checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int)ceil((float)(theHeroPosition.y + m_cMap->GetTileSize() + jumpspeed) / m_cMap->GetTileSize());
			tileOffset_x = (int)(GetMapOffset_x() / m_cMap->GetTileSize());
			int xValue = (int)theHeroPosition.x % 25;

			checkPosition_X = Math::Clamp(checkPosition_X, 0, m_cMap->getNumOfTiles_MapWidth());
			checkPosition_Y = Math::Clamp(checkPosition_Y, 0, m_cMap->GetNumOfTiles_Height());

			if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 1 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 2 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 3 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 4 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 5 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 6 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 7 ||
				(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 1 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 2 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 3 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 4 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 6 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 7 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 5)
				&& xValue != 0)
			{
				// Since the new position does not allow the hero to move into, then go	back to the old position
				theHeroPosition.y = (ceil(theHeroPosition.y / m_cMap->GetTileSize()) *	m_cMap->GetTileSize());
				hero_inMidAir_Up = false;
				hero_inMidAir_Down = true;
				jumpspeed = 0;
			}
			else
			{
				theHeroPosition.y += jumpspeed;
				jumpspeed -= 1;
				if (jumpspeed == 0)
				{
					hero_inMidAir_Up = false;
					hero_inMidAir_Down = true;
				}
			}
		}
		else if (isFreeFall())
		{
			// Check if the hero is still in mid air...
			checkPosition_X = (int)((mapOffset_x + theHeroPosition.x) / m_cMap->GetTileSize());
			checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int)ceil((float)(theHeroPosition.y - jumpspeed) / m_cMap->GetTileSize());
			int xValue = (int)theHeroPosition.x % 25;

			checkPosition_X = Math::Clamp(checkPosition_X, 0, m_cMap->getNumOfTiles_MapWidth());
			checkPosition_Y = Math::Clamp(checkPosition_Y, 0, m_cMap->GetNumOfTiles_Height());

			if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 1 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 2 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 3 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 4 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 5 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 6 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 7 ||
				(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 1 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 2 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 3 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 4 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 6 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 7 ||
				m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 5)
				&& xValue != 0)
			{
				// Since the new position does not allow the hero to move into, then go	back to the old position
				theHeroPosition.y = ((int)(theHeroPosition.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
				hero_inMidAir_Down = false;
				jumpspeed = 0;
			}
			else
			{
				theHeroPosition.y -= jumpspeed;
				jumpspeed += 1;
				if (jumpspeed > 15)
				{
					jumpspeed = 15;
				}
			}
		}

		if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 10)
		{
			TransitLevel = true;
		}
		if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 11)
		{
			TransitLevel2 = true;
		}


		ConstrainHero(25, 765, 25, 600);
		// Calculate the fine offset
		mapFineOffset_x = mapOffset_x % m_cMap->GetTileSize();

	}

    Projectile* CMovement::FetchProjectile(MapLoad* m_cMap)
	{
		for (std::vector<PROJECTILE::Projectile *>::iterator it = m_projectileList.begin(); it != m_projectileList.end(); ++it)
		{
			PROJECTILE::Projectile *projectile = (PROJECTILE::Projectile *)*it;
			std::vector<PROJECTILE::Projectile *>::iterator it2 = it;
			if (!projectile->active)
			{
				projectile->Init();
				projectile->active = true;

				return projectile;
			}
		}
		for (unsigned i = 0; i < 10; ++i)
		{
            PROJECTILE::Projectile *projectile = new PROJECTILE::Projectile(m_cMap);
			m_projectileList.push_back(projectile);
		}
		PROJECTILE::Projectile *projectile = m_projectileList.back();
		projectile->active = true;
		return projectile;
	}

    void CMovement::ProjectileUpdate(const float timeDiff, double dt, int scale, PROJECTILE::Projectile::ProjType type, MapLoad* m_cMap)
	{

		if (heroAnimationInvert == false)
		{
            Projectile = FetchProjectile(m_cMap);
			Projectile->SetPos(Vector3 (theHeroPosition.x, theHeroPosition.y, 10));
			Projectile->SetVel(Vector3(500, 0, 0));
			Projectile->SetScale(Vector3(scale, scale, 1));
			Projectile->Left = heroAnimationInvert;
			Projectile->type = type;
		}
		else if (heroAnimationInvert == true)
		{
            Projectile = FetchProjectile(m_cMap);
			Projectile->SetPos(Vector3(theHeroPosition.x, theHeroPosition.y, 10));
			Projectile->SetVel(Vector3(-500, 0, 0));
			Projectile->SetScale(Vector3(scale, scale, 1));
			Projectile->Left = heroAnimationInvert;
			Projectile->type = type;
		}
	}
}