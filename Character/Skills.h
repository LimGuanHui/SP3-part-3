
#pragma once

#include "SkillsExport.h"
#include "Attribute.h"
#include <string>

namespace SKILLS
{
	class SKILLS_API CSkills
	{

		enum SKILLS
		{
			HEALING = 1,
		};


	public:
		CSkills();
		~CSkills();

		void Skill(int Skill);

		void SetSkillName(int Name);
		std::string GetSkillName();

		void SetSkillEffect(int SkillEffect);

		void SetSkillInfo(int Info);
		std::string GetSkillInfo();

		void SetSkillGuageC(int SkillGuageC);
		void SetRefillspd(int SkillCharge);

		int GetSkillGuage();
		int GetRefillspd();

		ATTRIBUTE::CAttribute *Attribute;

	private:

		std::string Name;
		std::string SkillInfo;

		int SkillGuageC;
		int SkillRefill;

		

	};

}

