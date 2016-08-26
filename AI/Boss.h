#pragma once

#include "BossExports.h"
#include "BossAttribute.h"
#include "BossMovement.h"
#include "BossSkills.h"

namespace BOSS
{
	class BOSS_API Boss
	{
	public:
		Boss();
		~Boss();

		BOSS_ATTRIBUTE::BossAttribute *BossAttribute;
		BOSS_MOVEMENT::BMovement *BossMovement;
		BOSS_SKILL::CBossSkills *BossSkill;
	};

	BOSS_API Boss* N_Boss();
}


