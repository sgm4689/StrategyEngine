#include "InputManager.h"

InputManager::InputManager() {
	eventHandler = EventHandler::GetInstance();
	m1Down, m2Down, m3Down = false;
}

InputManager* inputManager = nullptr;

InputManager* InputManager::GetInstance() {
	if (inputManager == nullptr)
		inputManager = new InputManager();
	return inputManager;
}

void InputManager::Update(float dt, HWND windowHandle) {
	//Check for Mouse 1 status
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		if (!m1Down) {
			eventHandler->DispatchEvent(M1Down);
			m1Down = true;
		}
		else {
			eventHandler->DispatchEvent(M1Hold);
			m1Down = true;
		}
	}
	else if (m1Down) {
		m1Down = false;
		eventHandler->DispatchEvent(M1Up);
	}

	//Check for Mouse 2 status
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		if (!m2Down) {
			eventHandler->DispatchEvent(M2Down);
			m2Down = true;
		}
		else {
			eventHandler->DispatchEvent(M2Hold);
			m2Down = true;
		}
	}
	else if (m2Down) {
		m2Down = false;
		eventHandler->DispatchEvent(M2Up);
	}

	//Check for Mouse 3 status
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
		if (!m3Down) {
			eventHandler->DispatchEvent(M3Down);
			m3Down = true;
		}
		else {
			eventHandler->DispatchEvent(M3Hold);
			m3Down = true;
		}
	}
	else if (m3Down) {
		m3Down = false;
		eventHandler->DispatchEvent(M3Up);
	}
}