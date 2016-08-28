#include "MonsterAttribute.h"
#include "MyMath.h"
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

    void MonsterAttribute::Init(int MaxhealthPoint, int Damage, int Catch_PercentHP, int catchrate)
    {
        CurrentHealth = this->MaxhealthPoint = MaxhealthPoint;
        this->Damage = Damage;
        this->Catch_PercentHP = Catch_PercentHP;
        capturehp = (Catch_PercentHP / 100) * MaxhealthPoint;
        this->catchrate = catchrate;
        
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

    bool MonsterAttribute::Capture()
    {
        float currhpPC = 0;
        currhpPC = (float)((CurrentHealth * (pow( MaxhealthPoint,-1))) * 100);
        int capturechance = Math::RandIntMinMax(0, 100 - currhpPC );
        if (Math::RandIntMinMax(0, 100) <= 100 - (currhpPC * catchrate))
            return true;
        return false;
    }
}