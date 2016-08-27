#include "Buttons.h"
#include <Windows.h>

Buttons::Buttons()
{
	isClick = false;
}

Buttons::~Buttons()
{

}

void Buttons::Init(vector<GameObject *>* buttonList, MapLoad* m_cMap)
{

	PlayButton = new GameObject();
	PlayButton->type = GameObject::GO_PLAY;
	PlayButton->pos.Set(-20.f, -20.f, 0.f);
	PlayButton->scale.Set(30.f / m_cMap->GetTileSize(), 10.f / m_cMap->GetTileSize(), 0.f);
	PlayButton->active = false;
	buttonList->push_back(PlayButton);

	MenuButton = new GameObject();
	MenuButton->type = GameObject::GO_MENU;
	MenuButton->pos.Set(-20.f, -20.f, 0.f);
	MenuButton->scale.Set(30.f / m_cMap->GetTileSize(), 10.f / m_cMap->GetTileSize(), 0.f);
	MenuButton->active = false;
	buttonList->push_back(MenuButton);

	EditButton = new GameObject();
	EditButton->type = GameObject::GO_EDIT;
	EditButton->pos.Set(-20.f, -20.f, 0.f);
	EditButton->scale.Set(30.f / m_cMap->GetTileSize(), 10.f / m_cMap->GetTileSize(), 0.f);
	EditButton->active = false;
	buttonList->push_back(EditButton);

	LoadButton = new GameObject();
	LoadButton->type = GameObject::GO_LOAD;
	LoadButton->pos.Set(-20.f + camera.position.x, -20.f + camera.position.y, 1.f);
	MenuButton->scale.Set(30.f / m_cMap->GetTileSize(), 10.f / m_cMap->GetTileSize(), 0.f);
	LoadButton->active = false;
	buttonList->push_back(LoadButton);

	HighscoreButton = new GameObject();
	HighscoreButton->type = GameObject::GO_HIGHSCORE;
	HighscoreButton->pos.Set(-20.f + camera.position.x, -20.f + camera.position.y, 1.f);
	HighscoreButton->scale.Set(30.f, 10.f, 0.f);
	HighscoreButton->active = false;
	buttonList->push_back(HighscoreButton);

	SaveButton = new GameObject();
	SaveButton->type = GameObject::GO_SAVE;
	SaveButton->pos.Set(-20.f + camera.position.x, -20.f + camera.position.y, 1.f);
	SaveButton->scale.Set(30.f, 10.f, 0.f);
	SaveButton->active = false;
	buttonList->push_back(SaveButton);

	ExitButton = new GameObject();
	ExitButton->type = GameObject::GO_EXIT;
	ExitButton->pos.Set(-20.f + camera.position.x, -20.f + camera.position.y, 1.f);
	MenuButton->scale.Set(30.f / m_cMap->GetTileSize(), 10.f / m_cMap->GetTileSize(), 0.f);
	ExitButton->active = false;
	buttonList->push_back(ExitButton);

	RestartButton = new GameObject();
	RestartButton->type = GameObject::GO_RESTART;
	RestartButton->pos.Set(-20.f + camera.position.x, -20.f + camera.position.y, 1.f);
	RestartButton->scale.Set(30.f, 10.f, 0.f);
	RestartButton->active = false;
	buttonList->push_back(RestartButton);
}

void Buttons::Update(Vector3 mousepos, float dt, vector<GameObject *>* buttonList)
{
	isClick = false;
	for (vector<GameObject *>::iterator it = buttonList->begin(); it != buttonList->end(); ++it)
	{
		GameObject *go1 = (GameObject *)*it;
		if ((abs(go1->pos.x - mousepos.x) * 2 < (go1->scale.x) &&
			(abs(go1->pos.y - mousepos.y) * 2 < (go1->scale.y))))
		{
			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				isClick = true;
				button = go1;
				std::cout << button->type << std::endl;
			}
		}
		else
		{
			switch (go1->type)
			{
			case(GameObject::GO_PLAYHOVER) :
				go1->type = GameObject::GO_PLAY;
				go1->scale.Set(30.f, 10.f, 0.f);
				break;

			case(GameObject::GO_MENUHOVER) :
				go1->type = GameObject::GO_MENU;
				go1->scale.Set(30.f, 10.f, 0.f);
				break;

			case(GameObject::GO_EDITHOVER) :
				go1->type = GameObject::GO_EDIT;
				go1->scale.Set(30.f, 10.f, 0.f);
				break;

			case(GameObject::GO_LOADHOVER) :
				go1->type = GameObject::GO_LOAD;
				go1->scale.Set(30.f, 10.f, 0.f);
				break;

			case(GameObject::GO_HIGHSCOREHOVER) :
				go1->type = GameObject::GO_HIGHSCORE;
				go1->scale.Set(30.f, 10.f, 0.f);
				break;

			case(GameObject::GO_SAVEHOVER) :
				go1->type = GameObject::GO_SAVE;
				go1->scale.Set(30.f, 10.f, 0.f);
				break;

			case(GameObject::GO_EXITHOVER) :
				go1->type = GameObject::GO_EXIT;
				go1->scale.Set(30.f, 10.f, 0.f);
				break;

			case(GameObject::GO_RESTARTHOVER) :
				go1->type = GameObject::GO_RESTART;
				go1->scale.Set(30.f, 10.f, 0.f);
				break;
			}
		}
	}
}
