#include "MonsterMovement.h"
namespace MONSTER_MOVEMENT
{
	MMovement::MMovement()
		: theMonsterPosition(0, 0, 0)
		, theMonsterScale(1, 1, 1)
		, theMonsterVel(0, 0, 0)
		, AnimationCounter(1)
		, timer(0)
	{
	}

	MMovement::~MMovement()
	{
	}

    void MMovement::Init(Vector3 startpos,
        Vector3 theMonsterScale, float patrol, float movespeed, MapLoad* map)
    {
        theMonsterPosition = this->startpos = startpos;
        this->theMonsterScale = theMonsterScale;
        patrol_left =  patrol / 2;
        patrol_right = patrol_left;
        this->movespeed = movespeed;
        theMonsterVel = Vector3(-movespeed, 0, 0);
        Monstate = IDLE;
        facingleft = true;
        this->map = map;
    }
    void MMovement::Init(Vector3 startpos,
        Vector3 theMonsterScale, float patrol_left, float patrol_right, float movespeed, MapLoad* map)
    {
        this->startpos = startpos;
        this->theMonsterScale = theMonsterScale;
        this->patrol_left = patrol_left;
        this->patrol_right = patrol_right;
        this->movespeed = movespeed;
        Monstate = IDLE;
        facingleft = true;
        this->map = map;
    }


    float MMovement::GetPos_X()
	{
		return theMonsterPosition.x;
	}

    float MMovement::GetPos_Y()
	{
		return theMonsterPosition.y;
	}

    Vector3 MMovement::GetPos()
    {
        return theMonsterPosition;
    }

    float MMovement::GetScale_X()
	{
		return theMonsterScale.x;
	}

    float MMovement::GetScale_Y()
	{
		return theMonsterScale.y;
	}

    Vector3 MMovement::GetScale()
    {
        return theMonsterScale;
    }

    float MMovement::GetVel_X()
	{
		return theMonsterVel.x;
	}

    float MMovement::GetVel_Y()
	{
		return theMonsterVel.y;
	}

    void MMovement::SetPos_X(float pos_X)
	{
		this->theMonsterPosition.x = pos_X;
	}

    void MMovement::SetPos_Y(float pos_Y)
	{
		this->theMonsterPosition.y = pos_Y;
	}

    void MMovement::SetPos(Vector3 pos)
    {
        this->theMonsterPosition = pos;
    }

    void MMovement::SetScale_X(float scale_X)
	{
		this->theMonsterScale.x = scale_X;
	}

    void MMovement::SetScale_Y(float scale_Y)
	{
		this->theMonsterScale.y = scale_Y;
	}

    void MMovement::SetVel_X(float vel_X)
	{
		this->theMonsterVel.x = vel_X;
	}

    void MMovement::SetVel_Y(float vel_Y)
	{
		this->theMonsterVel.y = vel_Y;
	}

	void MMovement::SetAnimationCounter(int AC)
	{
		this->AnimationCounter = AC;
	}

	int MMovement::GetAnimationCounter()
	{
		return AnimationCounter;
	}

