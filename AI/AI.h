#pragma once

#include "AIExports.h"
#include "Boss.h"
#include "Monster.h"

namespace AI
{
	class AI_API ArtInt
	{
	public:
		ArtInt();
		~ArtInt();

		BOSS::Boss *Boss;
		MONSTER::Monster *Monster;
		
	private:
		
	};

	AI_API ArtInt* N_AI();
}


