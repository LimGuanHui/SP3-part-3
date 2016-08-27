#ifndef BUTTONS_H
#define BUTTONS_H
#include <vector>
#include <Vector3.h>
#include "GameObject.h"
#include "Camera3.h"
#include "MapLoad.h"
using namespace MAPLOADER;

using std::vector;

class Buttons
{
public:
	Buttons();
	~Buttons();

	void Init(vector<GameObject *> *buttonList, MapLoad* m_cMap);
	void Update(Vector3 mousepos, float dt, vector<GameObject *> *buttonList);
	
	bool isClick;
	bool isHover;

	

	GameObject* button;
	GameObject* PlayButton;
	GameObject* MenuButton;
	GameObject* EditButton;
	GameObject* LoadButton;
	GameObject* HighscoreButton;
	GameObject* SaveButton;
	GameObject* ExitButton;
	GameObject* RestartButton;

	Camera camera;
private:

};

#endif