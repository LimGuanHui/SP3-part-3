#pragma once

#include "BossSkillsExports.h"
#include "BossAttribute.h"
using namespace BOSS_ATTRIBUTE;
#include <string>

namespace BOSS_SKILL
{
	class BOSS_SKILL_API CBossSkills
	{
		enum SKILLS
		{
			DEADLY_HID_BOMB = 1,
		};

	public:
		CBossSkills();
		~CBossSkills();

		BossAttribute* N_BossAttribute;

		void BossSkill(int Skill);

		void SetBossSkillName(int Name);
		std::string GetBossSkillname();

		void SetBossSkillEffect(int SkillEffect);

		void SetBossSkillInfo(int Skillinfo);
		std::string GetBossSkillInfo();

		void SetSkillGuageC(int Charge);
		int GetSkillGuageC();

		void SetRefillspd(int RefillSpd);
		int GetRefillspd();

	private:
		std::string Name;
		std::string SkillInfo;

		int SkillGuageC;
		int SkillRefill;
	};
}


