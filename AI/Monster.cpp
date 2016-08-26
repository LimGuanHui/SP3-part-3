#include "Monster.h"

namespace MONSTER
{
	Monster::Monster()
	{
		Movement = new MONSTER_MOVEMENT::MMovement();
		Attribute = new MONSTER_ATTRIBUTE::MonsterAttribute();
	}


	Monster::~Monster()
	{
	}

	Monster* N_Monster()
	{
		return new Monster();
	}
}

