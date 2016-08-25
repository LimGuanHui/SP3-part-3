#include "Projectile.h"

namespace PROJECTILE
{
	Projectile::Projectile()
		:scale(1,1,1)
		, active(false)
	{
	}


	Projectile::~Projectile()
	{
	}

	void Projectile::Init()
	{
		scale = Vector3(1,1,1);

		active = false;
	}

	Vector3 Projectile::GetPos()
	{
		return pos;
	}
}

