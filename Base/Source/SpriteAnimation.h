#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include "Mesh.h"
#include <vector>
#include "GL\glew.h"
#include "Vertex.h"

struct Animation
{
	Animation(){}

	void Set(int startframe, int endFrame, int repeat, float time, bool active)
	{
		this->startFrame = startframe;
		this->endFrame = endFrame;
		this->repeatCount = repeat;
		this->animTime = time;
		this->animActive = active;
	}

	int startFrame;
	int endFrame;
	int repeatCount;
	float animTime;
	bool ended;
	bool animActive;


};

class SpriteAnimation : public Mesh
{
public:
	SpriteAnimation(const std::string &meshName, int row, int col);
	~SpriteAnimation();
	void Update(double dt);
	virtual void Render();

	int m_row;
	int m_col;

	float m_currentTime;
	int m_currentFrame;
	int m_playCount;

	Animation *m_anim;
};


#endif