#include "Army.h"

Army::Army(int troops, Mesh* mesh, Material* mat) : Entity(mesh, mat){
	units = new SceneNode(new Entity(mesh, mat));//Set a unit at the center of the army
	units->GetEntity()->GetTransform()->SetScale(0.25f, 0.25f, 0.25f);
	float angle = 360 / troops;
	for (int i = 0; i < troops; i++) {
		XMFLOAT2 position = GetPosition(angle * i, 1);
		SceneNode* unit = units->AddChild(new Entity(mesh, mat));
		unit->GetEntity()->GetLocalTransform()->SetPosition(position.x, position.y, 0);
		unit->GetEntity()->GetTransform()->SetScale(0.25f, 0.25f, 0.25f);
	}
	unitCount = troops;
	radius = 1;
}

Army::Army(int troops, float radius, Mesh* mesh, Material* mat) : Entity(mesh, mat) {
	units = new SceneNode(new Entity(mesh, mat));//Set a unit at the center of the army
	units->GetEntity()->GetTransform()->SetScale(0.25f, 0.25f, 0.25f);
	float angle = 360 / troops;
	for (int i = 0; i < troops; i++) {
		XMFLOAT2 position = GetPosition(angle * i, radius);
		SceneNode* unit = units->AddChild(new Entity(mesh, mat));
		unit->GetEntity()->GetLocalTransform()->SetPosition(position.x, position.y, 0);
		unit->GetEntity()->GetTransform()->SetScale(0.25f, 0.25f, 0.25f);
	}
	unitCount = troops;
	this->radius = radius;
}

SceneNode* Army::GetTroops() {
	return units;
}


void Army::SetScale(float x, float y, float z) {
	units->GetEntity()->GetTransform()->SetScale(x, y, z);
}

void Army::SetPosition(float x, float y, float z) {
	units->GetEntity()->GetTransform()->SetPosition(x, y, z);
}

void Army::SetRotation(float pitch, float yaw, float roll) {
	units->GetEntity()->GetTransform()->SetRotation(pitch, yaw, roll);
}

void Army::Update(float deltaTime, float totalTime) {
	units->Update(deltaTime, totalTime);
}

void Army::Draw(float deltaTime, float totalTime, Camera* camera, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context){
	units->Draw(deltaTime, totalTime, camera, context);
}

void Army::TakeDamage() {
	unitCount--;
	float angle = 360 / unitCount;
	units->RemoveChild(unitCount);//remove the last unit in the army
	for (int i = 0; i < unitCount; i++) {
		XMFLOAT2 position = GetPosition(angle * i, radius);
		SceneNode* unit = units->GetChild(i);
		unit->GetEntity()->GetLocalTransform()->SetPosition(position.x, position.y, 0);
		unit->GetEntity()->GetTransform()->SetScale(0.25f, 0.25f, 0.25f);
	}
}

XMFLOAT2 Army::GetPosition(float angleF, float radius) {
	float angleR = XMConvertToRadians(angleF);
	return XMFLOAT2(radius * cos(angleR), radius * sin(angleR));
}
