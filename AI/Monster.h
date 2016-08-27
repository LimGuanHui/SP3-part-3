#pragma once

#include "MonsterExports.h"
#include "MonsterMovement.h"
#include "MonsterAttribute.h"
#include <vector>
#include "MapLoad.h"
using namespace MAPLOADER;

namespace MONSTER
{
	class MONSTER_API Monster
	{
	public:
        enum MON_TYPE
        {
            GASTLY,
            MONSTER2,
            MONSTER3,
        };
		Monster();
		~Monster();
        void Init(Vector3 startpos, Vector3 scale, float patrol, float detectionradius, float movementspd, MON_TYPE type, MapLoad* map);
        void Init(Vector3 startpos, Vector3 scale ,float left_patrol, float right_patrol,
            float detectionradius, float movementspd, MON_TYPE type, MapLoad* map);
        void InitAttrib(int maxhp, int Damage);
        
        void update(double dt, Vector3 characterpos);
		
        
        MON_TYPE type;
        MONSTER_ATTRIBUTE::MonsterAttribute* Attribute;
        MONSTER_MOVEMENT::MMovement* Movement;        

        MapLoad* map;
    private:        
        float detectionradius;        
	};
	MONSTER_API Monster* N_Monster();

}