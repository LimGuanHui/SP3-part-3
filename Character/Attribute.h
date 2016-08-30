#pragma once


#ifndef ATTRIBUTE_EXPORTS
#define ATTRIBUTE_API __declspec(dllexport) 
#else
#define ATTRIBUTE_API __declspec(dllimport) 
#endif
//#include "Boss.h"
//using namespace BOSS;
//#include "Monster.h"
//using namespace MONSTER;

namespace ATTRIBUTE
{
	class ATTRIBUTE_API CAttribute
	{
	public:
		CAttribute();
		~CAttribute();

		void SetMaxHP(int HP);
		void SetDmg(int Dmg);
		void SetReceivedDamage(int receiveDmg);
		void SetRecovery(int Recover);
		void SetCurrentHP(int HP);

		int GetMaxHP();
		int GetCurrentHP();
		int GetDmg();

		void ActionBar(float value);
		float GetActionBar();
		//Boss* N_Boss;

	private:
		int Damage;
		int MaxHealthPoint;
		int CurrentHealthPoint;
		float ActionMeter;
	};
}


