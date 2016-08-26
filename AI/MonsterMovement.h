#pragma once

#include "MonsterMovementExports.h"
#include "Vector3.h"
#include "Vector2.h"

namespace MONSTER_MOVEMENT
{
	class MONSTER_MOVEMENT_API MMovement
	{
	public:

		enum State
		{
			IDLE,
			ATTACK,
			DIE,
		};

		MMovement();
		~MMovement();

		void Init();
        void update();

		int GetPos_X();
		int GetPos_Y();
		int GetScale_Y();
		int GetScale_X();
		int GetVel_X();
		int GetVel_Y();

		void SetPos_X(int pos_X);
		void SetPos_Y(int pos_Y);
		void SetScale_X(int scale_X);
		void SetScale_Y(int scale_Y);
		void SetVel_X(int vel_X);
		void SetVel_Y(int vel_Y);

	private:
		Vector3 theMonsterPosition;
		Vector3 theMonsterScale;
		Vector3 theMonsterVel;

	};

	MONSTER_MOVEMENT_API MMovement* N_Monster();
}