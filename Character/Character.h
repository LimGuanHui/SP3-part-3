#pragma once


#ifndef CHARACTER_EXPORTS
#define CHARACTER_API __declspec(dllexport) 
#else
#define CHARACTER_API __declspec(dllimport) 
#endif

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

		void Restart();
		bool active;


	private:
	

	};

	CHARACTER_API CCharacter* N_Character();
}


