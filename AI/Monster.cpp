#include "Monster.h"

namespace MONSTER
{
	Monster::Monster()
	{
		Movement = new MONSTER_MOVEMENT::MMovement();
		Attribute = new MONSTER_ATTRIBUTE::MonsterAttribute();
        type = GASTLY;
        
	}
	Monster::~Monster()
	{
	}

    void Monster::Init(Vector3 startpos, float patrol, float detectionradius, float movementspd)
    {
		Movement->SetPos_X(startpos.x);
		Movement->SetPos_Y(startpos.y);
        movespeed = movementspd;
    }
    void Monster::Init(Vector3 startpos, float left_patrol, float right_patrol, float detectionradius, float movementspd)
    {
        Movement->SetPos_X(startpos.x);
        Movement->SetPos_Y(startpos.y);
        movespeed = movementspd;
    }

	Monster* N_Monster()
	{
		return new Monster();
	}
}