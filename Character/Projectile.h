#ifndef PROJECTILE_H
#define PROJECTILE_H

#ifndef RPOJECTILE_EXPORTS
#define PROJECTILE_API __declspec(dllexport) 
#else
#define PROJECTILE_API __declspec(dllimport) 
#endif


#include "Vector3.h"
#include "Vector2.h"
#include "MapLoad.h"
using namespace MAPLOADER;

namespace PROJECTILE
{
	class PROJECTILE_API Projectile
	{
	public:

		enum ProjType
		{
			Bullet,
			ChargeBullet,
			Net,
			BossBullet,
		};

		Projectile(MapLoad* map);
		~Projectile();

        

		ProjType type;

		Vector3 GetPos();
		Vector3 GetScale();
		Vector3 GetVel();

		void SetPos(Vector3 pos);
		void SetScale(Vector3 scale);
		void SetVel(Vector3 vel);

        float getdmg();
        void setdmg(float dmg);
        
        bool active;
		bool Left;

		void Init();
        void Update(double dt);

		
	private:
        Vector3 pos;
        Vector3 vel;
        Vector3 scale;

        float dmg;
        MapLoad* map;
	};

}

#endif
