#include "Entity.h"
Entity::Entity(Mesh* mesh, Material* material) {
	this->mesh = mesh;
	this->material = material;
	transform = Transform();
}
Entity::~Entity() {

}
Mesh* Entity::GetMesh() {
	return mesh;
}
Transform* Entity::GetTransform() {
	return &transform;
}

Material* Entity::GetMaterial() {
	return material;
}