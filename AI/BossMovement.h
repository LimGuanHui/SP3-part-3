#pragma once

#include "BossMovementExports.h"
#include "Vector3.h"

namespace BOSS_MOVEMENT
{
	class BOSS_MOVEMENT_API BMovement
	{
	public:
		BMovement();
		~BMovement();

		void Init();

		int GetPos_X();
		int GetPos_Y();
		int GetScale_X();
		int GetScale_Y();
		int GetVel_X();
		int GetVel_Y();

		void SetPos_X(int pos_X);
		void SetPos_Y(int pos_Y);
		void SetScale_X(int scale_X);
		void SetScale_Y(int scale_Y);
		void SetVel_X(int vel_X);
		void SetVel_Y(int vel_Y);

	private:
		Vector3 position;
		Vector3 scale;
		Vector3 velocity;
	};

	BOSS_MOVEMENT_API BMovement* N_Boss();
}