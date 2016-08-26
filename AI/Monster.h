#pragma once

#include "MonsterExports.h"
#include "MonsterMovement.h"

namespace MONSTER
{
	class MONSTER_API Monster
	{
	public:
		Monster();
		~Monster();
		MONSTER_MOVEMENT::MMovement* Movement;

	};

	MONSTER_API Monster* N_Monster();
}


