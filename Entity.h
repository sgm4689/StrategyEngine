#pragma once
#include <Windows.h>
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
class Entity
{
	public:
		Entity();
		Entity(Mesh*, Material*);
		~Entity();
		Mesh* GetMesh();
		Transform* GetTransform();
		Transform* GetLocalTransform();
		Material* GetMaterial();
	protected:
		Transform transform;
		Transform localTransform;
		Mesh* mesh;
		Material* material;

};

