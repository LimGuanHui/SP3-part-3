#include "BossAttribute.h"

namespace BOSS_ATTRIBUTE
{
	BossAttribute::BossAttribute()
		: MaxHealthPoint(100)
		, Damage(0)
		, ReceiveDmg(0)
		, CurrentHealth(MaxHealthPoint)
	{
	}

	BossAttribute::~BossAttribute()
	{
	}

	void BossAttribute::SetBossMaxHealth(int MaxHealthPoint)
	{
		this->MaxHealthPoint = MaxHealthPoint;
	}

	int BossAttribute::GetBossMaxHealth()
	{
		return MaxHealthPoint;
	}

	void BossAttribute::SetBossDamage(int Damage)
	{
		this->Damage = Damage;
	}

	int BossAttribute::GetBossDamage()
	{
		return Damage;
	}

	void BossAttribute::SetReceivedDamage(int ReceiveDmg)
	{
		this->CurrentHealth -= ReceiveDmg;
	}

	int BossAttribute::GetReceivedDamage()
	{
		return ReceiveDmg;
	}

	int BossAttribute::GetCurrentHP()
	{
		return CurrentHealth;
	}

}

