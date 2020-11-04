#pragma once
#include <Windows.h>
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
class Entity
{
	public:
		Entity(Mesh*, Material*);
		~Entity();
		Mesh* GetMesh();
		Transform* GetTransform();
		Material* GetMaterial();
	private:
		Transform transform;
		Mesh* mesh;
		Material* material;

};

