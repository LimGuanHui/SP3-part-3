#pragma once

#include "MonsterExports.h"
#include "MonsterMovement.h"
#include "MonsterAttribute.h"

namespace MONSTER
{
	class MONSTER_API Monster
	{
	public:
        enum MON_TYPE
        {
            GASTLY,

        };
		Monster();
		~Monster();
        void Init(Vector3 startpos, Vector3 scale, float patrol, float detectionradius, float movementspd, MON_TYPE type);
        void Init(Vector3 startpos, Vector3 scale ,float left_patrol, float right_patrol,
            float detectionradius, float movementspd, MON_TYPE type);
        void InitAttrib(int maxhp, int Damage);
        
        void update(double dt);
		
        
        MON_TYPE type;
        MONSTER_ATTRIBUTE::MonsterAttribute* Attribute;
        MONSTER_MOVEMENT::MMovement* Movement;
    private:        
        float detectionradius;        
	};
	MONSTER_API Monster* N_Monster();
}