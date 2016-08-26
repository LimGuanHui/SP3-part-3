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

	void MMovement::Init()
	{
		theMonsterPosition =  (0, 0, 0);
		theMonsterScale = (1, 1, 1);
		theMonsterVel = (0, 0, 0);
	}

	int MMovement::GetPos_X()
	{
		return theMonsterPosition.x;
	}

	int MMovement::GetPos_Y()
	{
		return theMonsterPosition.y;
	}

	int MMovement::GetScale_X()
	{
		return theMonsterScale.x;
	}

	int MMovement::GetScale_Y()
	{
		return theMonsterScale.y;
	}

	int MMovement::GetVel_X()
	{
		return theMonsterVel.x;
	}

	int MMovement::GetVel_Y()
	{
		return theMonsterVel.y;
	}

	void MMovement::SetPos_X(int pos_X)
	{
		this->theMonsterPosition.x = pos_X;
	}

	void MMovement::SetPos_Y(int pos_Y)
	{
		this->theMonsterPosition.y = pos_Y;
	}

	void MMovement::SetScale_X(int scale_X)
	{
		this->theMonsterScale.x = scale_X;
	}

	void MMovement::SetScale_Y(int scale_Y)
	{
		this->theMonsterScale.y = scale_Y;
	}

	void MMovement::SetVel_X(int vel_X)
	{
		this->theMonsterVel.x = vel_X;
	}

	void MMovement::SetVel_Y(int vel_Y)
	{
		this->theMonsterVel.y = vel_Y;
	}

    void MMovement::update()
    {

    }

	MMovement* N_Monster()
	{
		return new MMovement();
	}
}