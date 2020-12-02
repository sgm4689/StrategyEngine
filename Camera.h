#pragma once
#include <Windows.h>
#include "Entity.h"
#include "Transform.h"
#include "EventHandler.h"
class Camera
{
	public:
		Camera(
			XMFLOAT3 position, 
			XMFLOAT3 orientation, 
			float aspect, 
			float FOV
		);
		Camera(
			XMFLOAT3 position,
			XMFLOAT3 orientation,
			float aspect,
			float FOV,
			float nearClip,
			float farClip,
			float moveSpeed,
			float lookSpeed
		);
		~Camera();
		void UpdateViewMatrix();
		void UpdateProjectionMatrix(float aspect);
		void Update(float dt, HWND windowHandle);
		bool OnClick();
		bool OnDrag();
		XMFLOAT2* ClientToWorld(LPPOINT clientP);
		Transform* GetTransform();
		XMFLOAT4X4 M4_view;
		XMFLOAT4X4 M4_projection;
	private:
		Transform transform;
		POINT P_lastPos;
		float FOV;
		float nearClip;
		float farClip;
		float moveSpeed;
		float lookSpeed;
		EventHandler* eventHandler;
		HWND wh;
		Camera* main;
};

