#include "Transform.h"
Transform::Transform() {
	V3_position = XMFLOAT3(0.0f,0.0f,0.0f);
	V3_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	V3_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMStoreFloat4x4(&M4_world, XMMatrixIdentity());
}
Transform::Transform(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale) {
	V3_position = position;
	V3_rotation = rotation;
	V3_scale = scale;
	XMStoreFloat4x4(&M4_world, XMMatrixIdentity());

}
Transform::~Transform() {

}
//Getters
void Transform::SetPosition(float x, float y, float z) {
	V3_position = XMFLOAT3(x, y, z);
}
void Transform::SetRotation(float pitch, float yaw, float roll) {
	V3_rotation = XMFLOAT3(pitch, yaw, roll);
}
void Transform::SetScale(float x, float y, float z) {
	V3_scale = XMFLOAT3(x, y, z);
}

//Setters
XMFLOAT3 Transform::GetPosition() {
	return V3_position;
}
XMFLOAT3 Transform::GetPitchYawRoll() {
	return V3_rotation;
}
XMFLOAT3 Transform::GetScale() {
	return V3_scale;
}
XMFLOAT4X4 Transform::GetWorldMatrix() {
	CreateWorld();
	return M4_world;
}

//Transformers
void Transform::MoveRelative(float x, float y, float z) {
	XMVECTOR absolute = XMVectorSet(x, y, z, 0.0f);
	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(V3_rotation.x, V3_rotation.y, V3_rotation.z);
	XMVECTOR rotated = XMVector3Rotate(absolute, rotation);
	XMVECTOR direction = XMLoadFloat3(&V3_position);
	direction += rotated;//The new position of the object
	XMStoreFloat3(&V3_position, direction);
}
void Transform::MoveAbsolute(float x, float y, float z) {
	V3_position = XMFLOAT3(V3_position.x + x, V3_position.y + y, V3_position.z + z);
}
void Transform::Rotate(float pitch, float yaw, float roll) {
	V3_rotation = XMFLOAT3(V3_rotation.x + pitch, V3_rotation.y + yaw, V3_rotation.z + roll);
}
void Transform::Scale(float x, float y, float z) {
	V3_scale = XMFLOAT3(V3_scale.x * x, V3_scale.y * y, V3_scale.z * z);
}

void Transform::CreateWorld() {
	XMMATRIX scale = XMMatrixScaling(V3_scale.x, V3_scale.y, V3_scale.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(V3_rotation.x, V3_rotation.y, V3_rotation.z);
	XMMATRIX translation = XMMatrixTranslation(V3_position.x, V3_position.y, V3_position.z);
	XMMATRIX world = scale * rotation * translation;

	XMStoreFloat4x4(&M4_world, world);
}