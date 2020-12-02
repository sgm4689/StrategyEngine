#pragma once
#include <DirectXMath.h>
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "MainCamera.h"
#include "EventHandler.h"
#include "Entity.h"

class Button
	:public Entity
{
public:
	Button(Mesh* mesh, Material*, float width, float height, HWND hWnd);
	Button(Mesh* mesh, Material*, float width, float height, float x, float y, HWND hWnd);

	template <typename T>
	void SetClick(bool(T::* method)(void), T* obj) {
		onPress = std::bind(method, obj);
	}

	DirectX::XMFLOAT2 GetDimentions();
	bool isActive;
private:
	MainCamera* main;
	voidFunc onPress;
	long width, height;
	HWND hWnd;
	bool OnClick();
	EventHandler* eventHandler = EventHandler::GetInstance();
};

