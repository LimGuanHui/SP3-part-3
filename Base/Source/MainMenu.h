#ifndef MAINMENU_H
#define MAINMENU_H

#include "SceneBase.h"
#include "Buttons.h"
#include "GameObject.h"
#include "Character.h"
#include <vector>
using namespace CHARACTER;

class MainMenu : public SceneBase
{
public:
	MainMenu();
	~MainMenu();

	int i;

	void Init(Buttons* button, CCharacter* character);  //(CCharacter* Character, Buttons* button,bool *quitegame);
	void Update(double dt);
	void RenderMenu(MapLoad* m_cMap);

	GameObject* FetchGO();

	enum GameState
	{
		Help,
		Edit,
		Menu,
		Pause,
		Game,
		End,
		Win,
	};

	enum StartOptions
	{
		Start,
		Instructions,
		Quit,
		END,
	};
	StartOptions StartOp;
	int startArrow;

	enum PauseOptions
	{
		Resume,
		ReturnMenu,
		Quit2,
		END2,
	};
	PauseOptions PauseOp;
	int pauseArrow;

	enum DeadOptions
	{
		Restart,
		ReturnMenu3,
		END3,
	};
	DeadOptions DeadOp;
	int deadArrow;

	enum WinOptions
	{
		ReturnMenu4,
		Quit4,
		END4,
	};
	WinOptions WinOp;
	int winArrow;


	GameState gamestate;
	Buttons* button;
	CCharacter* Character;
	int m_objectCount = 0;
	bool playerDead;
	std::vector<GameObject *> m_goList;
	bool QuitGame;
	bool RestartGame;

protected:
	float InputDelayTimer;
	float m_worldWidth;
	float m_worldHeight;
};

#endif