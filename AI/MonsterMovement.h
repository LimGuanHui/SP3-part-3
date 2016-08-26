#pragma once

#include "MonsterMovementExports.h"
#include "Vector3.h"

namespace MONSTER_MOVEMENT
{
	class MONSTER_MOVEMENT_API MMovement
	{
	public:
		MMovement();
		~MMovement();

		void Init();

		int GetPos_X();
		int GetPos_Y();

		void SetPos_X(int pos_X);
		void SetPos_Y(int pos_Y);

	private:
		Vector3 position;
	};

	MONSTER_MOVEMENT_API MMovement* N_Monster();
}