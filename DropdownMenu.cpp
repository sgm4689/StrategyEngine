#include "DropdownMenu.h"

DropdownMenu::DropdownMenu(Button** list, int listLength, Mesh* mesh, Material* mat, float width, float height, HWND hWnd) {
	this->list = list;
	this->listLength = listLength;
	this->min = DirectX::XMFLOAT2(-width/2,-height/2 );
	this->max = DirectX::XMFLOAT2(width/2,height/2 );
	this->mesh = mesh;
	this->mat = mat;
	transform = new Transform();

	//Overlay all of the buttons
	for (int x = 0; x < listLength; x++) {
		list[x]->GetTransform()->SetPosition(0,0,list[x]->GetTransform()->GetPosition().z);
		list[x]->isActive = false;
	}

	eventHandler->BindEvent(M1Down, &DropdownMenu::OnClick, this);
}

DropdownMenu::DropdownMenu(Button** list, int listLength, Mesh* mesh, Material* mat, float width, float height, float x, float y, HWND hWnd) {
	this->list = list;
	this->listLength = listLength;
	this->min = DirectX::XMFLOAT2( x-width/2,y-width/2 );
	this->max = DirectX::XMFLOAT2(x + width/2,y + height/2 );
	this->mesh = mesh;
	this->mat = mat;
	transform = new Transform(XMFLOAT3(x, y, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(width, height, 1));

	//Overlay all of the buttons
	for (int j = 0; j < listLength; j++) {
		list[j]->GetTransform()->SetPosition(x, y, list[j]->GetTransform()->GetPosition().z);
		list[j]->isActive = false;
	}

	eventHandler->BindEvent(M1Down, &DropdownMenu::OnClick, this);
}

Material* DropdownMenu::GetMaterial() {
	return mat;
}

Transform* DropdownMenu::GetTransform() {
	return transform;
}

Mesh* DropdownMenu::GetMesh() {
	return mesh;
}

//Drops down all of the buttons, and sets them active
bool DropdownMenu::OnClick() {

	//Check if menu is pressed
	POINT mouse;

	//Get Mouse position, then convert to world space
	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);
	Camera* cam = main->GetMain();
	DirectX::XMFLOAT2* mouseW = cam->ClientToWorld(&mouse);

	//Then activate the buttons
	if (mouseW->x > min.x&& mouseW->y > min.y&& mouseW->x < max.x && mouseW->y < max.y)
	{
		DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(transform->GetPosition().x, transform->GetPosition().y - (max.y - min.y));
		if (!extended) {
			//move all of the buttons
			for (int x = 0; x < listLength; x++) {
				list[x]->GetTransform()->SetPosition(position.x, position.y, list[x]->GetTransform()->GetPosition().z);
				list[x]->isActive = true;
				position.y -= list[x]->GetDimentions().y;
			}
			extended = !extended;
			return true;
		}
		else {
			//Overlay all of the buttons
			for (int x = 0; x < listLength; x++) {
				list[x]->isActive = false;
				list[x]->GetTransform()->SetPosition(transform->GetPosition().x, transform->GetPosition().y, list[x]->GetTransform()->GetPosition().z);
			}
			extended = !extended;
			return true;
		}
	}
	return false;
}