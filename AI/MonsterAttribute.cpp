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

	void MonsterAttribute::SetReceivedDamage(int ReceiveDmg)
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

	MonsterAttribute* test()
	{
		return new MonsterAttribute();
	}

}