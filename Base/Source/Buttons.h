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

	GameObject* button;
	GameObject* PlayButton;
	GameObject* MenuButton;
	GameObject* HighscoreButton;
	GameObject* ExitButton;
	GameObject* RestartButton;
	GameObject* HelpButton;
	GameObject* OkayButton;

	Camera camera;
	bool isClick;
private:

};

#endif