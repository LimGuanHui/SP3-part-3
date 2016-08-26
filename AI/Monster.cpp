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

    void Monster::Init(Vector3 startpos,Vector3 scale, float patrol, float detectionradius, float movementspd)
    {
		Movement->SetPos_X(startpos.x);
		Movement->SetPos_Y(startpos.y);
        Movement->Init(startpos, scale,patrol, movementspd);
        //movespeed = movementspd;
    }
    void Monster::Init(Vector3 startpos, Vector3 scale, float left_patrol, float right_patrol, float detectionradius, float movementspd)
    {
        Movement->SetPos_X(startpos.x);
        Movement->SetPos_Y(startpos.y); 
        Movement->Init(startpos, scale, left_patrol, right_patrol,movementspd);
        //movespeed = movementspd;
    }

    void Monster::InitAttrib(int maxhp, int Damage)
    {
        Attribute->Init(maxhp, Damage);
    }


    void Monster::update(double dt)
    {
        Movement->update(dt);
    }

	Monster* N_Monster()
	{
		return new Monster();
	}
}