#pragma once

#include "MonsterExports.h"
#include "MonsterMovement.h"
#include "MonsterAttribute.h"

namespace MONSTER
{
	class MONSTER_API Monster
	{
	public:
		Monster();
		~Monster();
		MONSTER_ATTRIBUTE::MonsterAttribute* Attribute;
		MONSTER_MOVEMENT::MMovement* Movement;

	};

	MONSTER_API Monster* N_Monster();
}


