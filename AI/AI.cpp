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
        delete Boss;
        Boss = NULL;
        delete Monster;
        Monster = NULL;
	}

	ArtInt *N_AI()
	{
		return new ArtInt();
	}
}

