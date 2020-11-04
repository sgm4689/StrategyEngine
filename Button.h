#pragma once
#include <DirectXMath.h>
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "MainCamera.h"
#include "EventHandler.h"
class Button
{
	typedef void (*func_t)();
public:
	Button(Mesh* mesh, Material*, float width, float height, func_t onClick, HWND hWnd);
	Button(Mesh* mesh, Material*, float width, float height, float x, float y, func_t onPress, HWND hWnd);
	Mesh* GetMesh();
	Transform* GetTransform();
	Material* GetMaterial();
	DirectX::XMFLOAT2 GetDimentions();
	bool isActive;
private:
	Transform transform;
	Mesh* mesh;
	MainCamera* main;
	Material* material;
	func_t onPress;
	long width, height;
	HWND hWnd;
	bool OnClick();
	EventHandler* eventHandler = EventHandler::GetInstance();
};

