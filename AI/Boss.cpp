#include "Boss.h"

namespace BOSS
{
	Boss::Boss()
	{
		BossMovement = new BOSS_MOVEMENT::BMovement();
		BossSkill = new BOSS_SKILL::CBossSkills();
		BossAttribute = BossSkill->N_BossAttribute;
	}


	Boss::~Boss()
	{
        delete BossMovement;
        BossMovement = NULL;
        delete BossSkill;
        BossSkill = NULL;
	}

	Boss* N_Boss()
	{
		return new Boss();
	}
}

