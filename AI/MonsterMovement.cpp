#include "MonsterMovement.h"

namespace MONSTER_MOVEMENT
{
	MMovement::MMovement()
	{
	}

	MMovement::~MMovement()
	{
	}

	void MMovement::Init()
	{
		position = Vector3(3, 0, 0);
	}

	int MMovement::GetPos_X()
	{
		return position.x;
	}

	int MMovement::GetPos_Y()
	{
		return position.y;
	}

	void MMovement::SetPos_X(int pos_X)
	{
		this->position.x = pos_X;
	}

	void MMovement::SetPos_Y(int pos_Y)
	{
		this->position.y = pos_Y;
	}

	MMovement* N_Monster()
	{
		return new MMovement();
	}
}