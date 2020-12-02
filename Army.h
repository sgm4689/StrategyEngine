#pragma once
#include "Entity.h"
#include "SceneNode.h"
#include <DirectXMath.h>
#include <vector>
class Army
	:public Entity
{
public:
	Army(int troops, Mesh* mesh, Material* mat);
	Army(int troops, float radius, Mesh* mesh, Material* mat);
	void TakeDamage();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime, Camera* camera, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	void SetScale(float x, float y, float z);
	void SetPosition(float x, float y, float z);
	void SetRotation(float pitch, float yaw, float roll);
	SceneNode* GetTroops();
private:
	XMFLOAT2 GetPosition(float angle, float radius);
	SceneNode* units;
	float radius;
	float unitCount;
};

