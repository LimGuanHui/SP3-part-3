#pragma once

#ifndef MONSTER_ATTRIBUTE_EXPORTS
#define MONSTER_ATTRIBUTE_API __declspec(dllexport)
#else
#define MONSTER_ATTRIBUTE_API __declspec(dllimport)
#endif

namespace MONSTER_ATTRIBUTE
{
	class MONSTER_ATTRIBUTE_API MonsterAttribute
	{
	public:
		MonsterAttribute();
		~MonsterAttribute();

        void Init(int MaxhealthPoint, int Damage);

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
