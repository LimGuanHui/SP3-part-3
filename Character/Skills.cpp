#include "Skills.h"

namespace SKILLS
{
	CSkills::CSkills(){

		Attribute = new ATTRIBUTE::CAttribute();
	}

	CSkills::~CSkills(){}

	void CSkills::Skill(int Skill)
	{
		switch (Skill)
		{
		case HEALING:
			SetSkillName(Skill);
			SetSkillEffect(Skill);
			SetSkillInfo(Skill);
			break;
		}
	}

	void CSkills::SetSkillName(int Name)
	{
		switch (Name)
		{
		case HEALING:
			this->Name = "HEAL";
			break;
		}
	}

	std::string CSkills::GetSkillName()
	{
		return Name;
	}

	void CSkills::SetSkillEffect(int SkillEffect)
	{
		switch (SkillEffect)
		{
		case HEALING:
			Attribute->SetRecovery(30);
			break;
		}
	}

	void CSkills::SetSkillInfo(int Info)
	{
		switch (Info)
		{
		case HEALING:
			this->SkillInfo = "Heal Character by 30HP";
			break;
		}
	}

	void CSkills::SetSkillGuageC(int SGCap)
	{
		this->SkillGuageC = SGCap;
	}

	void CSkills::SetRefillspd(int SGSpeed)
	{
		this->SkillRefill = SGSpeed;
	}

	int CSkills::GetSkillGuage()
	{
		return SkillGuageC;
	}

	int CSkills::GetRefillspd()
	{
		return SkillRefill;
	}
}

