#include "Entity.h"
Entity::Entity(Mesh* mesh, Material* material) {
	this->mesh = mesh;
	this->material = material;
	transform = Transform();
	localTransform = Transform();
}
Entity::~Entity() {

}

Mesh* Entity::GetMesh() {
	return mesh;
}
Transform* Entity::GetTransform() {
	return &transform;
}

Transform* Entity::GetLocalTransform() {
	return &localTransform;
}

Material* Entity::GetMaterial() {
	return material;
}