#include "Map.h"
Map::Map(int width, int height, float radius) {
	this->width = width;
	this->height = height;
	this->radius = radius;
	grid = new Entity * *[width];
	for (int i = 0;i < width;i++)
		grid[i] = new Entity * [height];
}

///Add to grid using screen position
bool Map::Add(Entity* ent){
	float x = ent->GetTransform()->GetPosition().x;
	float y = ent->GetTransform()->GetPosition().y;
	POINT pos = Screen2Grid(x, y);
	if (pos.x > width-1 || pos.x < 0 || pos.y > height || pos.y < 0)//validate input
		return false;
	grid[pos.x][pos.y] = ent;
	return true;
}

///Move to new position
bool Map::MoveAbsolute(Entity* ent, float x, float y) {
	float oldX = ent->GetTransform()->GetPosition().x;
	float oldY = ent->GetTransform()->GetPosition().y;
	POINT pos = Screen2Grid(x, y);
	POINT oldPos = Screen2Grid(oldX, oldY);
	if (pos.x > width-1 || pos.x < 0 || pos.y > height || pos.y < 0)//validate input
		return false;
	if (oldX!=x || oldY != y) {
		grid[pos.x][pos.y] = grid[oldPos.x][oldPos.y];
		grid[oldPos.x][oldPos.y] = nullptr;
	}
	ent->GetTransform()->SetPosition(x, y, ent->GetTransform()->GetPosition().z);
	return true;
}

///Move relative to old position
bool Map::MoveRelative(Entity* ent, float x, float y) {
	float oldX = ent->GetTransform()->GetPosition().x;
	float oldY = ent->GetTransform()->GetPosition().y;
	POINT pos = Screen2Grid(x, y);
	POINT oldPos = Screen2Grid(oldX, oldY);
	if (pos.x > width - 1 || pos.x < 0 || pos.y > height || pos.y < 0)//validate input
		return false;
	if (oldX != x || oldY != y) {
		grid[pos.x][pos.y] = grid[oldPos.x][oldPos.y];
		grid[oldPos.x][oldPos.y] = nullptr;
	}
	ent->GetTransform()->MoveRelative(x, y, 0);
	return true;
}

bool Map::Remove(Entity* ent) {
	float x = ent->GetTransform()->GetPosition().x;
	float y = ent->GetTransform()->GetPosition().y;
	POINT pos = Screen2Grid(x, y);
	if (pos.x > width - 1 || pos.x < 0 || pos.y > height || pos.y < 0)//validate input
		return false;
	grid[pos.x][pos.y] = nullptr;
	return true;
}

POINT Map::GetGridPosition(Entity* ent) {
	float x = ent->GetTransform()->GetPosition().x;
	float y = ent->GetTransform()->GetPosition().y;
	return Screen2Grid(x, y);
}

POINT Map::Grid2Screen(int x, int y) {
	if (y % 2 == 0) {//offset grid
		return POINT{ 
			(long)radius * 2 * x,
			(long)(radius * 2 * 0.75f * y)
		};
	}
	else {

		return POINT{ 
			(long)(radius * 2 * x + radius),
			(long)(radius * 2 * 0.75f * y)
 };
	}
	return POINT{ -1,-1 };
}

POINT Map::Screen2Grid(float x, float y) {
	return POINT{ 
		(long)round(x / (radius * 2)),
		(long)round(y / (radius * 2 * 0.75f))
	};
}