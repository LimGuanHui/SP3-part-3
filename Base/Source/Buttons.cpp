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
	LoadButton->pos.Set(-20.f, -20.f, 1.f);
	MenuButton->scale.Set(30.f / m_cMap->GetTileSize(), 10.f / m_cMap->GetTileSize(), 0.f);
	LoadButton->active = false;
	buttonList->push_back(LoadButton);

	HighscoreButton = new GameObject();
	HighscoreButton->type = GameObject::GO_HIGHSCORE;
	HighscoreButton->pos.Set(-20.f, -20.f, 1.f);
	HighscoreButton->scale.Set(30.f / m_cMap->GetTileSize(), 10.f / m_cMap->GetTileSize(), 0.f);
	HighscoreButton->active = false;
	buttonList->push_back(HighscoreButton);

	SaveButton = new GameObject();
	SaveButton->type = GameObject::GO_SAVE;
	SaveButton->pos.Set(-20.f, -20.f, 1.f);
	SaveButton->scale.Set(30.f / m_cMap->GetTileSize(), 10.f / m_cMap->GetTileSize(), 0.f);
	SaveButton->active = false;
	buttonList->push_back(SaveButton);

	ExitButton = new GameObject();
	ExitButton->type = GameObject::GO_EXIT;
	ExitButton->pos.Set(-20.f, -20.f, 1.f);
	MenuButton->scale.Set(30.f / m_cMap->GetTileSize(), 10.f / m_cMap->GetTileSize(), 0.f);
	ExitButton->active = false;
	buttonList->push_back(ExitButton);

	RestartButton = new GameObject();
	RestartButton->type = GameObject::GO_RESTART;
	RestartButton->pos.Set(-20.f, -20.f, 1.f);
	RestartButton->scale.Set(30.f / m_cMap->GetTileSize(), 10.f / m_cMap->GetTileSize(), 0.f);
	RestartButton->active = false;
	buttonList->push_back(RestartButton);

	HelpButton = new GameObject();
	HelpButton->type = GameObject::GO_HELP;
	HelpButton->pos.Set(-20.f, -20.f, 1.f);
	HelpButton->scale.Set(30.f / m_cMap->GetTileSize(), 10.f / m_cMap->GetTileSize(), 0.f);
	HelpButton->active = false;
	buttonList->push_back(HelpButton);

	OkayButton = new GameObject();
	OkayButton->type = GameObject::GO_OKAYHOVER;
	OkayButton->pos.Set(-20.f, -20.f, 1.f);
	OkayButton->scale.Set(30.f / m_cMap->GetTileSize(), 10.f / m_cMap->GetTileSize(), 0.f);
	OkayButton->active = false;
	buttonList->push_back(OkayButton);
}