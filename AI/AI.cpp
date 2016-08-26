#include "AI.h"

namespace AI
{
	ArtInt::ArtInt()
	{
		Boss = new BOSS::Boss();
		Monster = new MONSTER::Monster();
	}


	ArtInt::~ArtInt()
	{
	}

	ArtInt *N_AI()
	{
		return new ArtInt();
	}
}

