#include "BossMovement.h"

namespace BOSS_MOVEMENT
{
	BMovement::BMovement()
		: position(0, 0, 0)
		, velocity(0, 0, 0)
		, scale(0, 0, 0)
	{
	}

	BMovement::~BMovement()
	{
	}

	void BMovement::Init()
	{
	}

	int BMovement::GetPos_X()
	{
		return position.x;
	}

	int BMovement::GetPos_Y()
	{
		return position.y;
	}

	int BMovement::GetScale_X()
	{
		return scale.x;
	}

	int BMovement::GetScale_Y()
	{
		return scale.y;
	}

	int BMovement::GetVel_X()
	{
		return velocity.x;
	}

	int BMovement::GetVel_Y()
	{
		return velocity.y;
	}

	void BMovement::SetPos_X(int pos_X)
	{
		this->position.x = pos_X;
	}

	void BMovement::SetPos_Y(int pos_Y)
	{
		this->position.y = pos_Y;
	}

	void BMovement::SetScale_X(int scale_X)
	{
		this->scale.x = scale_X;
	}

	void BMovement::SetScale_Y(int scale_Y)
	{
		this->scale.y = scale_Y;
	}

	void BMovement::SetVel_X(int vel_X)
	{
		this->velocity.x = vel_X;
	}

	void BMovement::SetVel_Y(int vel_Y)
	{
		this->velocity.y = vel_Y;
	}

	BMovement* N_Boss()
	{
		return new BMovement();
	}
}