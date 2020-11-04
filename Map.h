#pragma once
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <vector>
#include <math.h>
#include "StrategyEngine.h"

class Map
{
public:
	//Create Grid
	Map(int width, int height, float radius);

	//Manipulate Grid Content
	bool Add(Entity* ent);
	bool MoveAbsolute(Entity* ent, float x, float y);
	bool MoveRelative(Entity* ent, float x, float y);
	bool Remove(Entity* ent);
	POINT GetGridPosition(Entity* ent);
private:
	//Helper functions
	POINT Grid2Screen(int x, int y);
	POINT Screen2Grid(float x, float y);

	int width, height;
	float radius;
	Entity*** grid;//A 2D array of Entity pointers
};

