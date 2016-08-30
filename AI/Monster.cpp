#include "Monster.h"

namespace MONSTER
{
	Monster::Monster() : active(false)
	{
		Movement = new MONSTER_MOVEMENT::MMovement();
		Attribute = new MONSTER_ATTRIBUTE::MonsterAttribute();
        type = GASTLY;
        
	}
	Monster::~Monster()
	{
	}

    void Monster::Init(Vector3 startpos, Vector3 scale, float patrol, 
        float detectionradius, float movementspd, MON_TYPE type, MapLoad* map, bool Floats, bool active)
    {
		Movement->SetPos_X(startpos.x);
		Movement->SetPos_Y(startpos.y);
        Movement->Init(startpos, scale,patrol, movementspd,map);
        this->type = type;
        this->map = map;
		Movement->Float = Floats;
		this->active = active;
		this->detectionradius = detectionradius;
    }
    void Monster::Init(Vector3 startpos, Vector3 scale, float left_patrol, float right_patrol, 
		float detectionradius, float movementspd, MON_TYPE type, MapLoad* map, bool Floats, bool active)
    {
        Movement->SetPos_X(startpos.x);
        Movement->SetPos_Y(startpos.y); 
        Movement->Init(startpos, scale, left_patrol, right_patrol, movementspd, map);
        this->type = type;
        this->map = map;
		Movement->Float = Floats;
		this->active = active;
		this->detectionradius = detectionradius;
    }

    void Monster::InitAttrib(int maxhp, int Damage, int Catch_PercentHP, int catchrate)
    {
        Attribute->Init(maxhp, Damage, Catch_PercentHP, catchrate);
    }


	void Monster::update(double dt, Vector3 characterpos, MapLoad* map, bool isBoss)
    {
        Movement->update(dt,characterpos,map, isBoss);
		Attribute->update(dt);
    }

	Monster* N_Monster()
	{
		return new Monster();
	}
}