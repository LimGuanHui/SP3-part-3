#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"

enum ParticleObject_TYPE
{
	P_WATER,
	P_TOTAL,
};

class ParticleObject
{
public:
	ParticleObject(ParticleObject_TYPE = P_WATER);
	~ParticleObject(void);

	
	ParticleObject_TYPE type;//Type of Particle
	Vector3 pos;//Position of Particle
	Vector3 vel;//velocity of Particle
	Vector3 scale;//Scale of Particle
	float rotation;//Rotation of Particle
	float rotationSpeed; //Rotational speed of Particle

	bool active;//Activate Paticle upon use

    int renderOrder;
    float distFromPlayer;
    bool PutInOrder;
};

#endif