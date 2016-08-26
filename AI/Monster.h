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
        void Init(Vector3 startpos, float patrol, float detectionradius);
        void Init(Vector3 startpos, float left_patrol, float right_patrol, float detectionradius);
		MONSTER_ATTRIBUTE::MonsterAttribute* Attribute;
		MONSTER_MOVEMENT::MMovement* Movement;
        enum MON_TYPE
        {
            GASTLY,
        };
        MON_TYPE type;
        enum STATE
        {
            Spawn,
            Idle,
            Attack,
            Die,
        };

        Vector3 getcurrpos();
    private:
        Vector3 initialpos;
        Vector3 currpos;
        float patrol_left, patrol_right;
        float detectionradius;
	};
	MONSTER_API Monster* N_Monster();

}


