#include "Character.h"

namespace CHARACTER
{
	CCharacter::CCharacter()
	{
		Movement = new MOVEMENT::CMovement();
		Skill = new SKILLS::CSkills();
		Attribute = Skill->Attribute;
		//AI = new AI::ArtInt();
        scoreforshow = score = 0;
	}

	CCharacter::~CCharacter()
	{
	}

	void CCharacter::Restart()
	{
		Movement->facingRight = true;
		Movement->SetAnimationCounter(0);
		//Attribute->SetRecovery(101);
	}

	CCharacter* N_Character()
	{
		return new CCharacter();
	}

    void CCharacter::Update(double dt)
    {
        if (scoreforshow < score)
            scoreforshow += 1;
    }
    void CCharacter::IncreaseScore(int score)
    {
        this->score += score;
    }
    int CCharacter::getScore()
    {
        return scoreforshow;
    }

}
