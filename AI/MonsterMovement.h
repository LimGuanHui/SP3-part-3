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

		float GetPos_X();
        float GetPos_Y();
        Vector3 GetPos();
        float GetScale_Y();
        float GetScale_X();
        float GetVel_X();
        float GetVel_Y();

		void SetPos_X(float pos_X);
        void SetPos_Y(float pos_Y);
        void SetPos(Vector3 pos);
        void SetScale_X(float scale_X);
        void SetScale_Y(float scale_Y);
        void SetVel_X(float vel_X);
        void SetVel_Y(float vel_Y);

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