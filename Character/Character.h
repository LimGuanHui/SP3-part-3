#pragma once

#include "CharacterExports.h"
#include "Movement.h"
#include "Skills.h"
//#include "AI.h"
//using namespace AI;

namespace CHARACTER
{
	class CHARACTER_API CCharacter
	{
	public:
		CCharacter();
		~CCharacter();
		
		ATTRIBUTE::CAttribute *Attribute;
		MOVEMENT::CMovement *Movement;
		SKILLS::CSkills *Skill;
		//AI::ArtInt *AI;


	private:
	

	};

	CHARACTER_API CCharacter* N_Character();
}


