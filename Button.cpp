#include "Button.h"
#include <iostream>

Button::Button(Mesh* mesh, Material* material, float width, float height, HWND hWnd)
	:Entity(mesh, material){
		this->mesh = mesh;
		this->material = material;
		this->onPress = onPress;
		transform = Transform();
		this->width = width;
		this->height = height;
		this->hWnd = hWnd;

		eventHandler->BindEvent(M1Down, &Button::OnClick, this);
		main = MainCamera::GetInstance();
		isActive = true;
}

Button::Button(Mesh* mesh, Material* material, float width, float height, float x, float y, HWND hWnd)
	:Entity(mesh, material) {
		this->mesh = mesh;
		this->material = material;
		transform = Transform(XMFLOAT3(x,y,0), XMFLOAT3(0,0,0), XMFLOAT3(width,height,1));
		this->onPress = onPress;
		this->hWnd = hWnd;

		this->width = width;
		this->height = height;

		eventHandler->BindEvent(M1Down, &Button::OnClick, this);
		main = MainCamera::GetInstance();
		isActive = true;
}

//Gets the width and height of the button
XMFLOAT2 Button::GetDimentions() {
	return DirectX::XMFLOAT2(width, height);
}

bool Button::OnClick() {
	if (!isActive)
		return false;
	POINT mouse;

	//Get Mouse position, then convert to world space
	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);
	Camera* cam = main->GetMain();
	DirectX::XMFLOAT2* mouseW = cam->ClientToWorld(&mouse);

	//Compare to the min & max of the button
	if (
		mouseW->x > transform.GetPosition().x - (width / 2.0f) && mouseW->y > transform.GetPosition().y - (height / 2.0f) &&
		mouseW->x < transform.GetPosition().x + (width / 2.0f) && mouseW->y < transform.GetPosition().y + (height / 2.0f)
	) {
		onPress();
		return true;
	}
	return false;
}