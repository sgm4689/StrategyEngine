#pragma once
#include <Windows.h>
#include <conio.h>
#include "EventHandler.h"
class InputManager
{
public:
	void Update(float dt, HWND windowHandle);
	static InputManager* GetInstance();
private:
	InputManager();
	bool m1Down, m2Down, m3Down;
	EventHandler* eventHandler;
};

