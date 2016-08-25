#pragma once

#include "AttributeExports.h"
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
		void SetCurrentHP(int HP);
		void SetDmg(int Dmg);
		void SetReceivedDamage(int receiveDmg);
		void SetRecovery(int Recover);

		int GetMaxHP();
		int GetCurrentHP();
		int GetDmg();

		//Boss* N_Boss;

	private:
		int Damage;
		int MaxHealthPoint;
		int CurrentHealthPoint;
	};
}


