#pragma once

#ifndef RPOJECTILE_EXPORTS
#define PROJECTILE_API __declspec(dllexport) 
#else
#define PROJECTILE_API __declspec(dllimport) 
#endif


#include "Vector3.h"

namespace PROJECTILE
{
	class PROJECTILE_API Projectile
	{
	public:
		Projectile();
		~Projectile();

		void SetPos(Vector3 pos);
		Vector3 GetPos();

		Vector3 pos;
		Vector3 vel;
		Vector3 scale;

		bool active;

		void Init();

	};

}


