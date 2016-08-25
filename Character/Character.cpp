#include "Character.h"

namespace CHARACTER
{
	CCharacter::CCharacter()
	{
		Movement = new MOVEMENT::CMovement();
		Skill = new SKILLS::CSkills();
		Attribute = Skill->Attribute;
		//AI = new AI::ArtInt();

	}

	CCharacter::~CCharacter()
	{
	}

	CCharacter* N_Character()
	{
		return new CCharacter();
	}
}
