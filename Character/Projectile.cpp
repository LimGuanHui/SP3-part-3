#include "Projectile.h"

namespace PROJECTILE
{
    Projectile::Projectile(MapLoad* map)
		:scale(1,1,1)
		, active(false)
		, Left(false)
		, type(Bullet)
        , map(map)
	{
	}


	Projectile::~Projectile()
	{
	}

	void Projectile::Init()
	{
		scale = Vector3(1,1,1);
		Left = false;
		active = false;
	}

    void Projectile::Update(double dt)
    {
        pos += vel * dt;
        
    }

	Vector3 Projectile::GetPos()
	{
		return pos;
	}

	Vector3 Projectile::GetScale()
	{
		return scale;
	}

	Vector3 Projectile::GetVel()
	{
		return vel;
	}

	void Projectile::SetPos(Vector3 pos)
	{
		this->pos = pos;
	}

	void Projectile::SetScale(Vector3 scale)
	{
		this->scale = scale;
	}

	void Projectile::SetVel(Vector3 vel)
	{
		this->vel = vel;
	}
    float Projectile::getdmg()
    {
        return dmg;
    }
    void Projectile::setdmg(float dmg)
    {
        this->dmg = dmg;
    }

}

