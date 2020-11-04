#pragma once
#include "Button.h"
#include "MainCamera.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "EventHandler.h"
#include <DirectXMath.h>

class DropdownMenu
{
public:
	DropdownMenu(Button** list, int listLength, Mesh* mesh, Material* mat, float width, float height, HWND hWnd);
	DropdownMenu(Button** list, int listLength, Mesh* mesh, Material* mat, float width, float height, float x, float y, HWND hWnd);
	Material* GetMaterial();
	Transform* GetTransform();
	Mesh* GetMesh();
private:
	MainCamera* main = MainCamera::GetInstance();
	EventHandler* eventHandler = EventHandler::GetInstance();
	Button** list;
	int listLength;
	DirectX::XMFLOAT2 min, max;
	HWND hWnd;
	Transform* transform;
	Material* mat;
	Mesh* mesh;
	bool OnClick();
	bool extended;
};

