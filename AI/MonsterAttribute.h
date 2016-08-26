#pragma once

#include "MonsterAttributeExports.h"

namespace MONSTER_ATTRIBUTE
{
	class MONSTER_ATTRIBUTE_API MonsterAttribute
	{
	public:
		MonsterAttribute();
		~MonsterAttribute();

		void SetMonsterMaxHealth(int HealthPoint);
		int GetMonsterMaxHealth();

		void SetMonsterDamage(int Damage);
		int GetMonsterDamage();

		void SetReceivedDamage(int ReceiveDmg);
		int GetReceivedDamage();

		int GetCurrentHP();

	private:
		int MaxhealthPoint;
		int Damage;
		int ReceiveDmg;
		int CurrentHealth;
	};

}
	MONSTER_ATTRIBUTE_API MONSTER_ATTRIBUTE::MonsterAttribute* test;

