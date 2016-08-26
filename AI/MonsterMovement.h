#pragma once

#ifndef MONSTER_MOVEMENT_EXPORTS
#define MONSTER_MOVEMENT_API __declspec(dllexport) 
#else
#define MONSTER_MOVEMENT_API __declspec(dllimport) 
#endif

#include "Vector3.h"

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

        void Init(Vector3 startpos,
            Vector3 theMonsterScale,float patrol, float movespeed);
        void Init(Vector3 startpos,
            Vector3 theMonsterScale, float patrol_left,float patrol_right, float movespeed);

        void update(double dt);

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

        State Monstate;
	private:
        Vector3 startpos;
		Vector3 theMonsterPosition;
		Vector3 theMonsterScale;
		Vector3 theMonsterVel;

        float patrol_left, patrol_right;
        float movespeed;
        bool facingleft;
	};

	MONSTER_MOVEMENT_API MMovement* N_Monster();
}