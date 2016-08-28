#ifndef MONSTERATTRIBUTE_H
#define MONSTERATTRIBUTE_H

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

        void Init(int MaxhealthPoint, int Damage, int Catch_PercentHP, int catchrate);

		void SetMonsterMaxHealth(int HealthPoint);
		int GetMonsterMaxHealth();

		void SetMonsterDamage(int Damage);
		int GetMonsterDamage();

		void ReceiveDamage(int ReceiveDmg);
		int GetReceivedDamage();

		int GetCurrentHP();

        bool Capture();

	private:
		int MaxhealthPoint;
		int Damage;
		int ReceiveDmg;
		int CurrentHealth;
        int Catch_PercentHP;
        int capturehp;
        int catchrate;
	};

}

#endif