#include "MonsterMovement.h"

namespace MONSTER_MOVEMENT
{
	MMovement::MMovement()
		: theMonsterPosition(0, 0, 0)
		, theMonsterScale(1,1, 1)
		, theMonsterVel(0,0,0)
	{
	}

	MMovement::~MMovement()
	{
	}

    void MMovement::Init(Vector3 startpos,
        Vector3 theMonsterScale, float patrol, float movespeed)
    {
        theMonsterPosition = this->startpos = startpos;
        this->theMonsterScale = theMonsterScale;
        patrol_left =  patrol / 2;
        patrol_right = patrol_left;
        this->movespeed = movespeed;
        theMonsterVel = Vector3(-movespeed, 0, 0);
        Monstate = IDLE;
        facingleft = true;
    }
    void MMovement::Init(Vector3 startpos,
        Vector3 theMonsterScale, float patrol_left, float patrol_right, float movespeed)
    {
        this->startpos = startpos;
        this->theMonsterScale = theMonsterScale;
        this->patrol_left = patrol_left;
        this->patrol_right = patrol_right;
        this->movespeed = movespeed;
        Monstate = IDLE;
        facingleft = true;
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

    void MMovement::update(double dt)
    {
        theMonsterPosition += theMonsterVel * dt;
        switch (Monstate)
        {
        case MONSTER_MOVEMENT::MMovement::IDLE:
        {
            float test = (theMonsterPosition.x - startpos.x) * (theMonsterPosition.x - startpos.x);
            if (theMonsterPosition.x <  (startpos.x - patrol_left) && facingleft)//left to right
            {
                theMonsterVel.x = -theMonsterVel.x;
                facingleft = false;
            }
            else if (theMonsterPosition.x > (startpos.x + patrol_right) && !facingleft)//right to left
            {
                theMonsterVel.x = -theMonsterVel.x;
                facingleft = true;
            }
        }
            break;
        case MONSTER_MOVEMENT::MMovement::ATTACK:
        {
            
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

	MMovement* N_Monster()
	{
		return new MMovement();
	}
}