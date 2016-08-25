#include "Attribute.h"

namespace ATTRIBUTE
{
	CAttribute::CAttribute() 
		: Damage(0)
		, MaxHealthPoint(100)
		, CurrentHealthPoint(MaxHealthPoint)
	{
		//N_Boss = new BOSS::Boss();
	}

	CAttribute::~CAttribute()
	{
	}

	void CAttribute::SetMaxHP(int HP)
	{
		this->MaxHealthPoint = HP;
	}

	void CAttribute::SetDmg(int Dmg)
	{
		this->Damage = Dmg;
	}

	void CAttribute::SetRecovery(int Recover)
	{
		if (CurrentHealthPoint < MaxHealthPoint)
			this->CurrentHealthPoint += Recover;
		else
			CurrentHealthPoint = MaxHealthPoint;
	}

	int CAttribute::GetMaxHP()
	{
		return MaxHealthPoint;
	}

	void CAttribute::SetReceivedDamage(int receiveDmg)
	{
		this->CurrentHealthPoint -= receiveDmg;
	}

	int CAttribute::GetCurrentHP()
	{
		return CurrentHealthPoint;
	}

	int CAttribute::GetDmg()
	{
		return Damage;
	}
}

