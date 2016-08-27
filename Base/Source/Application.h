
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
//#include "SceneText.h"
#include "SP3.h"
#include <irrKlang.h>
using namespace irrklang;
class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	bool GetMouseUpdate();
	//Declare variables to store the last and current mouse position
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;

	static int GetWindowWidth();
	static int GetWindowHeight();
    ISoundEngine* theSoundEngine;
    ISoundSource* BGM;

private:
	Application();
	~Application();

	// Declare the window width and height as constant integer
	int m_window_deadzone = 100;
	int m_window_width = 1024;
	int m_window_height = 800;
	//Declare a window object
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_ThreadOne;
	double m_dAccumulatedTime_ThreadTwo;

	// The handler for the scene
	SP3 *scene;
};

#endif