    void MMovement::update(double dt, Vector3 characterpos, MapLoad* map, bool isBoss)
    {
        theMonsterPosition += theMonsterVel * dt;
		timer += dt;
        switch (Monstate)
        {
        case MONSTER_MOVEMENT::MMovement::IDLE:
        {
            int checkPosition_X = (int)((theMonsterPosition.x + 0.5f) / map->GetTileSize());
            int checkPosition_Y = map->GetNumOfTiles_Height() - (int)((theMonsterPosition.y) / map->GetTileSize()) - 1;

            //int tileOffset_x = (int)(GetMapOffset_x() / m_cMap->GetTileSize());

            checkPosition_X = Math::Clamp(checkPosition_X, 0, map->getNumOfTiles_MapWidth());
            checkPosition_Y = Math::Clamp(checkPosition_Y, 0, map->GetNumOfTiles_Height());
            /*if (map->theScreenMap[checkPosition_Y - 1][checkPosition_X + 1] == 0)
            {

            }*/
            //right
            float test = (theMonsterPosition.x - startpos.x) * (theMonsterPosition.x - startpos.x);

            if (!facingleft)
            {
                if (map->theScreenMap[checkPosition_Y][checkPosition_X + 1] != 0 &&
					map->theScreenMap[checkPosition_Y][checkPosition_X + 1] != 10 &&
					map->theScreenMap[checkPosition_Y][checkPosition_X + 1] != 11 &&
                    map->theScreenMap[checkPosition_Y][checkPosition_X + 1] != 12 &&
					map->theScreenMap[checkPosition_Y][checkPosition_X + 1] != 13 &&
					map->theScreenMap[checkPosition_Y][checkPosition_X + 1] != 14)
                {
                    theMonsterVel.x = -theMonsterVel.x;
					
                    facingleft = true;
                }

                else if (map->theScreenMap[checkPosition_Y+1][checkPosition_X + 1] == 0)
                {
                    theMonsterVel.x = -theMonsterVel.x;
                    facingleft = true;
                }

                else if (theMonsterPosition.x > (startpos.x + patrol_right))//right to left
                {
                    theMonsterVel.x = -theMonsterVel.x;
                    facingleft = true;
                }
			
            }            
            //left
            if (facingleft)
            {
                if (map->theScreenMap[checkPosition_Y][checkPosition_X ] != 0 &&
					map->theScreenMap[checkPosition_Y][checkPosition_X ] != 10 &&
					map->theScreenMap[checkPosition_Y][checkPosition_X ] != 11 &&
                    map->theScreenMap[checkPosition_Y][checkPosition_X ] != 12 &&
					map->theScreenMap[checkPosition_Y][checkPosition_X ] != 13 &&
					map->theScreenMap[checkPosition_Y][checkPosition_X ] != 14)
                {
                    theMonsterVel.x = -theMonsterVel.x;
                    facingleft = false;
                }
                else if (map->theScreenMap[checkPosition_Y + 1][checkPosition_X] == 0)
                {
                    theMonsterVel.x = -theMonsterVel.x;
                    facingleft = false;
                }

                else if (theMonsterPosition.x <  (startpos.x - patrol_left))//left to right
                {
                    theMonsterVel.x = -theMonsterVel.x;
                    facingleft = false;
                }

            }

			if (Float)
			{
				float timer = 0;
				timer += 2;
				if (theMonsterPosition.y <= startpos.y)
				{
					theMonsterPosition.y = startpos.y;
					theMonsterVel.y += timer;
				}
				if (theMonsterPosition.y > startpos.y + 2)
				{
					theMonsterVel.y -= timer;
				}
			}

			if (!facingleft)
			{
				if (timer > 0.05)
				{
					AnimationCounter++;
					timer = 0;
				}
				if (AnimationCounter > 7)
					AnimationCounter = 1;
			}

			else if (facingleft)
			{
				if (timer > 0.05)
				{
					AnimationCounter--;
					timer = 0;
				}
				if (AnimationCounter < 1)
					AnimationCounter = 7;
			}

			if (isBoss)
			{
				Vector3 dist(theMonsterPosition - Vector3(characterpos.x, characterpos.y, 0));
				if (dist.LengthSquared() / map->GetTileSize() < (map->GetTileSize() * 60))
				{
					Monstate = ATTACK;
					facingleft = true;
				}
			}
			
        }
            break;
        case MONSTER_MOVEMENT::MMovement::ATTACK:
        {
			if (isBoss)
			{
				int checkPosition_X = (int)((theMonsterPosition.x + 0.5f) / map->GetTileSize());
				int checkPosition_Y = map->GetNumOfTiles_Height() - (int)((theMonsterPosition.y) / map->GetTileSize()) - 1;

				//int tileOffset_x = (int)(GetMapOffset_x() / m_cMap->GetTileSize());

				checkPosition_X = Math::Clamp(checkPosition_X, 0, map->getNumOfTiles_MapWidth());
				checkPosition_Y = Math::Clamp(checkPosition_Y, 0, map->GetNumOfTiles_Height());
				/*if (map->theScreenMap[checkPosition_Y - 1][checkPosition_X + 1] == 0)
				{

				}*/
				//right
				float test = (theMonsterPosition.x - startpos.x) * (theMonsterPosition.x - startpos.x);

				if (facingleft)
				{
					theMonsterVel = Vector3(0, 0, 0);
					SetAnimationCounter(4);
				}
			}

			Vector3 dist(theMonsterPosition - Vector3(characterpos.x, characterpos.y, 0));
			if (dist.LengthSquared() / map->GetTileSize() > (map->GetTileSize() * 60))
			{
				Monstate = IDLE;
				theMonsterVel = Vector3(-movespeed, 0, 0);
			}
			
        }
            break;
        case MONSTER_MOVEMENT::MMovement::DIE:
        {
            
        }
            break;
        default:
            break;
        }

		
		
    }

    bool MMovement::faceleft()
    {
        return facingleft;
    }

	MMovement* N_Monster()
	{
		return new MMovement();
	}

    
}