#include "BossSkills.h"

namespace BOSS_SKILL
{
	CBossSkills::CBossSkills()
	{
		N_BossAttribute = new BossAttribute();
	}


	CBossSkills::~CBossSkills()
	{
	}

	void CBossSkills::BossSkill(int Skill)
	{
		switch (Skill)
		{
		case DEADLY_HID_BOMB:
			SetBossSkillName(Skill);
			SetBossSkillEffect(Skill);
			SetBossSkillInfo(Skill);
			break;

		}
	}

	void CBossSkills::SetBossSkillName(int Name)
	{
		switch (Name)
		{
		case DEADLY_HID_BOMB:
			this->Name = "Hid Bomb";
			break;
		}
	}
	std::string CBossSkills::GetBossSkillname()
	{
		return Name;
	}

	void CBossSkills::SetBossSkillEffect(int SkillEffect)
	{
		switch (SkillEffect)
		{
		case DEADLY_HID_BOMB:
			N_BossAttribute->SetBossDamage(100);
			break;
		}
	}

	void CBossSkills::SetBossSkillInfo(int Skillinfo)
	{
		switch (Skillinfo)
		{
		case DEADLY_HID_BOMB:
			this->SkillInfo = "Silent Deadly Fart";
			break;
		}
	}
	std::string CBossSkills::GetBossSkillInfo()
	{
		return SkillInfo;
	}

	void CBossSkills::SetSkillGuageC(int Charge)
	{

	}
	int CBossSkills::GetSkillGuageC()
	{
		return SkillGuageC;
	}

	void CBossSkills::SetRefillspd(int RefillSpd)
	{

	}
	int CBossSkills::GetRefillspd()
	{
		return SkillRefill;
	}
}

