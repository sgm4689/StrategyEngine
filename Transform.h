#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class Transform
{
	public:
		Transform();
		Transform(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale);
		~Transform();

		//Setters
		void SetPosition(float x, float y, float z);
		void SetRotation(float pitch, float yaw, float roll);
		void SetScale(float x, float y, float z);

		//Getters
		XMFLOAT3 GetPosition();
		XMFLOAT3 GetPitchYawRoll();
		XMFLOAT3 GetScale();
		XMFLOAT4X4 GetWorldMatrix();

		//Transformers
		void MoveAbsolute(float x, float y, float z);
		void MoveRelative(float x, float y, float z);
		void Rotate(float pitch, float yaw, float roll);
		void Scale(float x, float y, float z);

	
private:
	XMFLOAT3 V3_position;
	XMFLOAT3 V3_scale;
	XMFLOAT3 V3_rotation;
	XMFLOAT4X4 M4_world;

	void CreateWorld();
};

