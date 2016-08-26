#include "MonsterAttribute.h"

namespace MONSTER_ATTRIBUTE
{
	MonsterAttribute::MonsterAttribute()
		:MaxhealthPoint(100)
		, Damage(0)
		, ReceiveDmg(0)
		, CurrentHealth(MaxhealthPoint)
	{
	}

	MonsterAttribute::~MonsterAttribute()
	{
	}

    void MonsterAttribute::Init(int MaxhealthPoint, int Damage)
    {
        CurrentHealth = this->MaxhealthPoint = MaxhealthPoint;
        this->Damage = Damage;
    }

	void MonsterAttribute::SetMonsterMaxHealth(int MaxHealthPoint)
	{
		this->MaxhealthPoint = MaxHealthPoint;
	}

	int MonsterAttribute::GetMonsterMaxHealth()
	{
		return MaxhealthPoint;
	}

	void MonsterAttribute::SetMonsterDamage(int Damage)
	{
		this->Damage = Damage;
	}

	int MonsterAttribute::GetMonsterDamage()
	{
		return Damage;
	}

	void MonsterAttribute::ReceiveDamage(int ReceiveDmg)
	{
		this->CurrentHealth -= ReceiveDmg;
	}

	int MonsterAttribute::GetReceivedDamage()
	{
		return ReceiveDmg;
	}

	int MonsterAttribute::GetCurrentHP()
	{
		return CurrentHealth;
	}
}