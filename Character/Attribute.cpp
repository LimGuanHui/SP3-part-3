#include "Attribute.h"

namespace ATTRIBUTE
{
	CAttribute::CAttribute() 
		: Damage(10)
		, MaxHealthPoint(100)
		, CurrentHealthPoint(MaxHealthPoint)
		, ActionMeter(100)
		, invul(false)
		, invulTimer(0)
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

	void CAttribute::SetCurrentHP(int HP)
	{
		this->CurrentHealthPoint = HP;
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
		if (!invul)
		{
			this->CurrentHealthPoint -= receiveDmg;
			invulTimer = 0;
			invul = true;
		}
		
	}

	int CAttribute::GetCurrentHP()
	{
		return CurrentHealthPoint;
	}

	int CAttribute::GetDmg()
	{
		return Damage;
	}

	void CAttribute::ActionBar(float Value)
	{
		this->ActionMeter += Value;
		if (ActionMeter > 100)
		{
			ActionMeter = 100;
		}
	}

	float CAttribute::GetActionBar()
	{
		return ActionMeter;
	}
	
	void CAttribute::update(double dt)
	{
		invulTimer += (float)dt;
		
		if (invulTimer > 0.2)
		{
			invul = false;
		}
	}
}

