#pragma once

#include "BossAttributeExports.h"

namespace BOSS_ATTRIBUTE
{
	class BOSS_ATTRIBUTE_API BossAttribute
	{
	public:
		BossAttribute();
		~BossAttribute();

		void SetBossMaxHealth(int HealthPoint);
		int GetBossMaxHealth();

		void SetBossDamage(int Damage);
		int GetBossDamage();

		void SetReceivedDamage(int ReceiveDmg);
		int GetReceivedDamage();

		//int Get

		int GetCurrentHP();

	private:
		int MaxHealthPoint;
		int Damage;
		int ReceiveDmg;
		int CurrentHealth;
	};

	//BOSS_ATTRIBUTE_API BossAttribute* BossAttribute();

}

