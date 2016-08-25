#pragma once

#include "MapLoadExports.h"

namespace MAPLOADER
{
	class MAPLOAD_API MapLoad
	{
	public:
		MapLoad();
		~MapLoad();

		void Init();
	};

	MAPLOAD_API MapLoad* LoadMap();
}


