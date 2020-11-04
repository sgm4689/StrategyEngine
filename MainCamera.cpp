#include "MainCamera.h"

MainCamera::MainCamera() {
	main = nullptr;
}

MainCamera* mainCamera = nullptr;

MainCamera* MainCamera::GetInstance() {
	if (mainCamera == nullptr)
		mainCamera = new MainCamera();
	return mainCamera;
}

Camera* MainCamera::GetMain() {
	return main;
}

bool MainCamera::SetMain(Camera* main) {
	if (main != nullptr) {
		this->main = main;
		return true;
	}
	return false;
}
