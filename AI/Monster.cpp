#include "Monster.h"

namespace MONSTER
{
	Monster::Monster()
	{
		Movement = new MONSTER_MOVEMENT::MMovement();
	}


	Monster::~Monster()
	{
	}

	Monster* N_Monster()
	{
		return new Monster();
	}
}

