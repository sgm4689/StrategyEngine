#pragma once
#include "Camera.h"
class MainCamera
{
public:
	static MainCamera* GetInstance();
	bool SetMain(Camera* main);
	Camera* GetMain();
private:
	MainCamera();//Private constructor so there's only ever one
	Camera* main;
};

