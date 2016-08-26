#pragma once

#ifndef MONSTER_EXPORTS
#define MONSTER_API __declspec(dllexport) 
#else
#define MONSTER_API __declspec(dllimport) 
#endif
#include "MonsterMovement.h"
#include "MonsterAttribute.h"
#include "Vector3.h"

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

        //Vector3 getcurrpos();
    private:
       // Vector3 initialpos;
       // Vector3 currpos;
        float patrol_left, patrol_right;
        float detectionradius;
	};
	MONSTER_API Monster* N_Monster();

}


